#include "Common.hpp"
#include <shellapi.h>
#include "ImGui/imgui.h"
#include "Executables.hpp"
#include "Auth.hpp"
#include "Authpoint.hpp"

#pragma warning (push)
#pragma warning (disable: 4389)

namespace Mystic
{
	
	
	bool MenuScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType MenuScript::GetType()
	{
		return ScriptType::Game;
	}

	void MenuScript::Initialize()
	{
		
		using namespace UserInterface;

		//Header Stuff
		g_Settings.m_Header = "mystic_hd";
		uint32_t textureID;
		char buffers[128];
		std::sprintf(buffers, "%s//Mystic//GTA//Headers//mystic_hd.ytd", std::getenv("appdata"), &g_Settings.m_Header);
		char mainname[128];
		std::sprintf(mainname, "mystic_hd.ytd", &g_Settings.m_Header);
		g_GameFunctions->m_FileRegister(&textureID, buffers, true, mainname, false);

		
		
		//End Header Stuff
		m_Initialized = true;
		Ped selectedPlayerPed;
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		g_CustomText->AddText(joaat("HUD_JOINING"), "Playing with Mystic");
		g_CustomText->AddText(joaat("HUD_TRANSP"), "Playing with Mystic");

		int c_PlayerLevel = { globalHandle(1589819).At(PLAYER::PLAYER_ID(), 818).At(211).At(6).As<int>() }; //Player's current level.

#pragma region Main Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Main Menu", SubmenuHome, [](RegularSubmenu* sub)
		{
			if (g_Settings.m_ExperimentalFeatures)
			{
				sub->AddOption<SubOption>("Experimental", nullptr, SubmenuExperimental);
			}
				sub->AddOption<SubOption>("Local Menu", nullptr, SubmenuSelf);
				sub->AddOption<SubOption>("Network Menu", nullptr, SubmenuNetowrk);
				sub->AddOption<SubOption>("Vehicle Menu", nullptr, SubmenuVehicle);
				sub->AddOption<SubOption>("Weapon Menu", nullptr, SubmenuWeapon);
				sub->AddOption<SubOption>("Recovery Menu", nullptr, SubmenuRecovery);
				sub->AddOption<SubOption>("World Menu", nullptr, SubmenuWorld);
				sub->AddOption<SubOption>("Misc Menu", nullptr, SubmenuMisc);
				sub->AddOption<SubOption>("Teleport Manager", nullptr, SubmenuTeleportManager);
				sub->AddOption<SubOption>("Protections Menu", nullptr, SubmenuProtections);
				sub->AddOption<SubOption>("Menu Info", nullptr, SubmenuInfo);
				sub->AddOption<SubOption>("Settings", nullptr, SubmenuSettings);
				});
		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport Manager", SubmenuTeleportManager, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Teleport to Waypoint", "Teleports you to your selected waypoint.", [=]
			{
					Utils::tpToMarker();
			});
			sub->AddOption<RegularOption>("Teleport to Nearest Vehicle", "Teleports you to your closest vehicle.", [=]
			{
					Functions::teleportNearestCar(PLAYER::PLAYER_PED_ID());
			});
			sub->AddOption<RegularOption>("Add TP Position", "Adds your position to the TP list.", [=]
			{
				g_Settings.TPWindowActivated = true;
			});
			sub->AddOption<SubOption>("Your Teleport Locations", nullptr, SubmenuTeleports);
			sub->AddOption<SubOption>("Preset Teleport Locations", nullptr, SubmenuPresetTeleports);
		});
#pragma endregion
#pragma region Experimental Menu

		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport Locations", SubmenuTeleports, [](RegularSubmenu* sub)
		{
			namespace fs = std::filesystem;
			fs::directory_iterator dirIt{g_TeleportManager->GetTeleportDirectory() };
			for (auto&& dirEntry : dirIt)
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (path.has_filename() && !fs::is_empty(path))
					{
						sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
						{
							fs::path m_FilePath;
							char prefix[64] = {};
							std::snprintf(prefix, sizeof(prefix) - 1, "%s%s", path.stem().u8string(), ".mys");

							m_FilePath.append(std::getenv("appdata"));
							m_FilePath.append(Mystic_NAME);
							m_FilePath.append("GTA\\");
							m_FilePath.append("Teleports\\");

							m_FilePath.append(prefix);

							std::ifstream ifs(m_FilePath);

							std::string content((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
							std::stringstream ss(content);
							std::vector<float> result;

							while (ss.good())
							{
								std::string substr;
								std::getline(ss, substr, ',');
								result.push_back(::atof(substr.c_str()));
							}
							if (PED::IS_PED_IN_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), true))
							{
								Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
								ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, result[0], result[1], result[2], false, false, false);
							}
							else {
								ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), result[0], result[1], result[2], false, false, false);
							}
						});
					}
					else
					{
						sub->AddOption<RegularOption>("No Teleports Found", "Add some to use them!");
					}
				}
			}

		});
#pragma endregion
#pragma region Teleports
		g_UiManager->AddSubmenu<RegularSubmenu>("Preset TP Locations", SubmenuPresetTeleports, [](RegularSubmenu* sub) {
			Entity entity = PLAYER::PLAYER_PED_ID();
			sub->AddOption<RegularOption>("Island", nullptr, [=] { Functions::tpToDestination(entity, { 336.158, 4373.934, 64.46 }); });
			sub->AddOption<RegularOption>("Boat Under Water", nullptr, [=] { Functions::tpToDestination(entity, { 3156.38355,-355.16663, -20.09742 }); });
			sub->AddOption<RegularOption>("Cave In Sea", nullptr, [=] { Functions::tpToDestination(entity, { 3067.58, 2212.25, 3.00 }); });
			sub->AddOption<RegularOption>("Galileo Observatory", nullptr, [=] { Functions::tpToDestination(entity, { -430.905, 1135.2722, 325.904 }); });
			sub->AddOption<RegularOption>("Vinewood Sign", nullptr, [=] { Functions::tpToDestination(entity, { 719.30,1204.76,325.88 }); });
			sub->AddOption<RegularOption>("Beach", nullptr, [=] { Functions::tpToDestination(entity, { -256.43,6569.93, 2.66 }); });
			sub->AddOption<RegularOption>("Mine Shaft", nullptr, [=] { Functions::tpToDestination(entity, { -604.92, 2113.95, 127.11 }); });
			sub->AddOption<RegularOption>("Maze Bank", nullptr, [=] { Functions::tpToDestination(entity, { -74.94243,-818.63446, 326.174347 }); });
			sub->AddOption<RegularOption>("Military Base", nullptr, [=] { Functions::tpToDestination(entity, { -2012.8470f, 2956.5270f, 32.8101f }); });
			sub->AddOption<RegularOption>("Zancudo Tower", nullptr, [=] { Functions::tpToDestination(entity, { -2356.0940, 3248.645, 101.4505 }); });
			sub->AddOption<RegularOption>("Mask Shop", nullptr, [=] { Functions::tpToDestination(entity, { -1338.16, -1278.11, 4.87 }); });
			sub->AddOption<RegularOption>("LSC", nullptr, [=] { Functions::tpToDestination(entity, { -373.01, -124.91, 38.31 }); });
			sub->AddOption<RegularOption>("Ammunation", nullptr, [=] { Functions::tpToDestination(entity, { 247.3652, -45.8777, 69.9411 }); });
			sub->AddOption<RegularOption>("Airport", nullptr, [=] { Functions::tpToDestination(entity, { -1102.2910f, -2894.5160f, 13.9467f }); });
			
			});
#pragma endregion
#pragma region Self Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Self Menu", SubmenuSelf, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>(D_TRANSLATE("Godmode"), D_TRANSLATE("GOD_DESC"), &f_GodMode, BoolDisplay::OnOff, false); 
			sub->AddOption<RegularOption>("Max Armor", "defend yourself the best you can.", [=]
			{
				Functions::maxarmor();
			});
			sub->AddOption<RegularOption>("Max Health", "defend yourself the best you can.", [=]
			{
				Functions::maxHealth(); 
			});
			sub->AddOption<BoolOption<bool>>(D_TRANSLATE("Never_Wanted"), D_TRANSLATE("NEVERWANT_DESC"), &f_NeverWanted, BoolDisplay::OnOff, false);
			sub->AddOption<NumberOption<int>>(D_TRANSLATE("Wanted_Level"), D_TRANSLATE("WANTLVL_DESC"), &f_WantedLevel, 0, 5, 1, 1, false, "", "", []
			{
				Functions::setWantedLevel(f_WantedLevel);
			});
			sub->AddOption<BoolOption<bool>>("Six Star Wanted Level", "Go beyond the most wanted outlaw.", &f_sixstars, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Off the Radar", "Sets you off the radar", &f_otr, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Bribe Authorities", "Police will ignore all your activities", &f_bribeAuthorities, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Invisibility", "Sets your player to be invisible.", &f_invisibleplayer, BoolDisplay::OnOff, false); 
			sub->AddOption<BoolOption<bool>>("No Ragdoll", "Don't get caught up.", &f_norag, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Slow Motion", nullptr, &f_SlowMoBool, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("No Clip", nullptr, &f_noClip, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Small Player", "Shrinks your player model.", &f_tiny, BoolDisplay::OnOff, false); 
			sub->AddOption<BoolOption<bool>>("Super Jump", "Makes your ped jump really high.", &f_superJump, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Super Speed", "Makes you run like the flash", &f_superSpeed, BoolDisplay::OnOff, false);
			if (f_superSpeed) sub->AddOption<NumberOption<float>>("Speed", nullptr, &f_runspeed, 0, 10, 1, 1, false);
			sub->AddOption<BoolOption<bool>>("Fast Swim", "Swim like your life depends on it.", &f_superSwim, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti-AFK", "Makes sure you don't idle out.", &f_antiAFK, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Slide Run", "Move out quickly.", &f_slideRun, BoolDisplay::OnOff, false);
			//sub->AddOption<BoolOption<bool>>("Dragon Breath", "Shoot some flames out of your mouth.", &f_dragonbreath, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Mobile Radio", nullptr, &f_mobileRadio, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Forcefield", nullptr, &f_forcefield, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Superman", nullptr, &f_superMan, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Disable Phone", "Removed your phone so no pesky calls.", &f_disphone, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Rainbow Hair", "Sets your hair through all colors.", &f_rainbowHair, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Hair Color", nullptr, &Lists::hair_color, &Lists::hair_color_id, false, [] {Functions::setPedHairColor(Lists::hair_index[Lists::hair_color_id]); });
			sub->AddOption<RegularOption>("Self Clone", nullptr, [=] {Functions::clonePlayer(PLAYER::PLAYER_PED_ID()); });
			sub->AddOption<RegularOption>("Clean Ped", "Cleans your ped, as good as new.",[=]{Functions::cleanPed();});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Walk Style", nullptr, &Lists::walk_style, &Lists::walk_id, false, [] {g_FiberScript->addTask([=] {
				STREAMING::REQUEST_ANIM_SET(Lists::walkDict[Lists::walk_id]);
				while (!STREAMING::HAS_ANIM_SET_LOADED(Lists::walkDict[Lists::walk_id])) g_FiberScript->Wait(0);
				PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), Lists::walkDict[Lists::walk_id], 1.0f);
				}); });
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Scenarios", nullptr, &Lists::animation, &Lists::anim_pos, false, []{Functions::anim(Lists::anim_pos);});
			sub->AddOption<RegularOption>("Stop All Scenarios", "Stops the current scenario.", [=]{BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());});
			sub->AddOption<SubOption>("Wardrobe", nullptr, SubmenuWardrobe);
			if (g_Settings.m_authLevel = 3) { f_stealthlimit = 15000000; }
			else if (g_Settings.m_authLevel = 2) { f_stealthlimit = 10000000; }
			else { f_stealthlimit = 5000000; }
			sub->AddOption<NumberOption<int>>("Amount", nullptr, &f_stealthamount, 1, f_stealthlimit, 10, 1000, false);
			sub->AddOption<NumberOption<int>>("Delay", nullptr, &f_stealthdelay, 1, 5000, 10, 100, false);
			sub->AddOption<BoolOption<bool>>("Stealth", "Use this option at your own risk", &f_stealth, BoolDisplay::OnOff, false);
			//sub->AddOption<SubOption>("Model Changer", nullptr, SubmenuModel);
		});
#pragma endregion
#pragma region Warbrobe
		g_UiManager->AddSubmenu<RegularSubmenu>("Wardrobe Menu", SubmenuWardrobe, [](RegularSubmenu* sub) {
			sub->AddOption<RegularOption>("Rainbow Textures", "Changes the color of your outfit.", [=] {Functions::rainbowTextures(); });
			sub->AddOption<RegularOption>("Random Outfit", "Try every new and wacky combination.", [=] {Functions::randoutfit(); });
			sub->AddOption<BoolOption<bool>>("Auto Update", "Updates the selected wardrobe item.", &auto_update, BoolDisplay::OnOff, false);
			sub->AddOption<NumberOption<int>>("Face", nullptr, &face, 1, 11, 1, 1, auto_update, "[", "]", [=]{Functions::wardrobe(0, face);});
			sub->AddOption<NumberOption<int>>("Mask", nullptr, &mask, 1, 132, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(1, mask); });
			sub->AddOption<NumberOption<int>>("Head", nullptr, &hair, 1,73, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(2, hair); });
			sub->AddOption<NumberOption<int>>("Gloves", nullptr, &gloves, 1, 164, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(3, gloves); });
			sub->AddOption<NumberOption<int>>("Tops", nullptr, &tops, 1, 254, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(11, tops); });
			sub->AddOption<NumberOption<int>>("Collar", nullptr, &collar, 1, 131, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(8, collar); });
			sub->AddOption<NumberOption<int>>("Pants", nullptr, &pants, 1, 99, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(4, pants); });
			sub->AddOption<NumberOption<int>>("Accessories", nullptr, &accessories, 1, 69, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(5, accessories); });
			sub->AddOption<NumberOption<int>>("Shoes", nullptr, &shoes, 1, 79, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(6, shoes); });
			sub->AddOption<NumberOption<int>>("Mission Stuff", nullptr, &mission_stuff, 1, 29, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(9, mission_stuff); });
			sub->AddOption<NumberOption<int>>("Decals", nullptr, &decals, 1, 28, 1, 1, auto_update, "[", "]", [=] {Functions::wardrobe(10, decals); });
			});
#pragma endregion
		g_UiManager->AddSubmenu<RegularSubmenu>("Model Changer", SubmenuModel, [](RegularSubmenu* sub) {
			for (auto model : Lists::pedModels)
			{
				sub->AddOption<RegularOption>(model, "", [model] { g_FiberScript->addTask([model] {Functions::ModelChange(model); }); });
			}
			});
#pragma region Recovery Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Recovery Menu", SubmenuRecovery, [](RegularSubmenu* sub) {
			sub->AddOption<SubOption>("Unlocks", nullptr, SubmenuUnlocks);
			sub->AddOption<SubOption>("Money", nullptr, SubmenuMoney);
			sub->AddOption<SubOption>("Stat Spoofer", nullptr, SubMenuStatSpoofer);
			static int f_LevelSkipModifier{};
			sub->AddOption<NumberOption<int>>(D_TRANSLATE("Player_Level_Skip"), D_TRANSLATE("PLYRLVLSKIP_DESC"), &f_LevelSkipModifier, 1, 100, 10, 1, true, "", "", [=]
				{
					g_Settings.m_LevelSkip = f_LevelSkipModifier;
				});
			static int f_PlayerLevel{};
			sub->AddOption<NumberOption<int>>(D_TRANSLATE("Player_Level"), D_TRANSLATE("PLYRLVL_DESC"), &f_PlayerLevel, 1, 8000, g_Settings.m_LevelSkip, 1, false, "", "", [=]
				{
					Functions::setPlayerLevel(f_PlayerLevel);
				});
			sub->AddOption<RegularOption>(D_TRANSLATE("Give_Achievements"), D_TRANSLATE("ACHIEVE_DESC"), [=] {Functions::giveAchievements(); });
			sub->AddOption<RegularOption>("Clear Bad Sport", nullptr, [=] {Functions::clearBadSport(); });
		}); 
#pragma endregion
#pragma region Unlocks Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Unlocks", SubmenuUnlocks, [](RegularSubmenu* sub) {
			sub->AddOption<RegularOption>("Unlock All", nullptr, [=] {Functions::unlockAll(); });
			sub->AddOption<RegularOption>("Unlock Chrome Wheels", nullptr, [=] {Functions::unlockChrome(); });
			sub->AddOption<RegularOption>("Unlock Armor", nullptr, [=] {Functions::unlockArmor(); });
			sub->AddOption<RegularOption>("Unlock Fireworks", nullptr, [=] {Functions::unlockFireWorks(); });
			sub->AddOption<RegularOption>("Unlock Parachutes", nullptr, [=] {Functions::unlockParachutes(); });
			sub->AddOption<RegularOption>("Unlock Hair Styles", nullptr, [=] {Functions::unlockHairstyles(); });
			sub->AddOption<RegularOption>("Unlock Weapons/Skins", nullptr, [=] {Functions::unlockWeapons(); });
			sub->AddOption<RegularOption>("Unlock Clothes", nullptr, [=] {Functions::unlockClothes(); });
			sub->AddOption<RegularOption>("Max Snacks", nullptr, [=] {Functions::maxSnacks(); });
			sub->AddOption<RegularOption>("Fill Office Clutter", nullptr, [=] {Functions::officeClutter(); });
			sub->AddOption<RegularOption>(D_TRANSLATE("Max_Stats"), D_TRANSLATE("MAXSTATS_DESC"), [=] {Functions::maxStats(); });
			sub->AddOption<BoolOption<bool>>("Unlock Bunker Research", nullptr, &ResearchUnlocks, BoolDisplay::OnOff, false, [] {	globalHandle(0x40001 + 21252).As<bool>() = ResearchUnlocks; globalHandle(262145).At(21252).As<bool>() = ResearchUnlocks; });
			});
#pragma endregion
#pragma region Money Menu
	g_UiManager->AddSubmenu<RegularSubmenu>("Money Recovery", SubmenuMoney, [](RegularSubmenu* sub) {
		
	});
#pragma endregion
#pragma region Stat Spoofer Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Stat Spoofer", SubMenuStatSpoofer, [](RegularSubmenu* sub) {
			sub->AddOption<BoolOption<bool>>(D_TRANSLATE("Level_Spoofer"), "Randomizes your level server wide", &f_spoofLevel, BoolDisplay::OnOff, false, []
				{
					Functions::spoofLevel(f_spoofLevel);
				});
			sub->AddOption<BoolOption<bool>>(D_TRANSLATE("Kill_Spoofer"), "Randomizes the amount of kills you have.", &f_spoofKills, BoolDisplay::OnOff, false, []
				{
					Functions::spoofKills(f_spoofKills);
				});
			});
#pragma endregion
#pragma region Online Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Online Menu", SubmenuNetowrk, [](RegularSubmenu* sub)
		{
			g_SelectedPlayer = PLAYER::PLAYER_PED_ID();
			if (g_SelectedPlayer != -1) {
				sub->AddOption<SubOption>("All Players", nullptr, SubmenuAllPlayer);
				sub->AddOption<SubOption>("Player List", nullptr, SubmenuPlayerList);
				if (g_Settings.m_authLevel >= 2) sub->AddOption<ChooseOption<const char*, std::size_t>>("Session Weather", "This Should only be used on GTA:O", &Lists::session_weather, &Lists::session_weather_pos, true);
				sub->AddOption<RegularOption>("Set Weather","Sets the currently selected weather.", [=]{g_GameFunctions->m_SessionWeather(1, Lists::session_weather_pos, 76, 0);});
				sub->AddOption<NumberOption<int>>("Session Time","Changes the current lobby time", &f_sessionTime, 0, 23, 1, 1, false, "", "", [=]{Functions::setNetworkTime(f_sessionTime, 0, 0);});
				if (g_Settings.m_authLevel >= 2) sub->AddOption<RegularOption>("Change Name", "", [] {
					g_FiberScript->addTask([] {
						MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0)
						{
							IsKeyboardActive = true;
							g_UiManager->ResetInput();
							g_FiberScript->Wait(0);
						}
						IsKeyboardActive = false;
						if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						Functions::changeName(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
						});

					});
				if (g_Settings.m_authLevel >= 2) sub->AddOption<ChooseOption<const char*, int>>("Name Color", nullptr, &Lists::nameColor, &Lists::nameColorInt, true);
				sub->AddOption<BoolOption<bool>>("Bullshark Testosterone", nullptr, &f_bullshark, BoolDisplay::OnOff);
			}
			else {
				sub->AddOption<RegularOption>("Not Online", nullptr);
			}
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("All Players", SubmenuAllPlayer, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>(D_TRANSLATE("Non_Host_Kick"), D_TRANSLATE("NONHOSTKICK_DESC"), []
			{
				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						if (ped != PLAYER::PLAYER_PED_ID())
						{
							g_SelectedPlayer = i;
							Functions::kick_to_new_lobby();
						}
					}
				}
			});
			sub->AddOption<RegularOption>("Cage All", "Cages all the players in the session.", []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::cagePlayer(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i));
							}
						}
					}
				});
			sub->AddOption<RegularOption>("CEO Kick All", "Kills all players from their organizations.", []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::ceo_kick();
							}
						}
					}
				});
			sub->AddOption<RegularOption>("CEO Ban All", "Bans all session players from organizations.", []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::ceo_ban();
							}
						}
					}
				});
			sub->AddOption<RegularOption>("Airstrike All", nullptr, []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::airStrikePlayer(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i));
							}
						}
					}
				});
			sub->AddOption<RegularOption>("Remove Wanted All", "Removes everyones wanted status.", []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::remoteRemWanted();
							}
						}
					}
				});
			sub->AddOption<RegularOption>("10K Bounty All", nullptr, []
				{
					for (std::uint32_t i = 0; i < 32; ++i)
					{
						if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
						{
							if (ped != PLAYER::PLAYER_PED_ID())
							{
								g_SelectedPlayer = i;
								Functions::set10Bounty();
							}
						}
					}
				});
		});
#pragma endregion
		g_UiManager->AddSubmenu<RegularSubmenu>("Player List", SubmenuPlayerList, [](RegularSubmenu* sub)
			{
				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [=]
							{
								g_SelectedPlayer = i;
							});
					}
				}
			});
#pragma region Selected Player
		g_UiManager->AddSubmenu<PlayerSubmenu>(&g_SelectedPlayer, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Player Info", "Retrieve detailed info on the player.", &f_PlayerInfo, BoolDisplay::OnOff);
			sub->AddOption<SubOption>("Remote Options", nullptr, SubmenuRemoteOptions);
			sub->AddOption<SubOption>("Nice Options", nullptr, SubmenuNiceOptions);
			sub->AddOption<SubOption>("Grief Options", nullptr, SubmenuGriefOptions);
			sub->AddOption<SubOption>("Drop Options", nullptr, SubmenuDropOptions);
			if (g_Settings.m_authLevel >= 3) sub->AddOption<SubOption>("Vehicle Options", nullptr, SubmenuPVehicleOptions);
			if (g_Settings.m_authLevel >= 2) sub->AddOption<SubOption>("Attachment Options", nullptr, SubmenuAttachmentOptions);
			sub->AddOption<RegularOption>(D_TRANSLATE("Non_Host_Kick"), D_TRANSLATE("NONHOSTKICK_DESC"), [] { Functions::non_host_kick(); });
			sub->AddOption<RegularOption>(D_TRANSLATE("Copy_Outfit"), D_TRANSLATE("COPYOUTFIT_DESC"), [] { Functions::copyOutfit(g_SelectedPlayer); });
			if (g_Settings.m_authLevel >= 2) sub->AddOption<RegularOption>("Join Crew", "Joins the players selected crew", [] { Functions::joinCrew(g_SelectedPlayer); });
			if (g_Settings.m_authLevel >= 3) sub->AddOption<RegularOption>(D_TRANSLATE("Blame_Player"), D_TRANSLATE("BLAME_DESC"), [] { Functions::blame(g_SelectedPlayer); });
			sub->AddOption<BoolOption<bool>>("Spectate", "Spectates the currently selected palyer.", &f_spectate, BoolDisplay::OnOff, false, [] { Functions::spectate(f_spectate, g_SelectedPlayer); });
		});
#pragma endregion
#pragma region Remote Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Remote Options", SubmenuRemoteOptions, [](RegularSubmenu* sub)
		{
			//sub->AddOption<RegularOption>("Instant Crash", "Probably safe if you aren't near this one!", [] { Functions::instantCrash(); });
			sub->AddOption<RegularOption>("Kick To New Lobby", D_TRANSLATE("KICKTONEWLOBBY_DESC"), [] 	{ Functions::kick_to_new_lobby(); 	});
			sub->AddOption<RegularOption>("CEO Kick", D_TRANSLATE("CEOKICK_DESC"), [] {  Functions::ceo_kick(); });
			sub->AddOption<RegularOption>("CEO Ban", D_TRANSLATE("CEOBAN_DESC"), [] { Functions::ceo_ban(); });
			sub->AddOption<RegularOption>("Remove Wanted", nullptr, [] { Functions::remoteRemWanted(); });
			sub->AddOption<RegularOption>("Destroy Personal Vehicle", nullptr, [] { Functions::destroyPersonalVeh(); });
			sub->AddOption<RegularOption>("Send Transaction Failed", nullptr, [] { Functions::transFailed(); });
			sub->AddOption<RegularOption>("Set 10K Bounty", nullptr, [] { Functions::set10Bounty(); });
			sub->AddOption<RegularOption>("Invite Infinite Loading", D_TRANSLATE("INIFNITEINVITE_DESC"), [] { Functions::inv_inf_loading(); });
			sub->AddOption<RegularOption>("Gentle Kick From Vehicle", D_TRANSLATE("GENTLEKICKFROMVEH_DESC"), [] { Functions::gentle_kick_from_vehicle(); });
			if (g_Settings.m_authLevel >= 2) sub->AddOption<RegularOption>("Send To Mission", "Sends the Player to a mission", [] { Functions::send_to_mission(); });
			sub->AddOption<RegularOption>("GTA Banner Effect", "Sends the GTA Banner Effect", [] { DWORD64 args[4] = { 561831609, g_SelectedPlayer, 0, 0 }; SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 4, 1 << g_SelectedPlayer); });
			if (g_Settings.m_authLevel >= 2) sub->AddOption<ChooseOption<const char*, int>>("Location", nullptr,  &Lists::remoteTPNames, &Lists::remoteTPDestination, true);
			if (g_Settings.m_authLevel >= 2) sub->AddOption<RegularOption>("Remote TP", "Remotely teleport a player to a location.", [] { Functions::remoteTP(); });
		});
#pragma endregion
#pragma region Nice Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Nice Options", SubmenuNiceOptions, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Give All MKI Weapons", nullptr, [] {Functions::GiveallWeapons(); });
			sub->AddOption<RegularOption>("Give All MKII Weapons", nullptr, [] {Functions::GiveallMktwo(); });
			sub->AddOption<RegularOption>("Give Stun Gun", nullptr, [] {Functions::GivestunGun(); });
			sub->AddOption<RegularOption>("Give Digi Scanner", nullptr, [] {Functions::Givedigiscanner(); });
		});
#pragma endregion
#pragma region Grief Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Grief Options", SubmenuGriefOptions, [](RegularSubmenu* sub)
		{
			//sub->AddOption<BoolOption<bool>>("Hostile Peds", "Make everyone their enemy.", &f_hostilepeds, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Cage", "Traps the current player in a cage.", []{Functions::cagePlayer(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer));});
			sub->AddOption<RegularOption>("Ram Player", "Rams the player with a bigass vehicle.", [] {Functions::ramPlayer(); });
			sub->AddOption<RegularOption>("Airstrike", "Rains missles on the selected player.", [] { Functions::airStrikePlayer(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Attach to Player's Ped", "Attaches you to the players model.", [] 	{ Functions::attachToPlayer(g_SelectedPlayer); });
			sub->AddOption<RegularOption>("Detach", "Get yourself off of the selected player.", [] { Functions::detachFromPlayer(); });
			sub->AddOption<RegularOption>("Clone", "Makes copies of the player's ped and spawns them.", [] { Functions::clonePlayer(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)); });
			sub->AddOption<BoolOption<bool>>("Water Loop", "Spawns water under the player and propels them.", &f_waterloop, BoolDisplay::OnOff, false, [] { Functions::waterLoop(f_waterloop, g_SelectedPlayer); });
			sub->AddOption<BoolOption<bool>>("Fire Loop", "Spawns fire under the player and propels them.", &f_fireloop, BoolDisplay::OnOff, false, [] { Functions::fireLoop(f_fireloop, g_SelectedPlayer); });
		});
#pragma endregion
#pragma region Drop Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Drop Options", SubmenuDropOptions, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Money Drop Prop", nullptr, &Lists::money_drop, &Lists::money_drop_pos, true);
			sub->AddOption<NumberOption<int>>("Money Value", "Sets the value of the drops.", &f_dropamount, 0, 2000, 1, 1, false);
			sub->AddOption<NumberOption<int>>("Drop Delay", "Sets the delay at which drops are.", &f_delay, 0, 1000, 1, 1, false);
			sub->AddOption<BoolOption<bool>>("Money Drop", "Drops money on players.", &f_moneyDrop, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("2.5k Money Drop", "2.5k Drops money on players.", &f_newdrop, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Aid Drop", nullptr, &Lists::aid_drop, &Lists::aid_drop_pos, false, []{Functions::pickupDrops(Lists::aid_drop_pos);});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Ammo Drop", nullptr, &Lists::ammo_drop, &Lists::ammo_drop_pos, false, []{Functions::ammoDrops(Lists::ammo_drop_pos);});
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Weapon Drop", nullptr, &Lists::weapon_drops, &Lists::weapon_drop_pos, false, [] {Functions::weaponDrops(Lists::weapon_drop_pos); });
		});
#pragma endregion
#pragma region Vehicle Player Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Options", SubmenuPVehicleOptions, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Duplicate Vehicle", "Duplicate's the palyer's vehicle", [] { Functions::dupeCar(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)); });
			sub->AddOption<RegularOption>("Max Vehicle", nullptr, [=]
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
				g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]{Functions::maxVehicle(vehicle);});
			});
			sub->AddOption<RegularOption>("Repair Vehicle", nullptr, [=]
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
				g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]{Functions::repairVehicle(vehicle);});
			});
			sub->AddOption<RegularOption>("Slingshot Vehicle", nullptr, [=]
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
				g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {Functions::slingShot(vehicle); });
			});
			sub->AddOption<RegularOption>("Pop Tires", nullptr, [=]{Functions::popTires();});
			sub->AddOption<BoolOption<bool>>("Remote Horn Boost", "Allows horn boost for other players.", &f_remotehornboost, BoolDisplay::OnOff, false);
		});
#pragma endregion
#pragma region Attachment Options
		g_UiManager->AddSubmenu<RegularSubmenu>("Attachment Options", SubmenuAttachmentOptions, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, int>>("Attachment Prop", nullptr, &Lists::objects, &Lists::object_selected, true);
			sub->AddOption<RegularOption>("Attach Object", "Attach the given object to the player.", [] {  Functions::attachObjToPlayer((char*)Lists::objects[Lists::object_selected]); });
			sub->AddOption<RegularOption>("Detach Objects", "Detach all the objects from the player.", [] {  Functions::detachAll(); });
		});
#pragma endregion
#pragma region Weapon Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Menu", SubmenuWeapon, [=](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Explosive", "Explosive gun and settings.", SubmenuExplosive);
			sub->AddOption<SubOption>("Weapon Modications", nullptr, SubmenuWepmod);
			sub->AddOption<SubOption>("Impacts", nullptr, SubmenuImpact);
			sub->AddOption<RegularOption>("Give All Weapons", D_TRANSLATE("GIVEALLWEAPONS_DESC"), []{Functions::allWeapons();});
			sub->AddOption<RegularOption>("Give All MK2 Weapons", "Gives you all the mk2 weapon variants.", []{Functions::allMktwo();});
			sub->AddOption<RegularOption>("All MK2 Weapons Upgrades", "Gives you all the mk2 weapon upgrades.", [] {Functions::giveAllmkUpgrades(); });
			sub->AddOption<RegularOption>("Stun Gun", "Gives you all the stun gun from sp.", [] {Functions::stunGun(); });
			sub->AddOption<RegularOption>("Digiscanner", "Gives you the digiscanner.", [] {Functions::digiscanner(); });
			sub->AddOption<RegularOption>("Refill Ammo", D_TRANSLATE("REFILLAMMO_DESC"), []{Functions::refillAmmo();});
			sub->AddOption<BoolOption<bool>>("Rapid Fire", nullptr, &f_rapidfire, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Infinite Ammo", D_TRANSLATE("INFAMMO_DESC"), &f_InfAmmo, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("One Shot Kill", D_TRANSLATE("ONESHOT_DESC"), &f_oneShotKill, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Trigger Bot", D_TRANSLATE("TRIGGERBOT_DESC"), &f_triggerBot, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Delete Gun", "Deletes entities you shoot at", &f_deleteGun, BoolDisplay::OnOff, false);
			if (g_Settings.m_authLevel >= 2)sub->AddOption<BoolOption<bool>>("Gravity Gun", "Control a object and its position", &f_gravityGun, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Dildo Knife", "Stab them with a long hard dildo", [] { Functions::dildoKnife(); 	});
			sub->AddOption<BoolOption<bool>>("Cartoon Gun", "Get that funky and live clown jam.", &f_cartoonGun, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Ped Gun", "Shoots peds where you want!", &f_pedGun, BoolDisplay::OnOff, false);
			
		});
#pragma endregion
#pragma region Explosive Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Explosive Ammo Menu", SubmenuExplosive, [=](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Explosive Type", nullptr, &Lists::weapon_types, &Lists::weapon_types_pos, true);
			sub->AddOption<BoolOption<bool>>("Explosive Gun Active", "Destroy whatever comes your way.", &f_explosiveGun, BoolDisplay::OnOff, false);
			sub->AddOption<NumberOption<int>>("Damage Scale", "Sets the damage scale and radius.", &f_damagescale, 0, 250, 1, 1, false, "", "");
			sub->AddOption<BoolOption<bool>>("Explosion Audible", "Sets whether the explosion can be heard.", &f_audible, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Explosion Invisible", "Sets whether the explosion can be seen.", &f_invisible, BoolDisplay::OnOff, false);
		});
#pragma endregion
#pragma region Weapon modification Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Modifications", SubmenuWepmod, [=](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Rainbow Gun", "Changes your weapon tint through all tints.", &f_rainbowgun, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Weapon Tint", nullptr, &Lists::weapon_tint, &Lists::tint_id, true, [] { Functions::setWeaponTint(Lists::tint_id); });
			sub->AddOption<NumberOption<float>>("Damage", nullptr, &damage, 0.f, 1000.f, 0.1f, 1, true, "", "/1000.f", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0xB0 }, damage);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						damage = (float)Functions::getInputs();
						if (damage > 1000)
							damage = 1000;
						if (damage < 0)
							damage = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0xB0 }, damage);
						});
				}
				});
			sub->AddOption<NumberOption<float>>("Spread", nullptr, &spread, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x74 }, spread);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						spread = (float)Functions::getInputs();
						if (spread > 1000)
							spread = 1000;
						if (spread < 0)
							spread = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x74 }, spread);
						});
				}
				});
			sub->AddOption<NumberOption<float>>("Recoil", nullptr, &recoil, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x2D8 }, recoil);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						recoil = (float)Functions::getInputs();
						if (recoil > 1000)
							recoil = 1000;
						if (recoil < 0)
							recoil = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x2D8 }, recoil);
						});
				}
				});
			sub->AddOption<NumberOption<float>>("Range", nullptr, &range, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x25C }, range);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						range = (float)Functions::getInputs();
						if (range > 1000)
							range = 1000;
						if (range < 0)
							range = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x25C }, range);
						});
				}
				});
			sub->AddOption<NumberOption<float>>("Reload", nullptr, &reloadtime, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x12C }, reloadtime);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						reloadtime = (float)Functions::getInputs();
						if (reloadtime > 1000)
							reloadtime = 1000;
						if (reloadtime < 0)
							reloadtime = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0x12C }, reloadtime);
						});
				}
				});
			sub->AddOption<NumberOption<float>>("Muzzle Velocity", nullptr, &muzzlevelocity, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
				Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0xFC }, muzzlevelocity);
				if (IsKeyPressed(VK_NUMPAD5))
				{
					g_FiberScript->addTask([] {

						muzzlevelocity = (float)Functions::getInputs();
						if (muzzlevelocity > 1000)
							muzzlevelocity = 1000;
						if (muzzlevelocity < 0)
							muzzlevelocity = 0;
						Memory::set_value<float>({ 0x08, 0x10C8, 0x20, 0xFC  }, muzzlevelocity);
						});
				}
				});
		});
#pragma endregion
#pragma region Impact Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Impact Menu", SubmenuImpact, [=](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, int>>("Projectile Type", nullptr, &Lists::projectile_list, &Lists::projectile_index, true);
			sub->AddOption<BoolOption<bool>>("Projectile Gun", "Set your own custom bullets.", &f_projectilegun, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Airstrike Gun", "Shoot and make it rain hell.", &f_airstrikeGun, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Vehicle Gun", "Shoot and make it rain hell.", &f_vehShooter, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Money Prop Type", nullptr, &Lists::money_pickup, &Lists::money_pickup_pos, true);
			sub->AddOption<NumberOption<int>>("Money Value", "Sets the value of the props.", &f_price, 0, 2000, 1, 1, false, "", "");
			sub->AddOption<BoolOption<bool>>("Money Gun", "Shoot out that grand money.", &f_moneygun, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Animal Model", nullptr, &Lists::meow_type, &Lists::meow_id, true);
			sub->AddOption<BoolOption<bool>>("Animal Gun", "Shoot furry little friends.", &f_meowGun, BoolDisplay::OnOff, false);
		});
#pragma endregion
#pragma region Vehicle Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Menu", SubmenuVehicle, [=](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Spawner", nullptr, SubmenuSpawner);
			sub->AddOption<SubOption>("Mobile LSC [BETA]", nullptr, SubmenuVehicleLSC);
			if (g_Settings.m_authLevel >= 2) sub->AddOption<SubOption>("Vehicle Multipliers", nullptr, SubmenuVehicleMultipliers);
			sub->AddOption<SubOption>("Fun Options", nullptr, SubmenuFunVehicleOptions);
			sub->AddOption<BoolOption<bool>>("Vehicle God Mode", "Makes your vehicle invincible.", &f_cargod, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Max Vehicle", D_TRANSLATE("MAXVEHICLE_DESC"), [=]
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
				g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]
				{
					Functions::maxVehicle(vehicle);
				});
			});
			sub->AddOption<RegularOption>("Repair Vehicle", D_TRANSLATE("REPAIRVEHICLE_DESC"), [=] { Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false); g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]
				{
					Functions::repairVehicle(vehicle);
				}); });
			sub->AddOption<BoolOption<bool>>("Speedometer", nullptr, &f_speedometer, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Set Numplate As Speedomter", "Requires the speedometer", &f_numplatespeedometer, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, int>>("Speedometer Unit", nullptr, &Lists::speedUnit, &f_speedomterunit, false);
			sub->AddOption<BoolOption<bool>>("Vehicle Color Fade", "Fades your vehicles colors", &f_rainbowVeh, BoolDisplay::OnOff, false);
			sub->AddOption<NumberOption<int>>("Horn Boost Speed", "Sets the speed you boost at.", &f_hornspeed, 0, 200, 1, 1, false, "", "");
			sub->AddOption<BoolOption<bool>>("Horn Boost", "Press your horn button to boost through the streets.", &f_hornboost, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Engine Always On", "Keeps your engine running.", &f_enginealwayson, BoolDisplay::OnOff, false);
			//sub->AddOption<BoolOption<bool>>("Drive On Water", nullptr, &f_driveonwater, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Stick To Ground", D_TRANSLATE("STICKTOGROUND_DESC"), &f_stuckToGround, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("No Bike Fall", D_TRANSLATE("NOBIKEFALL_DESC"), &f_noBikefall, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Drift Mode", "Slip and slide in your vehicle", &f_drift, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Close Doors", "", [] { VEHICLE::SET_VEHICLE_DOORS_SHUT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true); });
		});
#pragma endregion
#pragma region Fun Vehicle Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Fun Vehicle Options", SubmenuFunVehicleOptions, [=](RegularSubmenu* sub)
		{
			if (g_Settings.m_authLevel >= 2) sub->AddOption<SubOption>("Vehicle Acrobatics", nullptr, SubmenuVehAcrobatics);
			sub->AddOption<SubOption>("Helicopter Bomber", nullptr, SubmenuVehBomber);
			sub->AddOption<ChooseOption<const char*, int>>("Driving Style", nullptr, &Lists::drivingstyle_combo, &Lists::drivingStyle, false);
			sub->AddOption<RegularOption>("Auto Pilot", "Hands free driving to your destination.", []{Functions::autoPilot(Lists::drivingstyle_types[Lists::drivingStyle]);});
			sub->AddOption<RegularOption>("Stop Task", "Maybe a pit stop is necessary.", [] {BRAIN::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID()); });
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Vehicle Weapon", nullptr, &Lists::vehicle_weapons, &Lists::vehwep_id, false);
			sub->AddOption<BoolOption<bool>>("Vehicle Weapons", nullptr, &f_vehweapons, BoolDisplay::OnOff, false);
		});
#pragma endregion
#pragma region Vehicle Acrobatics
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Acrobatics", SubmenuVehAcrobatics, [=](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Front Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 6.0f, 0, -2.0f, 0, true, true, true, true, false, true); }); 
				});
				sub->AddOption<RegularOption>("Side Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 6.0f, 5.0f, 2.0f, 0, true, true, true, true, false, true); });
					});
				sub->AddOption<RegularOption>("Back Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 6.0f, 0, 2.0f, 0, true, true, true, true, false, true); });
					});
				sub->AddOption<RegularOption>("Kick Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 5.0f, 2.0f, 0, 0, true, true, true, true, false, true); });
					});
				sub->AddOption<RegularOption>("Bunny Hop", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 7.0f, 0, 0, 0, true, true, true, true, false, true); });
					});
				sub->AddOption<RegularOption>("Double Front Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 12.0f, 0, -4.0f, 0, true, true, true, true, false, true); });
					});
				sub->AddOption<RegularOption>("Double Back Flip", nullptr, [] {
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 12.0f, 0, 4.0f, 0, true, true, true, true, false, true); });
					});
				
				
			});
#pragma endregion
#pragma region Bomber Mode
		g_UiManager->AddSubmenu<RegularSubmenu>("Bomber Mode", SubmenuVehBomber, [=](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Bomber Mode", "Allows you to drop bombs from helis.", &f_bomberMode, BoolDisplay::OnOff, false);
			sub->AddOption<ChooseOption<const char*, int>>("Explosion Type", nullptr, &Lists::weapon_types, &f_bombType, false);
			sub->AddOption<NumberOption<float>>("Radius", nullptr, &f_bomberRadius, 1, 30, 1, 3, true);
			sub->AddOption<NumberOption<int>>("Red", nullptr, &f_bombRed, 0, 255, 1, 10, true);
			sub->AddOption<NumberOption<int>>("Green", nullptr, &f_bombRed, 0, 255, 1, 10, true);
			sub->AddOption<NumberOption<int>>("Blue", nullptr, &f_bombRed, 0, 255, 1, 10, true);
		});
#pragma endregion
#pragma region Vehicle Multipliers
		static float Acceleration = { 0.f };
		static float Brake = { 0.f };
		static float Traction = { 0.f };
		static float Deform = { 0.f };
		static float Suspensions = { 0.f };
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Stats", SubmenuVehicleMultipliers, [](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<float>>("Acceleration", nullptr, &Acceleration, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
					Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x4C }, Acceleration);
					if (IsKeyPressed(VK_NUMPAD5))
					{
						g_FiberScript->addTask([] {

							Acceleration = (float)Functions::getInputs();
							if (Acceleration > 1000.f)
								Acceleration = 1000.f;
							if (Acceleration < 0.f)
								Acceleration = 0.f;
							Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x4C }, Acceleration);
							});
					}


					});
				sub->AddOption<NumberOption<float>>("Brake", nullptr, &Brake, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
					Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, Brake);
					if (IsKeyPressed(VK_NUMPAD5))
					{
						g_FiberScript->addTask([] {

							Brake = (float)Functions::getInputs();
							if (Brake > 1000.f)
								Brake = 1000.f;
							if (Brake < 0.f)
								Brake = 0.f;
							Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, Brake);
							});
					}

					});
				sub->AddOption<NumberOption<float>>("Traction", nullptr, &Traction, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
					Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, Traction);
					if (IsKeyPressed(VK_NUMPAD5))
					{
						g_FiberScript->addTask([] {

							Traction = (float)Functions::getInputs();
							if (Traction > 1000.f)
								Traction = 1000.f;
							if (Traction < 0.f)
								Traction = 0.f;
							Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, Traction);
							});
					}

					});
				sub->AddOption<NumberOption<float>>("Deform", nullptr, &Deform, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
					Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, Deform);
					if (IsKeyPressed(VK_NUMPAD5))
					{
						g_FiberScript->addTask([] {

							Deform = (float)Functions::getInputs();
							if (Deform > 1000.f)
								Deform = 1000.f;
							if (Deform < 0.f)
								Deform = 0.f;
							Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, Deform);
							});
					}

					});
				sub->AddOption<NumberOption<float>>("Suspension", nullptr, &Suspensions, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
					Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, Suspensions);
					if (IsKeyPressed(VK_NUMPAD5))
					{
						g_FiberScript->addTask([] {

							Suspensions = (float)Functions::getInputs();
							if (Suspensions > 1000.f)
								Suspensions = 1000.f;
							if (Suspensions < 0.f)
								Suspensions = 0.f;
							Memory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, Suspensions); 
							});
					}


					});
			});
#pragma endregion
#pragma region Mobile LSC
		g_UiManager->AddSubmenu<RegularSubmenu>("Mobile LSC", SubmenuVehicleLSC, [](RegularSubmenu* sub)
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				sub->AddOption<NumberOption<std::int32_t>>("Spoiler:", nullptr, &Spoiler, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SPOILER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SPOILER, Spoiler, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Front Bumper:", nullptr, &FBumper, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FRONTBUMPER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FRONTBUMPER, FBumper, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Rear Bumper:", nullptr, &RBumper, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_REARBUMPER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_REARBUMPER, RBumper, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Side Skirt:", nullptr, &SSkirt, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SIDESKIRT), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SIDESKIRT, SSkirt, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Exhaust:", nullptr, &Exhaust, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_EXHAUST), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_EXHAUST, Exhaust, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Grille:", nullptr, &Grille, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_GRILLE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_GRILLE, Grille, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Chassis:", nullptr, &Chasis, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_CHASSIS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_CHASSIS, Chasis, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Hood:", nullptr, &Hood, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HOOD), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HOOD, Hood, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Fender:", nullptr, &Fender, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FENDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FENDER, Fender, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Right Fender:", nullptr, &RFender, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_RIGHTFENDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_RIGHTFENDER, RFender, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Roof:", nullptr, &Roof, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ROOF), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ROOF, Roof, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Engine:", nullptr, &Engine, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE, Engine, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Brakes:", nullptr, &Brakes, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_BRAKES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_BRAKES, Brakes, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Transmission:", nullptr, &Transmission, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRANSMISSION), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRANSMISSION, Transmission, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Horns:", nullptr, &Horns, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HORNS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HORNS, Horns, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Suspension:", nullptr, &Suspension, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SUSPENSION), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SUSPENSION, Suspension, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Armor:", nullptr, &Armor, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARMOR), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARMOR, Armor, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Turbo:", nullptr, &Turbo, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TURBO), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TURBO, Turbo, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Tire Smoke:", nullptr, &TSmoke, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TIRESMOKE), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TIRESMOKE, TSmoke, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Xenon:", nullptr, &Xenon, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_XENONLIGHTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_XENONLIGHTS, Xenon, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Wheels:", nullptr, &Wheels, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_FRONTWHEELS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_FRONTWHEELS, Wheels, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Windows:", nullptr, &Windows, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_WINDOWS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_WINDOWS, Windows, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Plate Holder:", nullptr, &Plateholder, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_PLATEHOLDER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_PLATEHOLDER, Plateholder, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Vanity Plates:", nullptr, &Vanity, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_VANITY_PLATES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_VANITY_PLATES, Vanity, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Trim:", nullptr, &Trims, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRIM), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRIM, Trims, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Ornaments:", nullptr, &Ornaments, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ORNAMENTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ORNAMENTS, Ornaments, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Dashboard:", nullptr, &Dashboard, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DASHBOARD), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DASHBOARD, Dashboard, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Dial:", nullptr, &Dial, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DIAL), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DIAL, Dial, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Door Speaker:", nullptr, &DoorSpeaker, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_DOOR_SPEAKER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_DOOR_SPEAKER, DoorSpeaker, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Seats:", nullptr, &Seats, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SEATS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SEATS, Seats, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Steering Wheel:", nullptr, &SteeringWheel, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_STEERINGWHEEL), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_STEERINGWHEEL, SteeringWheel, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Shifting Levers:", nullptr, &Shifter, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SHIFTER_LEAVERS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SHIFTER_LEAVERS, Shifter, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Plaques:", nullptr, &Plaques, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_PLAQUES), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_PLAQUES, Plaques, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Speakers:", nullptr, &Speakers, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_SPEAKERS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_SPEAKERS, Speakers, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Trunk:", nullptr, &Trunk, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRUNK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRUNK, Trunk, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Hydraulics:", nullptr, &Hydraulics, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_HYDRAULICS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_HYDRAULICS, Hydraulics, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Engine Block:", nullptr, &EngineBlock, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ENGINE_BLOCK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ENGINE_BLOCK, EngineBlock, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Air Filter:", nullptr, &Filter, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_AIR_FILTER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_AIR_FILTER, Filter, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Struts:", nullptr, &Struts, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_STRUTS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_STRUTS, Struts, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Arch Cover:", nullptr, &Arch, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_ARCH_COVER), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_ARCH_COVER, Arch, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Aerials:", nullptr, &Aerials, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_AERIALS), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_AERIALS, Aerials, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Trim 2:", nullptr, &TrimTwo, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TRIM_2), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TRIM_2, TrimTwo, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Tank:", nullptr, &Tank, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_TANK), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_TANK, Tank, false); });
				sub->AddOption<NumberOption<std::int32_t>>("Livery:", nullptr, &Livery, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, MOD_LIVERY), 1, 3, true, "", "", [veh] { VEHICLE::SET_VEHICLE_MOD(veh, MOD_LIVERY, Livery, false); });
			});
#pragma endregion
#pragma region World Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("World Menu", SubmenuWorld, [=](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Nearby Peds", nullptr, SubmenuWorldPeds);
			sub->AddOption<SubOption>("Nearby Vehicles", nullptr, SubmenuWorldVehicles);
			sub->AddOption<BoolOption<bool>>("World Restriction Bypass", "Bypass the limits of the world!", &f_worldrestrict, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Moon Gravity", "Make everything float!", &f_moonGravity, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Chaos Mode", "Oh it's going to gte wild.", &f_chaosMode, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Slow Motion", "Makes everything move slow (Local).", &f_SlowMoBool, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Thunder/Lightning", "Makes it really scary in the air.", &f_Thunder, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Blackout", "Cuts the world power.", &f_blackout, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Heat Vision (SP)", "See like superman does.", &f_heatvision, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Night Vision (SP)", "Bravo 6 going dark.", &f_nightvision, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Set Wave Intensity", nullptr, &f_setwaveintense, BoolDisplay::OnOff, false);
			if (f_setwaveintense) { sub->AddOption<NumberOption<float>>("Wave Intensity", nullptr, &f_waveintense, 0.f, 25.f, 1.f, 5, true); }
			sub->AddOption<BoolOption<bool>>("Set Rain Intensity", nullptr, &f_setrainintense, BoolDisplay::OnOff, false);
			if (f_setrainintense) { sub->AddOption<NumberOption<float>>("Wave Intensity", nullptr, &f_rainintese, 0.f, 30.f, 1.f, 5, true); }
			sub->AddOption<BoolOption<bool>>("Set Windspeed", nullptr, &f_setwindspeed, BoolDisplay::OnOff, false);
			if (f_setwindspeed) { sub->AddOption<NumberOption<float>>("Wave Intensity", nullptr, &f_windspeed, 0.f, 20.f, 1.f, 5, true); }
			sub->AddOption<BoolOption<bool>>("Set Wind Direction", nullptr, &f_setwinddirection, BoolDisplay::OnOff, false);
			if (f_setwinddirection) { sub->AddOption<NumberOption<float>>("Wave Intensity", nullptr, &f_winddirection, 0.f, 360.f, 1.f, 10, true); }
		});
#pragma endregion
#pragma region World Ped Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Nearby Peds", SubmenuWorldPeds, [=](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Kill Nearby Peds Loop", nullptr, &f_killnearbypeds, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Kill Nearby Peds", "Kills all the peds around you.", [] {Functions::killNearbyPed(); });
			sub->AddOption<BoolOption<bool>>("Delete Nearby Peds Loop", nullptr, &f_deletenearbypeds, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Delete Nearby Peds", "Deletes all the peds around you.", [] {Functions::deleteNearbyPeds(); });
			sub->AddOption<RegularOption>("Force Aggressive Driving", "Forces peds to drive agressively.", [] {Functions::forceAgressiveDriving(); });
		});
#pragma endregion
#pragma region World Vehicles Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Nearby Vehicles", SubmenuWorldVehicles, [=](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Delete Nearby Vehicles", nullptr, [] {Functions::deleteNearbyVehicles(); });
			sub->AddOption<RegularOption>("Max Nearby Vehicles", nullptr, [] {Functions::maxNearby(); });
			sub->AddOption<RegularOption>("Kill Nearby Engines", nullptr, [] {Functions::killEngine(); });
			sub->AddOption<RegularOption>("Out of Control", nullptr, [] {Functions::outOfControl(); });
		});
#pragma endregion
#pragma region Misc Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Misc Menu", SubmenuMisc, [=](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Merry Weather Services", nullptr, SubmenuMerryWeather);
			sub->AddOption<RegularOption>("Reset Chip Cooldown ~r~[Risky]", "", [] {
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_CASINO_CHIPS_PUR_GD"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_CASINO_CHIPS_PURTIM"), 0, 1);
			});
			sub->AddOption<RegularOption>("Reset Lucky Wheel Usage ~r~[Risky]", "", [] {
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_LUCKY_WHEEL_USAGE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_LUCKY_WHEEL_USAGE"), 0, 1);
			});
			sub->AddOption<BoolOption<bool>>("Black Hole", "A Destroyer of all things good.", &f_blackhole, BoolDisplay::OnOff, false, [] { setCurrentPos(); });
			sub->AddOption<RegularOption>(D_TRANSLATE("Reset_Vehicle_Sell_Time"), D_TRANSLATE("SELLTIME_DESC"), []
		{
			Functions::resetVehicleSTime();
		});
			sub->AddOption<BoolOption<bool>>("No Orbital Canon Cooldown", "Don't wait for the canon to reset.", &f_noOrbCooldown, BoolDisplay::OnOff, false, [] { Functions::noOrbCooldown(f_noOrbCooldown); });
			sub->AddOption<BoolOption<bool>>("Free Cam", "Explore the world from another view.", &f_freeCam, BoolDisplay::OnOff, false, []
		{
			Functions::freeCam(f_freeCam);
			CAM::RENDER_SCRIPT_CAMS(false, true, 700, true, true, 1);
		});
			sub->AddOption<BoolOption<bool>>("Chat Bypass", "No need for those pesky chat censors", &f_chatrestrict, BoolDisplay::OnOff, false);
			sub->AddOption<RegularOption>("Skip Cutscenes", "Skip those boring cut scenes", [] { Functions::skipCutscenes(); });
		});
#pragma endregion
	
#pragma region Merry Weather 
	g_UiManager->AddSubmenu<RegularSubmenu>("Merry Weather Services", SubmenuMerryWeather, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>(D_TRANSLATE("Request_Ammo"), D_TRANSLATE("REQUESTAMMO_DESC"), []
		{
			Functions::merryweather("AMMO");
		});
		sub->AddOption<RegularOption>(D_TRANSLATE("Request_Boat"), D_TRANSLATE("REQUESTBOAT_DESC"), []
		{
			Functions::merryweather("BOAT");
		});
		sub->AddOption<RegularOption>(D_TRANSLATE("Request_Heli"), D_TRANSLATE("REQUESTHELI_DESC"), []
		{
			Functions::merryweather("HELI");
		});
		sub->AddOption<RegularOption>(D_TRANSLATE("Request_BackupHeli"), D_TRANSLATE("REQUESTBACKUPHELI_DESC"), []
		{
			Functions::merryweather("BACKUPHELI");
		});
		sub->AddOption<RegularOption>(D_TRANSLATE("Request_Airstrike"), D_TRANSLATE("REQUESTAIRSTRIKE_DESC"), []
		{
			Functions::merryweather("AIRSTRIKE");
		});
	});
#pragma endregion
#pragma region Spawn Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawn Menu", SubmenuSpawner, [=](RegularSubmenu* sub)
		{
				sub->AddOption<RegularOption>("Spawn By Name", "", [=] {
					g_FiberScript->addTask([=] {
						MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
						while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0){IsKeyboardActive = true;  g_UiManager->ResetInput(); g_FiberScript->Wait(0);}
						IsKeyboardActive = false;
						if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
							return 0;
						Functions::spawnVehicle(playerPed, MISC::GET_HASH_KEY(MISC::GET_ONSCREEN_KEYBOARD_RESULT()), true, true, false, false);
						});

					});
				sub->AddOption<SubOption>("Spawn Settings", nullptr, SubmenuSpawnerSettings);
				sub->AddOption<SubOption>("DLC Vehicles", nullptr, SubmenuDLCVehicles);
				sub->AddOption<ChooseOption<const char*, int>>("Super", nullptr, &Hashes::vehicle_super, &Hashes::vehicle_super_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_super_hash[Hashes::vehicle_super_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Sports", nullptr, &Hashes::vehicle_sports, &Hashes::vehicle_sports_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_sports_hash[Hashes::vehicle_sports_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Sport Classic", nullptr, &Hashes::vehicle_sports_classic, &Hashes::vehicle_sports_classic_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_sports_classic_hash[Hashes::vehicle_sports_classic_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Sedans", nullptr, &Hashes::vehicle_sedan, &Hashes::vehicle_sedan_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_sedan_hash[Hashes::vehicle_sedan_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Offroad", nullptr, &Hashes::vehicle_offroad, &Hashes::vehicle_offroad_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_offroad_hash[Hashes::vehicle_offroad_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Coupes", nullptr, &Hashes::vehicle_coupe, &Hashes::vehicle_coupe_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_offroad_hash[Hashes::vehicle_offroad_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Muscle", nullptr, &Hashes::vehicle_muscle, &Hashes::vehicle_muscle_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_muscle_hash[Hashes::vehicle_muscle_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Boats", nullptr, &Hashes::vehicle_boat, &Hashes::vehicle_boat_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_boat_hash[Hashes::vehicle_boat_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Commercial", nullptr, &Hashes::vehicle_commercial, &Hashes::vehicle_commercial_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_commercial_hash[Hashes::vehicle_commercial_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Compacts", nullptr, &Hashes::vehicle_compact, &Hashes::vehicle_compact_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_compact_hash[Hashes::vehicle_compact_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Cycles", nullptr, &Hashes::vehicle_cycle, &Hashes::vehicle_cycle_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_cycle_hash[Hashes::vehicle_cycle_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Emergency", nullptr, &Hashes::vehicle_emergency, &Hashes::vehicle_emergency_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_emergency_hash[Hashes::vehicle_emergency_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Helicopters", nullptr, &Hashes::vehicle_helicopter, &Hashes::vehicle_helicopter_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_helicopter_hash[Hashes::vehicle_helicopter_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Industrial", nullptr, &Hashes::vehicle_industrial, &Hashes::vehicle_industrial_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_industrial_hash[Hashes::vehicle_industrial_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Military", nullptr, &Hashes::vehicle_military, &Hashes::vehicle_military_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_military_hash[Hashes::vehicle_military_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Motorcycles", nullptr, &Hashes::vehicle_motorcycle, &Hashes::vehicle_motorcycle_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_motorcycle_hash[Hashes::vehicle_motorcycle_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Planes", nullptr, &Hashes::vehicle_plane, &Hashes::vehicle_plane_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_plane_hash[Hashes::vehicle_plane_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Service", nullptr, &Hashes::vehicle_service, &Hashes::vehicle_service_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_service_hash[Hashes::vehicle_service_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("SUVs", nullptr, &Hashes::vehicle_suv, &Hashes::vehicle_suv_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_suv_hash[Hashes::vehicle_suv_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Trailers", nullptr, &Hashes::vehicle_trailer, &Hashes::vehicle_trailer_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_trailer_hash[Hashes::vehicle_trailer_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Trains", nullptr, &Hashes::vehicle_train, &Hashes::vehicle_train_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_train_hash[Hashes::vehicle_train_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Utility", nullptr, &Hashes::vehicle_utility, &Hashes::vehicle_utility_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_utility_hash[Hashes::vehicle_utility_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Vans", nullptr, &Hashes::vehicle_van, &Hashes::vehicle_van_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_utility_hash[Hashes::vehicle_utility_index], PLAYER::PLAYER_PED_ID());	});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("DLC Vehicles", SubmenuDLCVehicles, [=](RegularSubmenu* sub)
			{
				sub->AddOption<ChooseOption<const char*, int>>("Casino Heist", nullptr, &Hashes::vehicle_casinoheist, &Hashes::vehicle_casinoheist_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_casinoheist_hash[Hashes::vehicle_casinoheist_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Casino", nullptr, &Hashes::vehicle_casino, &Hashes::vehicle_casino_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_casino_hash[Hashes::vehicle_casino_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Arena Wars", nullptr, &Hashes::vehicle_arenawars, &Hashes::vehicle_arenawars_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_arenawars_hash[Hashes::vehicle_arenawars_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("After Hours", nullptr, &Hashes::vehicle_afterhours, &Hashes::vehicle_afterhours_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_afterhours_hash[Hashes::vehicle_afterhours_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Southern San Andreas", nullptr, &Hashes::vehicle_ssas, &Hashes::vehicle_ssas_index, false, [] { Functions::spawn_vehicle(Hashes::vehicle_ssas_hash[Hashes::vehicle_ssas_index], PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Doomsday", nullptr, &Hashes::vehicle_doomsday, &Hashes::vehicle_doomsday_index, false, [] { uint32_t vehicle = MISC::GET_HASH_KEY(Hashes::vehicle_doomsday[Hashes::vehicle_doomsday_index]); Functions::spawn_vehicle(vehicle, PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Smugglers Run", nullptr, &Hashes::vehicle_smugglers, &Hashes::vehicle_smugglers_index, false, [] { uint32_t vehicle = MISC::GET_HASH_KEY(Hashes::vehicle_smugglers[Hashes::vehicle_smugglers_index]); Functions::spawn_vehicle(vehicle, PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Gun Running", nullptr, &Hashes::vehicle_gunrunning, &Hashes::vehicle_gunrunning_index, false, [] { uint32_t vehicle = MISC::GET_HASH_KEY(Hashes::vehicle_gunrunning[Hashes::vehicle_gunrunning_index]); Functions::spawn_vehicle(vehicle, PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Cunning Stunts", nullptr, &Hashes::vehicle_cunningstunts, &Hashes::vehicle_cunningstunts_index, false, [] { uint32_t vehicle = MISC::GET_HASH_KEY(Hashes::vehicle_cunningstunts[Hashes::vehicle_cunningstunts_index]); Functions::spawn_vehicle(vehicle, PLAYER::PLAYER_PED_ID());	});
				sub->AddOption<ChooseOption<const char*, int>>("Import/Export", nullptr, &Hashes::vehicle_importexport, &Hashes::vehicle_importexport_index, false, [] { uint32_t vehicle = MISC::GET_HASH_KEY(Hashes::vehicle_importexport[Hashes::vehicle_importexport_index]); Functions::spawn_vehicle(vehicle, PLAYER::PLAYER_PED_ID());	});
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawn Settings", SubmenuSpawnerSettings, [=](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Spawn In", "Spawns you in the vehicle", &g_Settings.spawnIn, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Spawn Max", "Spawn with all upgrades", &g_Settings.spawnMax, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Spawn God", "Spawn the entity invincible", &g_Settings.spawnGod, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Spawn with Engine On", "Spawn with the engine turned on", &g_Settings.spawnEngineOn, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Clear Area", "Clear nearby entities and objects", &g_Settings.spawnClearArea, BoolDisplay::OnOff, false);
			});
#pragma endregion
#pragma region Protections Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Protections Menu", SubmenuProtections, [=](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Game Events (Emergencies Only)", nullptr, SubmenuGameProtections);
			sub->AddOption<BoolOption<bool>>("Block All Scripted Events", "Protection from all incoming events by players.", &f_antiAll, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Notify Events", "Notify the player of incoming events.", &f_notifyEvents, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Redirect Events", "Redirects player attacks.", &f_redirectEvents, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Log Events", "Logs the events and info to logs.", &f_logEvents, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Block Money Drops", "Protection from kicks.", &f_blockdrops, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Kick", "Protection from kicks.", &f_antiKick, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti CEO Ban", "Protection from CEO Bans.", &f_antiCEOBan, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti CEO Kick", "Protection from CEO kicks.", &f_antiCEOKick, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Send To Mission", "Protection from force mission starts.", &f_antiSendToMission, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Block Passive", "Protection from blocks against passive mode.", &f_antiBlockPassive, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Notifications", "Protection from notifications.", &f_antiNotifs, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Cam Reset", "Protection from cam manipulation.", &antiCamReset, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Kick From Vehicle", "Protection from gentle kicks from vehicles.", &f_antiKickFromVeh, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Bounty", "Protection from players setting bounties on you.", &f_antibounty, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti GTA Banner", "Protection from players sending the GTA banner.", &f_antibanner, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Remote OTR", "Protection from players making you OTR.", &f_antiotr, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Remote TP", "Protection from remote teleportation.", &f_antiTPApt, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Sound Spams", "Protection from pestering noises.", &f_antiSoundspam, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Infinite Loading Screen", "Protection from the constant blackscreen.", &f_antiInfLoading, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Transaction Error", "Protection from money errors.", &f_antiTransError, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Anti Remote Remove Wanted", "Protection from remote wanted edits.", &f_antiRemoteWanted, BoolDisplay::OnOff, false);
			});
#pragma endregion
#pragma region Game Protections
		g_UiManager->AddSubmenu<RegularSubmenu>("Game Event Protections", SubmenuGameProtections, [=](RegularSubmenu* sub)
			{
				//Game Events
				sub->AddOption<BoolOption<bool>>("Anit Game EVents", D_TRANSLATE("ANTIEVENT_DESC"), &f_antiAll, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Freeze", "Makes sure you don't get frozen", &f_antiFreeze, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Pickups", "No more fake money drops", &f_antiPickups, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-PTFX", "Rid those pesky effects", &f_antiPTFX, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Stat Edits", "Protect yourself from remote stats", &f_antiStats, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Sound Manipulation", "Protection for that pesky beeping", &f_antiSound, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Control", "Only self control", &f_antiControl, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Weapon Manipulation", "Keep your weapons", &f_antiWeapon, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Weather and Time Changes", "Keep the session in sync", &f_antiWeatherTime, BoolDisplay::OnOff, false);
				sub->AddOption<BoolOption<bool>>("Anti-Explosions", "Fuck explosions", &f_antiExplosions, BoolDisplay::OnOff, false);
			});
#pragma endregion
#pragma region Menu Info
		g_UiManager->AddSubmenu<RegularSubmenu>("Menu Info", SubmenuInfo, [=](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>(std::move(RegularOption(D_TRANSLATE("Game_Version")).SetRightText(g_GameVariables->m_GameBuild)));
			sub->AddOption<RegularOption>("Mystic Website", D_TRANSLATE("SITE_DESC"), []
			{
				ShellExecuteA(NULL, "open", "https://spookimystic.com/", NULL, NULL, SW_SHOWNORMAL);
			});
		});
#pragma endregion
#pragma region Settings
		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Language", "Change your language.", SubmenuSettingsLanguage);
			sub->AddOption<SubOption>("Customize", nullptr, SubmenuCustomization);
			sub->AddOption<BoolOption<bool>>("Experimental Features", "Enables Experimental and Risky Options", &g_Settings.m_ExperimentalFeatures, BoolDisplay::OnOff, false);
			sub->AddOption<BoolOption<bool>>("Toggle Friends", "Should we execute scripts on friends?", &g_Settings.m_TargetFriendly, BoolDisplay::OnOff, false);

			sub->AddOption<NumberOption<int>>("Character Slot", nullptr, &g_Settings.m_SelectedCharacter, 1, 2, 1, 1, true, "", "", []
			{
				if (g_Settings.m_SelectedCharacter == 1)
				{
					g_Settings.m_CharSlot = "MP0";
				}
				else
				{
					g_Settings.m_CharSlot = "MP1";
				}
			});
			
			sub->AddOption<NumberOption<float>>("Menu X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Menu Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<BoolOption<std::atomic_bool>>("Log Window", nullptr, &g_Settings.m_LogWindow, BoolDisplay::OnOff);
			
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Language", SubmenuSettingsLanguage, [](RegularSubmenu* sub)
		{
			namespace fs = std::filesystem;
			fs::directory_iterator dirIt{ g_TranslationManager->GetTranslationDirectory() };
			for (auto&& dirEntry : dirIt)
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (path.has_filename() && !fs::is_empty(path))
					{
						sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
						{
							g_TranslationManager->LoadTranslations(path.stem().u8string().c_str());
						});
					}
					else
					{
						sub->AddOption<RegularOption>("No Translations Found", "Go to our website and get some!");
					}
				}
			}

		});

		

#pragma endregion
#pragma region Customization Menu
		g_UiManager->AddSubmenu<RegularSubmenu>("Customize", SubmenuCustomization, [](RegularSubmenu* sub)
		{
			sub->AddOption<SubOption>("Header", nullptr, SubmenuSettingsHeader);
			sub->AddOption<SubOption>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
			sub->AddOption<SubOption>("Options", nullptr, SubmenuSettingsOption);
			sub->AddOption<SubOption>("Footer", nullptr, SubmenuSettingsFooter);
			sub->AddOption<SubOption>("Description", nullptr, SubmenuSettingsDescription);
			sub->AddOption<SubOption>("Input", nullptr, SubmenuSettingsInput);
			sub->AddOption<NumberOption<float>>("Width", nullptr, &g_UiManager->m_Width, 0.01f, 1.f, 0.01f, 2);
			sub->AddOption<BoolOption<bool>>("Sounds", nullptr, &g_UiManager->m_Sounds, BoolDisplay::OnOff);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header", SubmenuSettingsHeader, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Type", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, []
			{
				g_UiManager->m_HeaderType = Lists::HeaderTypesBackend[Lists::HeaderTypesPosition];
			});

			switch (g_UiManager->m_HeaderType)
			{
			case HeaderType::Static:
				sub->AddOption<SubOption>("Background", nullptr, SubmenuSettingsHeaderStaticBackground);
				break;
			case HeaderType::Gradient:
				sub->AddOption<SubOption>("Gradient", nullptr, SubmenuSettingsHeaderGradientBackground);
				break;
			case HeaderType::YTD:
				sub->AddOption<SubOption>("Header", nullptr, SubmenuSettingsHeaderYTDList); //SubmenuSettingsHeaderYTDBackground
				break;
			}

			sub->AddOption<SubOption>("Text", nullptr, SubmenuSettingsHeaderText);
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_HeaderHeight, 0.01f, 0.2f, 0.001f, 3);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Background", SubmenuSettingsHeaderStaticBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
		});
		g_UiManager->AddSubmenu<RegularSubmenu>("Header Properties", SubmenuSettingsHeaderYTDList, [](RegularSubmenu* sub)
			{
				namespace fs = std::filesystem;
				fs::directory_iterator dirIt{ g_HeaderManager->GetHeaderDirectory() };
				for (auto&& dirEntry : dirIt)
				{
					if (dirEntry.is_regular_file())
					{
						auto path = dirEntry.path();
						if (path.has_filename())
						{
							if (path.extension() == ".ytd")
							{
								sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
									{
										char prefix[64] = {};
										std::snprintf(prefix, sizeof(prefix) - 1, "%s", path.stem().u8string());
										g_Settings.m_Header = prefix;
									});
							}
						}
					}
				}
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Gradient", SubmenuSettingsHeaderGradientBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Transparent", nullptr, &g_UiManager->m_HeaderGradientTransparent, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Flip", nullptr, &g_UiManager->m_HeaderGradientFlip, BoolDisplay::OnOff);

			sub->AddOption<NumberOption<std::uint8_t>>("R1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("R2", nullptr, &g_UiManager->m_HeaderGradientColorRight.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G2", nullptr, &g_UiManager->m_HeaderGradientColorRight.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B2", nullptr, &g_UiManager->m_HeaderGradientColorRight.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A2", nullptr, &g_UiManager->m_HeaderGradientColorRight.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Text", SubmenuSettingsHeaderText, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_UiManager->m_HeaderText, BoolDisplay::OnOff);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_UiManager->m_HeaderTextSize, 0.1f, 2.f, 0.01f, 2);
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Infobar", SubmenuSettingsSubmenuBar, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_SubmenuBarHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_SubmenuBarTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text R", nullptr, &g_UiManager->m_SubmenuBarTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text G", nullptr, &g_UiManager->m_SubmenuBarTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text B", nullptr, &g_UiManager->m_SubmenuBarTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text A", nullptr, &g_UiManager->m_SubmenuBarTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Options", SubmenuSettingsOption, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_OptionHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_OptionTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background R", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background G", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background B", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background A", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background R", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background G", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background B", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background A", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text R", nullptr, &g_UiManager->m_OptionSelectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text G", nullptr, &g_UiManager->m_OptionSelectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text B", nullptr, &g_UiManager->m_OptionSelectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text A", nullptr, &g_UiManager->m_OptionSelectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text R", nullptr, &g_UiManager->m_OptionUnselectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text G", nullptr, &g_UiManager->m_OptionUnselectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text B", nullptr, &g_UiManager->m_OptionUnselectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text A", nullptr, &g_UiManager->m_OptionUnselectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Footer", SubmenuSettingsFooter, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_FooterHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Sprite Size", nullptr, &g_UiManager->m_FooterSpriteSize, 0.01f, 1.f, 0.001f, 3);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_FooterBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_FooterBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_FooterBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_FooterBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite R", nullptr, &g_UiManager->m_FooterSpriteColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite G", nullptr, &g_UiManager->m_FooterSpriteColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite B", nullptr, &g_UiManager->m_FooterSpriteColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite A", nullptr, &g_UiManager->m_FooterSpriteColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Description", SubmenuSettingsDescription, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Padding", "Padding before the description rect.", &g_UiManager->m_DescriptionHeightPadding, 0.01f, 1.f, 0.001f,
				3);
			sub->AddOption<NumberOption<float>>("Height", "Size of the description rect.", &g_UiManager->m_DescriptionHeight, 0.01f, 1.f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", "Size of the description text.", &g_UiManager->m_DescriptionTextSize, 0.1f, 2.f, 0.01f, 2);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Input", SubmenuSettingsInput, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::int32_t>>("Open Delay", nullptr, &g_UiManager->m_OpenDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Back Delay", nullptr, &g_UiManager->m_BackDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Enter Delay", nullptr, &g_UiManager->m_EnterDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Vertical Delay", nullptr, &g_UiManager->m_VerticalDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Horizontal Delay", nullptr, &g_UiManager->m_HorizontalDelay, 10, 1000, 10, 0);
		});

#pragma endregion
	}

	void MenuScript::InitializeVariables()
	{
		f_PlayerInfo = false;

		f_GodMode = false;
		f_NeverWanted = false;
		f_superJump = false;
		f_antiAFK = false;
		f_slideRun = false;
		f_runspeed = 1.f;
		f_tiny = false;
		f_disphone = false;
		f_invisibleplayer = false;
		f_sixstars = false;
		f_superSpeed = false;
		f_mobileRadio = false;
		f_superSwim = false;
		f_noClip = false;
		f_superMan = false;
		f_forcefield = false;
		f_norag = false;
		f_WantedLevel = 0;
		f_dragonbreath = false;
		auto_update = false;
		f_stealthdelay = 500;
		if (g_Settings.m_authLevel = 3) { f_stealthamount = 15000000; }
		else if (g_Settings.m_authLevel = 2) { f_stealthamount = 10000000; }
		else { f_stealthamount = 5000000; }

		//vehicle
		f_cargod = false;
		f_rainbowVeh = false;
		f_stuckToGround = false;
		f_noBikefall = false;
		f_drift = false;
		f_vehweapons = false;
		f_hornboost = false;
		f_hornspeed = 100;
		//bomber mode
		f_bomberMode = false;
		f_bomberRadius = 15;
		f_bombRed = 100;
		f_bombGreen = 50;
		f_bombBlue = 220;
		f_bombType = 1;
		

		f_spoofLevel = false;
		f_spoofKills = false;

		//online
		f_otr = false;
		f_bribeAuthorities = false;
		f_joinMsg = false;
		f_moneyDrop = false;
		f_waterloop = false;
		f_fireloop = false;
		f_spectate = false;
		f_dropamount = 2000;
		f_delay = 200;
		f_remotehornboost = false;
		f_remotehornspeed = 100;
		f_PedEsp = false;
		//f_hostilepeds = false;


		f_antiAll = false;
		//weapons
		f_oneShotKill = false;
		f_InfAmmo = false;
		f_triggerBot = false;
		f_deleteGun = false;
		f_pedSwapGun = false;
		f_explosiveGun = false;
		f_airstrikeGun = false;
		f_fireworkGun = false;
		f_meowGun = false;
		f_pedGun = false;
		f_cartoonGun = false;
		f_invisible = false;
		f_audible = true;
		f_aimbot = false;
		f_damagescale = 150;
		f_moneygun = false;
		f_price = 2000;
		f_valk = false;
		f_killspoofer = false;
		f_selectedPlayer = 0;
		f_rainbowgun = false;
		f_projectilegun = false;
		//Misc
		f_noOrbCooldown = false;
		f_freeCam = false;
		f_moonGravity = false;
		f_chatrestrict = true;
		f_worldrestrict = false;
		f_blackhole = false;

		//protections
		f_antiFreeze = false;
		f_antiPickups = false;
		f_antiPTFX = false;
		f_antiStats = false;
		f_antiSound = false;
		f_antiControl = false;
		f_antiWeapon = false;
		f_antiWeatherTime = false;
		f_antiExplosions = false;
		f_antiKick = true;

		f_antiBlockPassive = true;
		f_antiKickFromVeh = true;
		f_antiNotifs = true;
		f_antiSendToMission = true;
		f_antiCEOKick = true;
		f_antiCEOBan = true;

		f_sessionTime = 0;
		f_fps = true;
	}

	void MenuScript::Destroy()
	{
		g_UiManager.reset();
	}

	void MenuScript::Tick()
	{
		g_UiManager->OnTick();
		if(!g_Settings.USerAuthType){
			//PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			//ShowCursor(true);
			//SetCursor(LoadCursorA(NULL, (LPCSTR)IDC_ARROW));

		}
		//self
		Functions::setGodMode(f_GodMode);
		Functions::setSuperJump(f_superJump);
		Functions::setNeverWanted(f_NeverWanted);
		Functions::antiAFK(f_antiAFK);
		Functions::slideRun(f_slideRun);
		Functions::tiny(f_tiny);
		Functions::sixstar(f_sixstars);
		Functions::disphone(f_disphone);
		Functions::invisible(f_invisibleplayer);
		Functions::mobileRadio(f_mobileRadio);
		Functions::setSuperSpeed(f_superSpeed, f_runspeed);
		Functions::setSuperSwim(f_superSwim);
		Functions::superMan(f_superMan);
		Functions::forcefield(f_forcefield);
		Functions::noClip(f_noClip);
		Functions::ragdoll(f_norag);
		Functions::dragonBreath(f_dragonbreath);
		Functions::slowMotion(f_SlowMoBool);
		
		Functions::antiAll(f_antiAll);
		Functions::spoofLevel(f_spoofLevel);
		Functions::spoofKills(f_spoofKills);
		Functions::stealthmoney(f_stealth, f_stealthamount, f_stealthdelay);
		//vehicles
		Functions::carGod(f_cargod);
		Functions::rainbowColor(f_rainbowVeh);
		Functions::stickToGround(f_stuckToGround);
		Functions::noBikeFall(f_noBikefall);
		Functions::drift(f_drift);
		Functions::personalhornBoost(f_hornboost, f_hornspeed);
		Functions::engineAlwaysOn(f_enginealwayson, PLAYER::PLAYER_PED_ID());
		Functions::bomberMode(f_bomberMode, f_bombType, f_bombRed, f_bombGreen, f_bombBlue, f_bomberRadius);
		Functions::vehicleWeapons(f_vehweapons, PLAYER::PLAYER_PED_ID(), Lists::vehwep_id);
		Functions::speedometer(Lists::speedUnit[f_speedomterunit], f_speedometer, f_numplatespeedometer);
		g_FiberScript->addTask([=] {Functions::driveonwater(f_driveonwater); });
		
		//online
		Functions::joinMsg(f_joinMsg);

		//Functions::whoIsAnimed(f_whoaimed);
		Functions::moneyDrop(f_moneyDrop, f_dropamount, Lists::money_drop_pos, f_delay, f_dropheight);
		Functions::dropCashv2(f_newdrop, f_delay, f_dropheight);
		Functions::waterLoop(f_waterloop, g_SelectedPlayer);
		Functions::fireLoop(f_fireloop, g_SelectedPlayer);
		Functions::spectate(f_spectate, g_SelectedPlayer);
		Functions::bullSharkTestosterone(f_bullshark);
		Functions::hornBoost(f_remotehornboost, g_SelectedPlayer, f_remotehornspeed);
		//Functions::hostilePeds(f_hostilepeds, g_SelectedPlayer);
		//weapons
		Functions::triggerBot(f_triggerBot);
		Functions::deleteGun(f_deleteGun);
		Functions::pedSwapGun(f_pedSwapGun);
		Functions::explosiveGun(f_explosiveGun, Lists::weapon_types_pos, f_damagescale, f_audible, f_invisible);
		Functions::airstrikeGun(f_airstrikeGun);
		Functions::meowGun(f_meowGun, Lists::meow_id);
		Functions::pedGun(f_pedGun);
		Functions::cartoonGun(f_cartoonGun);
		Functions::moneygun(f_moneygun, Lists::money_pickup_pos, f_price);
		Functions::aimbot(f_aimbot);
		Functions::projectileEditor(f_projectilegun, Lists::projectile_hash[Lists::projectile_index]);
		//Functions::valkyrieRocket(f_valk);
		Functions::killSpoofer(f_killspoofer, f_selectedPlayer);
		Functions::rainbowGun(f_rainbowgun);
		Functions::shootVehicles(f_vehShooter);
		Functions::gravityGun(f_gravityGun);
		Functions::setInfAmmo(f_InfAmmo);
		Functions::rapidFire(f_rapidfire);

		//Misc
		Functions::noOrbCooldown(f_noOrbCooldown);
		Functions::freeCam(f_freeCam);
		Functions::moonGravity(f_moonGravity);
		Functions::bypassChat(f_chatrestrict);
		Functions::restricted(f_worldrestrict);
		BeginHole(f_blackhole); 
		//world Options
		Functions::Chaos(f_chaosMode);
		if (f_Thunder) {
			MISC::_CREATE_LIGHTNING_THUNDER();
		}
		GRAPHICS::_SET_BLACKOUT(f_blackout);
		GRAPHICS::SET_NIGHTVISION(f_nightvision);
		GRAPHICS::SET_SEETHROUGH(f_heatvision);
		if (f_setwaveintense) {
			WATER::_SET_WAVES_INTENSITY(f_waveintense);
		} else WATER::_SET_WAVES_INTENSITY(1.f);
		if (f_setrainintense) {
			MISC::_SET_RAIN_FX_INTENSITY(f_rainintese);
		} 
		if (f_setwindspeed) {
			MISC::SET_WIND_SPEED(f_windspeed);
		} else MISC::SET_WIND_SPEED(1.f);
		if (f_setwinddirection) {
			MISC::SET_WIND_DIRECTION(f_winddirection);
		}
		Functions::killNearbyLoop(f_killnearbypeds);
		Functions::deleteNearbyPedsLoop(f_deletenearbypeds);
		//protections
		Functions::antiPtfx(f_antiPTFX);
		Functions::antiExplosion(f_antiExplosions);
		Functions::antiweatherTime(f_antiWeatherTime);
		Functions::antiPickup(f_antiPickups);
		Functions::antiControl(f_antiControl);
		Functions::antiWeapon(f_antiWeapon);
		Functions::antiFreeze(f_antiFreeze);
		Functions::antiSound(f_antiSound);
		Functions::antiStat(f_antiStats);
		Functions::antiKick(f_antiKick);
		Functions::antiCEOBan(f_antiCEOBan);
		Functions::antiCEOKick(f_antiCEOKick);
		Functions::antiSendMission(f_antiSendToMission);
		Functions::antiNotifications(f_antiNotifs);
		Functions::antiKickFromVeh(f_antiKickFromVeh);
		Functions::antiBlockPassive(f_antiBlockPassive);
		
		Functions::FPS(f_fps);

		if (f_PlayerInfo && PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) != NULL)
			g_UiManager->DrawPlayerInfo(g_SelectedPlayer);
	}
}

#pragma warning (pop)
