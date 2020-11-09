#include "Common.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_render.h"
#include "spotifyaction.hpp"
#define PROP_MONEY_BAG_01 0x113FD533
#define PICKUP_MONEY_CASE 0xCE6FDD6B


namespace Mystic::Functions {
	//Global Variable Initialization
	Vehicle lastVeh{};
	std::vector<Vehicle> spawned = { NULL };
	std::vector<Hash> spawnedCar = { NULL };
	std::vector<Entity>closestPeds = { NULL };
	std::vector<Entity>closestVehicles = { NULL };
	std::int32_t m_BackDelay = 300;


	void setFunctionLoop(Ped ped, Player player, Vehicle vehicle, NativeVector3 coords)
	{
		for (int i = 0; i < 32; i++)
		{
			if (auto playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i); ENTITY::DOES_ENTITY_EXIST(playerPed))
			{
				setPlayerLoop(playerPed, i);
			}
		}
	}

	void setPlayerLoop(Ped ped, int index)
	{
	}

	//Volatile - meaning it's useless and detected. Don't use it's just here to show their initial stealth.
	bool netShopBeginService(Any* transactionId, Any a2, int hash, Any a4, int amount, int mode)
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return false;

		return g_GameFunctions->m_NetShopBeginService(*reinterpret_cast<std::uint64_t*>(g_GameVariables->m_TransactionProcessor), transactionId, -1135378931, a2, hash, a4, amount, mode);
	}

	bool netShopCheckoutStart(Any transactionId)
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return false;

		bool ret{};

		if (transactionId)
			ret = g_GameFunctions->m_NetShopCheckoutStart(*reinterpret_cast<std::uint64_t*>(g_GameVariables->m_TransactionProcessor), transactionId, 0);

		return ret;
	}

	void setStealthMoney(int amount, bool isBank)
	{
		bool is15Mil = amount > 10000000;
		int transactionHash = is15Mil ? -1586170317 : 312105838;
		int isBanked = isBank ? 4 : 1;
		Any transactionID = INT_MAX;

		if (netShopBeginService(&transactionID, (int64_t)-1375457012, transactionHash, 1445302971, amount, isBanked))
		{
			netShopCheckoutStart(transactionID);
		}
	}
	void resetStealth() {
		globalHandle(4262927).As<int>() = 0;
		globalHandle(4262927).At(0, 7).As<int>() = 2147483647;
		globalHandle(4262927).At(0, 7).At(4).As<int>() = NETWORK::GET_NETWORK_TIME();
	}

	Ped* nearbyPeds(Player player) {
		const int numElements = 10;
		const int arrSize = numElements * 2 + 2;
		Ped ped[arrSize];
		Ped output[arrSize];
		ped[0] = numElements;
		int count = PED::GET_PED_NEARBY_PEDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), ped, 1);
		if (ped != NULL)
		{
			for (int i = 1; i <= count; i++)
			{
				int offsettedID = i;
				if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
				{
					g_CallbackScript->AddCallback<ModelCallback>((ped[offsettedID]), [=] {ped[offsettedID]; });
					NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(ped[offsettedID], true);
					output[offsettedID] = ped[offsettedID];
				}
			}
		}
		return output;
	}
#include <stdlib.h>
	int getInputs()
	{
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0)
		{
			g_MenuScript->IsKeyboardActive = true;
			g_UiManager->ResetInput();
			g_FiberScript->Wait(0);
		}
		g_MenuScript->IsKeyboardActive = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			return 0;
		return atoi(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}
	std::string DocumentsPaths()
	{
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			return AddStrings(str, (char*)"\\Mystic");
		}
		return "";
	}
	char str[100];
	char* AddStrings(char* string1, char* string2)
	{
		memset(str, 0, sizeof(str));
		strcpy_s(str, "");
		strcpy_s(str, string1);
		strcat_s(str, string2);
		return str;
	}
	//Player Information
	BOOL IsPlayerFriend(Player player)
	{
		int handle[76];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
		return NETWORK::NETWORK_IS_FRIEND(&handle[0]);
	}
	std::string getRole(int player)
	{
		if (IsPlayerFriend(player))
			return "~b~Friend";
		else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player) == PLAYER::PLAYER_PED_ID())
			return "~o~Me";
		else if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0) == PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player))
			return "~c~Host";
		else
			return "~y~Client";
	}

	int getPlayerLevel(Player p)
	{
		int level = globalHandle(1590446).At(p, 871).At(211).At(6).As<int>();
		return level;
	}

	int getPlayerHealth(Ped ped)
	{
		int health = (int)ENTITY::GET_ENTITY_HEALTH(ped);
		int maxHealth = (int)ENTITY::GET_ENTITY_MAX_HEALTH(ped);
		int percentage = health * 100 / maxHealth;

		return percentage;
	}

	int getPlayerArmor(Ped ped, Player p)
	{
		int armor = (int)PED::GET_PED_ARMOUR(ped);
		int maxArmor = (int)PLAYER::GET_PLAYER_MAX_ARMOUR(p);
		int percentage = armor * 100 / maxArmor;

		return percentage;
	}

	const char* getPlayerVehicle(Ped ped)
	{
		bool inVehicle = PED::IS_PED_IN_ANY_VEHICLE(ped, 0);

		if (inVehicle)
		{
			Hash vehicleHash = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
			const char* vehicleModel = HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicleHash));

			return vehicleModel;
		}
		else {
			char result[16];
			sprintf_s(result, "%s", "Not in Vehicle!");
			return result;
		}

	}

	char* getPlayerHeading(Player p)
	{
		// Potentially Fixed (Requires Testing).
		char Buffer[50];
		float heading = (float)ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(p));
		sprintf_s(Buffer, "%.1f", heading);

		return Buffer;
	}

	std::string getPlayerWeapon(Ped ped)
	{
		Hash weaponHash;

		if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponHash, 1))
		{
			char weapon[100];
			sprintf_s(weapon, "%d", weaponHash);

			//g_Logger->Info(weapon);

			std::string weaponName = _strdup(Hashes::Weapons[weaponHash].c_str());
			return weaponName;
		}
		else {
			return "No Weapon";
		}
	}

	int getPlayerSpeed(Ped ped)
	{
		int speed = (int)round(ENTITY::GET_ENTITY_SPEED(ped) * 2.24);
		return speed;
	}

	int getPlayerAmmo(Ped ped, Hash WeaponHash)
	{
		int Ammo = (int)WEAPON::GET_AMMO_IN_PED_WEAPON(ped, WeaponHash);

		return Ammo;
	}


	char* getPlayerKDRatio(Player p)
	{
		char Buffer[10];
		float KD = globalHandle(1590446).At(p, 818).At(211).At(26).As<float>();
		sprintf_s(Buffer, "%.2f", KD);

		return Buffer;
	}

	char* getPlayerWantedLevel(Player p)
	{
		char buffer[20];
		int Stars = (int)PLAYER::GET_PLAYER_WANTED_LEVEL(p);

		sprintf_s(buffer, "%i / 5", Stars);


		return buffer;
	}


	//Helpers
	float degToRad(float degs)
	{
		return degs * 3.141592653589793f / 180.f;
	}

	
	void updateNearbyArray()
	{
		closestPeds.clear();
		closestVehicles.clear();
		Ped me = PLAYER::PLAYER_PED_ID();

		INT i, offsettedID, count = 100;

		Ped* peds = new Ped[count * 2 + 2];
		peds[0] = count;
		INT found = PED::GET_PED_NEARBY_PEDS(me, peds, -1);
		for (i = 0; i < found; i++)
		{
			offsettedID = i * 2 + 2;

			if (!ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))	continue;
			closestPeds.push_back(peds[offsettedID]);

		}
		delete[] peds;
		Vehicle* vehicles = new Vehicle[count * 2 + 2];
		vehicles[0] = count;
		found = PED::GET_PED_NEARBY_VEHICLES(me, vehicles);
		for (i = 0; i < found; i++)
		{
			offsettedID = i * 2 + 2;

			if (!ENTITY::DOES_ENTITY_EXIST(vehicles[offsettedID]))	continue;

			closestVehicles.push_back(vehicles[offsettedID]);

		}
		delete[] vehicles;
	}
	void setNetworkedScript(std::function<void()> script)
	{
		scrThread* networkedThread{};
		if (g_GameVariables->m_IsSessionStarted && (networkedThread = findNetworked(RAGE_JOAAT("freemode"))) != nullptr)
		{
			auto originalThread = ActiveThread();
			setActiveThread(networkedThread);
			std::invoke(script);
			setActiveThread(originalThread);
		}
		else
		{
			std::invoke(script);
		}
	}

	NativeVector3 add(NativeVector3* vectorA, NativeVector3* vectorB) {
		NativeVector3 result;
		result.x = vectorA->x;
		result.y = vectorA->y;
		result.z = vectorA->z;
		result.x += vectorB->x;
		result.y += vectorB->y;
		result.z += vectorB->z;
		return result;
	}

	Vector2 WorldToScreen(NativeVector3 pos)
	{
		auto& io = ImGui::GetIO();
		Vector2 tempVec2;
		reinterpret_cast<bool(__fastcall*)(NativeVector3*, float*, float*)>(g_GameFunctions->m_W2S)(&pos, &tempVec2.x, &tempVec2.y);
		tempVec2.x *= io.DisplaySize.x;
		tempVec2.y *= io.DisplaySize.y;
		return tempVec2;
	}

	NativeVector3 multiply(NativeVector3* vector, float x) {
		NativeVector3 result;
		result.x = vector->x;
		result.y = vector->y;
		result.z = vector->z;
		result.x *= x;
		result.y *= x;
		result.z *= x;
		return result;
	}

	NativeVector3 rot_to_direction(NativeVector3* rot) {
		float radiansZ = rot->z * 0.0174532924f;
		float radiansX = rot->x * 0.0174532924f;
		float num = abs((float)cos((double)radiansX));
		NativeVector3 dir;
		dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
		dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
		dir.z = (float)sin((double)radiansX);
		return dir;
	}

	void notifyAboveMap(std::string msg, BOOL blink) {
		HUD::SET_TEXT_OUTLINE();
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&msg[0u]);
		HUD::_DRAW_NOTIFICATION(blink, 0);
	}
	void notifyMap(const char* fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		HUD::SET_TEXT_OUTLINE();
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		HUD::_DRAW_NOTIFICATION(FALSE, FALSE);
	}	void notifyMap(std::string str) { notifyMap(&str[0]); }
	// quick function to get - coords - of - entity:
	NativeVector3 coordsOf(Entity entity) {
		return ENTITY::GET_ENTITY_COORDS(entity, 1);
	}

	int r = 255, g = 0, b = 0;
	void RGBFade()
	{
		if (r > 0 && b == 0) {
			r--;
			g++;
		}
		if (g > 0 && r == 0) {
			g--;
			b++;
		}
		if (b > 0 && g == 0) {
			r++;
			b--;
		}
	}


	void addMoney()
	{
		Any transactionID = MISC::GET_RANDOM_INT_IN_RANGE(1000000000, 2000000000);
		if (NETSHOP::_NETWORK_SHOP_BEGIN_SERVICE(&transactionID, 1474183246, -1216489292, 1445302971, 650000, 4))
			NETSHOP::_NETWORK_SHOP_CHECKOUT_START(transactionID);
	}

	void removeMoney()
	{
		Any transactionID = MISC::GET_RANDOM_INT_IN_RANGE(1000000000, 2000000000);
		NETSHOP::_NETWORK_SHOP_BEGIN_SERVICE(&transactionID, 1474183246, 2043854386, 537254313, 600000, 4);
		NETSHOP::_NETWORK_SHOP_CHECKOUT_START(transactionID);
	}

	//self options
	void setGodMode(bool enable)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), enable);
	}

	void setNeverWanted(bool enable)
	{
		if (enable)
		{
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
			PLAYER::SET_MAX_WANTED_LEVEL(0);
		}
		else
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
			PLAYER::SET_MAX_WANTED_LEVEL(5);
		}
	}
	void setWantedLevel(int level)
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), level, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
	}
	void dragonBreath(bool enable) {
		if (enable) {
			NativeVector3 Mouth = PED::GET_PED_BONE_COORDS(PLAYER::PLAYER_PED_ID(), 39317, 0.1f, 0.0f, 0.0f);
			FIRE::ADD_EXPLOSION(Mouth.x, Mouth.y, Mouth.z, 28, 1.0f, true, false, 0.0f, true);
			FIRE::ADD_EXPLOSION(Mouth.x, Mouth.y, Mouth.z, 27, 1.0f, true, false, 0.0f, true);
		}
	}
	void setSuperJump(bool enable)
	{
		if (enable)
		{
			Player playerPed = PLAYER::PLAYER_PED_ID();
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
			MISC::SET_SUPER_JUMP_THIS_FRAME(playerPed);
		}
	}
	void ragdoll(bool enable) {
		if (enable) {
			PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), false);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), false);
			PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), false);
		}
		else {
			//PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), true);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), true);
			PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), true);
		}
	}

	void rainbowhaircolor(bool enable) {
		if (enable) {
			for (int i = 0; i < 63; i++) {
				PED::_SET_PED_HAIR_COLOR(PLAYER::PLAYER_PED_ID(), i, 0);
			}
		}
	}

	void setPedHairColor(int HairID) {
		PED::_SET_PED_HAIR_COLOR(PLAYER::PLAYER_PED_ID(), HairID, 0);
	}

	void setSuperSwim(bool enable) {
		if (enable) {
			Memory::set_value<float>({ 0x8, 0x10B8, 0x0148 }, 2.5f);
		}
		else {
			Memory::set_value<float>({ 0x8, 0x10B8, 0x0148 }, 1.0f);
		}
	}
	void mobileRadio(bool enable) {
		int station = AUDIO::GET_PLAYER_RADIO_STATION_INDEX();
		if (enable)
		{
			g_GameFunctions->m_MobileRadio(true);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(1);
			AUDIO::SET_RADIO_TO_STATION_INDEX(station);
			
		}
		else
		{
			g_GameFunctions->m_MobileRadio(false);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(0);
		}
	}
	void slowMotion(bool enable)
	{
		if (enable)
			MISC::SET_TIME_SCALE(0.2);
		else
			MISC::SET_TIME_SCALE(1);
	}
	void wardrobe(int index, int clothes)
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		PED::SET_PED_COMPONENT_VARIATION(playerPed, index, clothes, 0, -1);
	}
	void rainbowTextures()
	{
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
		Ped PlayerPed = PLAYER::PLAYER_PED_ID();
		for (int componentId = 0; componentId < 11; componentId++) {
			int drawableId = PED::GET_PED_DRAWABLE_VARIATION(PlayerPed, componentId);
			PED::GET_PED_TEXTURE_VARIATION(PlayerPed, componentId);
			int textureId = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PlayerPed, componentId, drawableId);
			PED::SET_PED_COMPONENT_VARIATION(PlayerPed, componentId, drawableId, textureId, 0);
		}
	}
	void antiAFK(bool enable)
	{
		if (enable)
		{
			globalHandle(1373500).At(1165).As<int>() = -1;
			globalHandle(1373500).At(1149).As<int>() = -1;
		}
	}
	void camZoom(float zoom)
	{
		CAM::_ANIMATE_GAMEPLAY_CAM_ZOOM(zoom, zoom);
	}
	void slideRun(bool enable)
	{
		if (enable)
		{
			if (BRAIN::IS_PED_RUNNING(PLAYER::PLAYER_PED_ID()) || BRAIN::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID()))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 1.5f, 0.f, 0.f, 0.f, 0.f, 1, true, true, true, false, true);
			}
		}
	}
	void cleanPed()
	{
		PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
		PED::RESET_PED_VISIBLE_DAMAGE(PLAYER::PLAYER_PED_ID());
	}
	void tiny(bool enable) {
		PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, enable);
	}
	void sixstar(bool enable) {
		if (enable) {
			MISC::SET_FAKE_WANTED_LEVEL(6);
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 5, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
		}
		else {
			MISC::SET_FAKE_WANTED_LEVEL(0);
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false);
		}
	}
	void disphone(bool enable) {
		if (enable)
		{
			globalHandle(19486).At(1).As<int>() = 1;
		}
	}
	void randoutfit() {
		PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), true);
	}
	void maxarmor() {
		int Ped = PLAYER::PLAYER_PED_ID();
		PED::ADD_ARMOUR_TO_PED(Ped, 100);
	}
	void maxHealth() {
		ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400);
	}
	void invisible(bool enable) {
		if (enable) {
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false, 0);
		}
		else {
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true, 0);
		}

	}
	void forcefield(bool enable)
	{
		if (enable) {
			Player playerPed = PLAYER::PLAYER_PED_ID();
			NativeVector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
			//FIRE::ADD_EXPLOSION(pCoords.x + 2, pCoords.y + 2, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
			//FIRE::ADD_EXPLOSION(pCoords.x + 3, pCoords.y + 3, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
			FIRE::ADD_EXPLOSION(pCoords.x + 4, pCoords.y + 4, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
			FIRE::ADD_EXPLOSION(pCoords.x + 5, pCoords.y + 5, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
			FIRE::ADD_EXPLOSION(pCoords.x + 6, pCoords.y + 6, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
			FIRE::ADD_EXPLOSION(pCoords.x + 7, pCoords.y + 7, pCoords.z, 7, 9.0f, false, true, 0.0f, 1);
		}
	}
	void superMan(bool enable)
	{
		if (enable) {
			if (GetAsyncKeyState(VK_SPACE))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0, 0, 10, 0, 0, 0, 1, true, true, true, true, true);
			}
			Hash hash = MISC::GET_HASH_KEY("GADGET_PARACHUTE");
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1, 1);

			if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
			{
				if (GetAsyncKeyState(0x57)) // W key
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0);
				}

				if (GetAsyncKeyState(0x53)) // S key
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 3, 6, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0);
				}
				if (GetAsyncKeyState(VK_SHIFT))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 6, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0);
				}
			}
		}
	}
	void setSuperSpeed(bool enable, float speed) {
		if (enable) {
			Memory::set_value<float>({ 0x8, 0x10B8, 0x014C }, speed);
		}
		else
		{
			Memory::set_value<float>({ 0x8, 0x10B8, 0x0148 }, 1.0f);
		}
	}
	void personalhornBoost(bool enable, int speed) {
		if (enable) {
			if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_PED_ID()) || GetAsyncKeyState(VK_KEY_E))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
				g_CallbackScript->AddCallback<NetworkControlCallback>((veh), [=] {
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
					});
			}
		}
	}
	void noClip(bool enable)
	{
		if (enable) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			NativeVector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			if (GetAsyncKeyState(VK_KEY_S) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				float xVec = fivef * sin(degToRad(heading)) * -1.0f;
				float yVec = fivef * cos(degToRad(heading));
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.x -= xVec, pos.y -= yVec;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(VK_KEY_W) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				float xVec = fivef * sin(degToRad(heading)) * -1.0f;
				float yVec = fivef * cos(degToRad(heading));
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.x += xVec, pos.y += yVec;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(VK_KEY_A) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 266)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading + 0.5f);
			}
			if (GetAsyncKeyState(VK_KEY_D) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 271)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading - 0.5f);
			}
			if (GetAsyncKeyState(VK_SHIFT)) {
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.z -= 0.5;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.z += 0.5;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
		}
	}
	void anim(int ID) {
		switch (ID) {
		case 0:return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_PAPARAZZI", 0, true); break;
		case 1: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_DRUG_DEALER_HARD", 0, true); break;
		case 2: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_AA_COFFEE", 0, true); break;
		case 3: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_MUSICIAN", 0, true); break;
		case 4: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_MUSCLE_FLEX", 0, true); break;
		case 5: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_JOG_STANDING", 0, true); break;
		case 6: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_BINOCULARS", 0, true); break;
		case 7: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_HUMAN_CLIPBOARD", 0, true); break;
		case 8: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS", 0, true); break;
		case 9: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "PROP_HUMAN_MUSCLE_CHIN_UPS", 0, true); break;
		case 10: return BRAIN::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "PROP_HUMAN_BBQ", 0, true); break;
		}
	}

	const char* walkDict(int ID) {
		switch (ID) {
		case 0: return "move_m@generic"; break;
		case 1: return "move_f@generic"; break;
		case 2: return "move_cop@action"; break;
		case 3: return "move_m@gangster@generic"; break;
		case 4: return "move_m@hipster@a"; break;
		case 5: return "move_m@hobo@a"; break;
		case 6: return "move_m@fat@a"; break;
		case 7: return "move_lester_CaneUp"; break;
		case 8: return "move_m@drunk@a"; break;
		case 9: return "move_m@drunk@moderatedrunk"; break;
		case 10: return "move_m@drunk@verydrunk"; break;
		case 11: return "move_f@sexy@a"; break;
		case 12: return "move_m@alien"; break;
		case 13: return "move_m@business@a"; break;
		case 14: return "move_m@casual@a"; break;
		case 15: return "move_m@clipboard"; break;
		case 16: return "move_m@coward"; break;
		case 17: return "move_m@fire"; break;
		case 18: return "move_m@flee@a"; break;
		case 19: return "move_m@hiking"; break;
		case 20: return "move_m@hurry@a"; break;
		case 21: return "move_injured_generic"; break;
		case 22: return "move_m@intimidation@1h"; break;
		case 23: return "move_m@muscle@a"; break;
		case 24: return "move_m@quick"; break;
		case 25: return "move_m@sad@a"; break;
		case 26: return "move_m@shadyped@a"; break;
		case 27: return "move_m@shocked@a"; break;
		case 28: return "move_f@arrogant@a"; break;
		case 29: return "move_f@handbag"; break;
		case 30: return "move_f@heels@c"; break;
		}
	}
	void walkStyle(int ID) {
		g_FiberScript->addTask([=] {
		STREAMING::REQUEST_ANIM_SET(walkDict(ID));
		while (!STREAMING::HAS_ANIM_SET_LOADED(walkDict(ID)));
		PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), walkDict(ID), 1.0f);
			});
	}
	//online options
	void setNetworkTime(int h, int m, int s)
	{
		g_FiberScript->addTask([=] {
		g_GameVariables->m_ClockTime->hour = h;
		g_GameVariables->m_ClockTime->minute = m;
		g_GameVariables->m_ClockTime->second = s;
		g_GameFunctions->m_SessionTime(4, 0);
			});
	}

	void otr(bool enable)
	{
		if (enable)
		{
			globalHandle(2424073 + 1 + PLAYER::PLAYER_ID() * 421 + 200).As<bool>() = true;
			globalHandle(2437549 + 70).As<int>() = NETWORK::GET_NETWORK_TIME() + 120000;
		}
	}

	void bribeAuthorities(bool enable)
	{
		if (enable)
		{
			globalHandle(2528542).At(6099).As<int>() = 5;
			globalHandle(2528542).At(6100).As<int>() = 1;
			globalHandle(2528542).At(6102).As<int>() = NETWORK::GET_NETWORK_TIME() + 720000;
		}
	}

	void joinMsg(bool enable)
	{
		if (enable)
		{
			for (int i = 0; i < 33; i++) {
				if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))) {
					notifyMap("%s is joining", g_GameFunctions->m_GetPlayerName(i));
				}
			}
		}

	}

	void kick_to_new_lobby()
	{
		Hash kickHashes[] = { -1246838892, 1667907776, 297770348, 498709856, -1539546643, 0xE051FFE5, -333038531, -1153009121, -867805069, 1046017888, -1998642710,
			106447194, 519946261, 163985506, 873197495, -906869562, 741569622, -1434746202, 797746128, 1658079306, 290191363,
			615831251, -1926987112, 1404046700, -612288019, -1441300790, -1290756689, -370669082, 1780075204, -632266118,
			426145626, -814250534, -1010395481, 925877602, -1179957982, 900146065, 498709856, -1297785021, 143196100, -1818550779,
			2037380969, 1070706073, -1933245257, 380850754, -1924332863, 709335341, -1704719905, 709335341, 1152017566, 930233414,
			297770348, 498709856, -452918768, -1246838892, -332594529, -123645928, 1209128713, 1800294560, 1476326089, -771910813,
			-803645682, 1062544784, -531496987, 400031869, -1424895288, 20218675039, -498955166, -815817885, 1953937033, -1941292498,
			-941739545, 149365611, 693546501, -1462751325

		};
		for (int i = 0; i < 74; i++) {
			uint64_t kick[4] = { kickHashes[i],  g_SelectedPlayer, 0, 0 }; 
			g_GameFunctions->m_TriggerScriptEvent(1, kick, 4, 1 << g_SelectedPlayer);
		}
	}
	void destroyPersonalVeh()
	{
		DWORD64 args[3] = { DESTROY_PERSONAL_VEHICLE, g_SelectedPlayer, g_SelectedPlayer };
		SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 3, 1 << g_SelectedPlayer);
	}
	void remoteRemWanted()
	{
		DWORD64 args[3] = { REMOVE_WANTED_LEVEL, g_SelectedPlayer, Utils::GlobForNeverWanted(g_SelectedPlayer) };
		SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 3, 1 << g_SelectedPlayer);
	}
	int GlobalFunc9() {
		return globalHandle(1651198 + 9).As<int>();
	}

	int GlobalFunc10() {
		return globalHandle(1651198 + 10).As<int>();
	}
	void transFailed()
	{
		DWORD64 args[8] = { TRANSACTION_FAIL, g_SelectedPlayer, 10000, 0, 0, Utils::GlobForNeverWanted(g_SelectedPlayer), GlobalFunc9(), GlobalFunc10() };
		SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 8, 1 << g_SelectedPlayer);
	}
	void set10Bounty()
	{
		for (int i = 0; i < 32; i++) {
			DWORD64 args[22] = { SET_BOUNTY, i, g_SelectedPlayer, 0, 10000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, GlobalFunc9(), GlobalFunc10() };
			SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 22, 1 << i);
		}
	}
	void ceo_kick()
	{
		uint64_t ceokick[4] = { -701823896, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0, 0 };
		g_GameFunctions->m_TriggerScriptEvent(1, ceokick, 4, 1 << g_SelectedPlayer);
	}
	void setName(std::string& name)
	{
		Utils::allocateNameAddresses();
		size_t newLen = name.size() + 1;
		if (newLen <= 1 || newLen > 20)
			return;

		char	buffer[0x20] = {};
		char* pSearch = PLAYER::GET_PLAYERS_NAME(PLAYER::PLAYER_ID()) + 0x5C;
		size_t	curLen = strlen(pSearch) + 1;
		strncpy_s(buffer, pSearch, curLen);

		for (uint32_t i = 0, found = 0, match = 0; found < 4; ++pSearch)
		{
			if (*pSearch != buffer[i])
				goto LABEL_RESET;

			if (++match < curLen)
			{
				++i;
				continue;
			}

			strncpy_s(pSearch - i, newLen, &name[0], newLen);
			++found;

		LABEL_RESET:
			i = match = 0;
		}
		int VectorSize = Utils::getNameAddresses().size();
		int i = 0;
		for (DWORD64 Address : Utils::getNameAddresses()) {
			i++;
			strncpy((char*)Address, &name[0], newLen);
		}
	}
	void changeName(std::string name)
	{
		std::string ColorString;

		ColorString = Lists::nameColorSets[Lists::nameColorInt];
		std::string str = ColorString + name;
		if (str != "!!INVALID!!" && str != "")
			setName(str);
		Utils::notifyMap("Changing name to: %s", name);
	}
	void ceo_ban()
	{
		uint64_t ban_ceo[4] = { -327286343, g_SelectedPlayer, 1, 5 };
		g_GameFunctions->m_TriggerScriptEvent(1, ban_ceo, 4, 1 << g_SelectedPlayer);
	}

	void non_host_kick()
	{
		uint64_t non_kick[3] = { -1246838892, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0 }; //OLD: 585981075; player; 0; 0x35
		g_GameFunctions->m_TriggerScriptEvent(1, non_kick, 3, 1 << g_SelectedPlayer);
	}

	void inv_inf_loading()
	{
		uint64_t inv_inf_loading[9] = { -125347541, g_SelectedPlayer, 0, -1, 1, 1, 0, 0, 0 };
		g_GameFunctions->m_TriggerScriptEvent(1, inv_inf_loading, 9, 1 << g_SelectedPlayer);
	}

	void gentle_kick_from_vehicle()
	{
		uint64_t args[2] = { -1089379066, g_SelectedPlayer };
		g_GameFunctions->m_TriggerScriptEvent(1, args, 2, 1 << g_SelectedPlayer);
	}

	void blame(Player player)
	{
		Player victim = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		g_GameVariables->OwnedExplosionBypass(true);
		for (int i = 0; i < 33; i++) {
			Player targetPlayers = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
			NativeVector3 playerCoords = coordsOf(targetPlayers);
			if (targetPlayers != victim) {
				g_GameFunctions->m_AddOwnedExplosion(victim, playerCoords, EXP_TAG_GRENADE, 5.0f, true, false, 0.1f);
			}
		}
		g_GameVariables->OwnedExplosionBypass(false);
	}
	void ramPlayer()
	{
		Hash model = MISC::GET_HASH_KEY("INSURGENT");

		g_CallbackScript->AddCallback<ModelCallback>(model, [model] {
			float offset;
			STREAMING::REQUEST_MODEL(model);
			NativeVector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0.0, -10.0, 0.0);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				NativeVector3 dim1, dim2;
				MISC::GET_MODEL_DIMENSIONS(model, &dim1, &dim2);

				offset = dim2.y * 1.6;

				NativeVector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer));
				float rot = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0)).z;
				pCoords.x + (dir.x * offset);
				pCoords.y + (dir.y * offset);
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, pCoords.x, pCoords.y, pCoords.z, rot, true, false);

				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
				ENTITY::SET_ENTITY_VISIBLE(veh, true, 0);
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 1000.0);
			}
			});
	}
	void copyOutfit(Player player)
	{
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), i));
			for (int i = 0; i < 2; i++) {
				g_FiberScript->Wait(0);
			}
		}
	}
	Object* Crashobjects;
	void instantCrash() {
		
		Player toCrash = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 playerPosition = coordsOf(toCrash);
		NativeVector3 safeCoords = { 1741.4960f, 3269.2570f, 41.6014f };
		Hash modelHashes[32] = { 0x9CF21E0F, 0x4F2526DA,
			0x34315488, 0x6A27FEB1, 0xCB2ACC8, 0xC6899CDE, 0xD14B5BA3,
			0xD9F4474C, 0x32A9996C, 0x69D4F974, 0xCAFC1EC3,0x79B41171, 
			0x1075651, 0xC07792D4, 0x781E451D,0x762657C6, 0xC2E75A21,
			0xC3C00861, 0x81FB3FF0, 0x45EF7804, 0xE65EC0E4, 0xE764D794, 
			0xFBF7D21F, 0xE1AEB708, 0xA5E3D471, 0xD971BBAE, 0xCF7A9A9D,
			0xC2CC99D8, 0x8FB233A4, 0x24E08E1F, 0x337B2B54, 0xB9402F87
		};
		tpToDestination(PLAYER::PLAYER_PED_ID(), safeCoords);
		for (int i = 0; i < 32; i++) {
			Crashobjects[i] = OBJECT::CREATE_OBJECT(modelHashes[i], playerPosition.x, playerPosition.y, playerPosition.z, 1, 1, 1);
		}
		for (int j = 0; j < 31; j++) {
			OBJECT::DELETE_OBJECT(Crashobjects);
		}
	}
	void attachObjToPlayer(char* object)
	{

		Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 pos = { 0.f, -.26f, .1f };
		NativeVector3 rot = { 0.f, 0.f, 0.f };
		Hash hash = MISC::GET_HASH_KEY(object);
		const auto ped_position = ENTITY::GET_ENTITY_COORDS(t, true);
		NativeVector3 empty;
		auto objectToAttach = OBJECT::CREATE_OBJECT(hash, ped_position.x, ped_position.y, ped_position.z, true, false, false);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(objectToAttach, t, SKEL_Spine0, 0, 0, 0, 0, 0, 0, 1, 1, false, true, 2, 1);
		return;
	}
	void detachAll()
	{
		for (int i = 0; i < 5; i++) {
			NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
			MISC::CLEAR_AREA_OF_PEDS(coords.x, coords.y, coords.z, 2, 0);
			MISC::CLEAR_AREA_OF_OBJECTS(coords.x, coords.y, coords.z, 2, 0);
			MISC::CLEAR_AREA_OF_VEHICLES(coords.x, coords.y, coords.z, 2, 0, 0, 0, 0, 0, 0);
			for (int i = 0; i < 136; i++) {
				Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(coords.x, coords.y, coords.z, 4.0, MISC::GET_HASH_KEY(Lists::objects[i]), 0, 0, 1);

				if (ENTITY::DOES_ENTITY_EXIST(obj) && ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(obj, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer))) {
					Utils::requestControlEnt(obj);
					int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(obj);
					NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
					Utils::requestControlId(netID);
					ENTITY::DETACH_ENTITY(obj, 1, 1);
				}
			}
		}
	}
	void send_to_mission()
	{
		uint64_t mission[2] = { -348418057, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
		g_GameFunctions->m_TriggerScriptEvent(1, mission, 2, 1 << g_SelectedPlayer);
	}
	void remoteTP()
	{
		DWORD64 args[9] = { -125347541, g_SelectedPlayer, 1, -1, 1, Lists::remoteTPID[Lists::remoteTPDestination], 0,0,0 }; //Arcadius Office
		SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 9, 1 << g_SelectedPlayer);
	}
	void soundSpam(bool enable, int player, int sound)
	{
		if (enable)
		{

			uint64_t sound_spam[3] = { 552065831, player, sound };
			g_GameFunctions->m_TriggerScriptEvent(1, sound_spam, 3, 1 << player);

		}

	}
	void dupeCar(int player)
	{
		Vehicle pedVeh = NULL;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(player, FALSE))
		{
			pedVeh = PED::GET_VEHICLE_PED_IS_IN(player, FALSE);
			if (ENTITY::DOES_ENTITY_EXIST(pedVeh))
			{
				Hash vehicleModelHash = ENTITY::GET_ENTITY_MODEL(pedVeh);
				g_CallbackScript->AddCallback<ModelCallback>(vehicleModelHash, [vehicleModelHash, playerPed, pedVeh] {
					setNetworkedScript([=] {
						NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);
						Vehicle playerVeh = VEHICLE::CREATE_VEHICLE(vehicleModelHash, playerPosition.x, playerPosition.y, playerPosition.z, ENTITY::GET_ENTITY_HEADING(playerPed), true, false);
						PED::SET_PED_INTO_VEHICLE(playerPed, playerVeh, -1);
						int primaryColor, secondaryColor;
						VEHICLE::GET_VEHICLE_COLOURS(pedVeh, &primaryColor, &secondaryColor);
						VEHICLE::SET_VEHICLE_COLOURS(playerVeh, primaryColor, secondaryColor);
						if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(pedVeh))
						{
							int r, g, b;
							VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(pedVeh, &r, &g, &b);
							VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(playerVeh, r, g, b);
						}
						if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(pedVeh))
						{
							int r, g, b;
							VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(pedVeh, &r, &g, &b);
							VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(playerVeh, r, g, b);
						}
						if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModelHash) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicleModelHash))
						{
							VEHICLE::SET_VEHICLE_MOD_KIT(playerVeh, 0);
							VEHICLE::SET_VEHICLE_WHEEL_TYPE(playerVeh, VEHICLE::GET_VEHICLE_WHEEL_TYPE(pedVeh));
							for (int i = 0; i <= 24; i++)
							{
								if (i > 16 && i < 23)
									VEHICLE::TOGGLE_VEHICLE_MOD(playerVeh, i, VEHICLE::IS_TOGGLE_MOD_ON(pedVeh, i));
								else
									VEHICLE::SET_VEHICLE_MOD(playerVeh, i, VEHICLE::GET_VEHICLE_MOD(pedVeh, i), VEHICLE::GET_VEHICLE_MOD_VARIATION(pedVeh, i));
							}
							int tireSmokeColor[3], pearlescentColor, wheelColor;
							VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(pedVeh, &tireSmokeColor[0], &tireSmokeColor[1], &tireSmokeColor[2]);
							VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(playerVeh, tireSmokeColor[0], tireSmokeColor[1], tireSmokeColor[2]);
							VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(playerVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(pedVeh));
							VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(playerVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(pedVeh));
							VEHICLE::GET_VEHICLE_EXTRA_COLOURS(pedVeh, &pearlescentColor, &wheelColor);
							VEHICLE::SET_VEHICLE_EXTRA_COLOURS(playerVeh, pearlescentColor, wheelColor);
							if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(pedVeh, 0))
							{
								int convertableState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(pedVeh);
								if (convertableState == 0 || convertableState == 3 || convertableState == 5)
									VEHICLE::RAISE_CONVERTIBLE_ROOF(playerVeh, 1);
								else
									VEHICLE::LOWER_CONVERTIBLE_ROOF(playerVeh, 1);
							}
							for (int i = 0; i <= 3; i++)
							{
								VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(playerVeh, i, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(pedVeh, i));
							}
							for (int i = 0; i <= 11; i++)
							{
								if (VEHICLE::DOES_EXTRA_EXIST(pedVeh, i))
									VEHICLE::SET_VEHICLE_EXTRA(playerVeh, i, !VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(pedVeh, i));
							}
							if ((VEHICLE::GET_VEHICLE_LIVERY_COUNT(pedVeh) > 1) && VEHICLE::GET_VEHICLE_LIVERY(pedVeh) >= 0)
							{
								VEHICLE::SET_VEHICLE_LIVERY(playerVeh, VEHICLE::GET_VEHICLE_LIVERY(pedVeh));
							}
							int neonColor[3];
							VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(pedVeh, &neonColor[0], &neonColor[1], &neonColor[2]);
							VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(playerVeh, neonColor[0], neonColor[1], neonColor[2]);
							VEHICLE::SET_VEHICLE_WINDOW_TINT(playerVeh, VEHICLE::GET_VEHICLE_WINDOW_TINT(pedVeh));
							VEHICLE::SET_VEHICLE_DIRT_LEVEL(playerVeh, VEHICLE::GET_VEHICLE_DIRT_LEVEL(pedVeh));
						}
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicleModelHash);

						});
					});
			}
		}
		else
		{
			Utils::notifyAboveMap((char*)"Player is not in a vehicle.");
		}
	}
	void whoIsAnimed(bool enable) {
		Entity target;
		Ped local = PLAYER::PLAYER_PED_ID();
		for (int i = 0; i < 33; i++) {
			Ped player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
			if (player != PLAYER::PLAYER_PED_ID()) {
				PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &target);
				if (target == PLAYER::PLAYER_ID()) {
					notifyMap(std::to_string(player));
				}
			}
		}
	}
	void bullSharkTestosterone(bool enable)
	{
		globalHandle(2437549).At(3880).As<int>() = enable ? NETWORK::GET_NETWORK_TIME() : 0;
	}

	Hash modelDrop(int ID) {
		switch (ID) {
		case 0: return PROP_MONEY_BAG_01; break;
		case 1: return MISC::GET_HASH_KEY("prop_xmas_tree_int"); break;
		case 2: return MISC::GET_HASH_KEY("prop_alien_egg_01"); break;
		case 3: return MISC::GET_HASH_KEY("prop_cs_dildo_01"); break;
		case 4: return MISC::GET_HASH_KEY("prop_ld_int_safe_01"); break;
		case 5: return MISC::GET_HASH_KEY("prop_med_bag_01"); break;
		case 6: return MISC::GET_HASH_KEY("prop_gun_case_01"); break;
		case 7: return MISC::GET_HASH_KEY("prop_acc_guitar_01"); break;
		case 8: return MISC::GET_HASH_KEY("prop_golf_bag_01");; break;
		}
	}
	int TimeTest1 = 0;
	void moneyDrop(bool enable, int amount, int droptype, int delay, int height)
	{
		if (enable)
		{
			Ped playerped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
			if (!ENTITY::DOES_ENTITY_EXIST(playerped)) return;
			if ((timeGetTime() - TimeTest1) > delay)
			{
				g_CallbackScript->AddCallback<ModelCallback>(modelDrop(droptype), [=] {
					NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerped, 1);
					OBJECT::CREATE_AMBIENT_PICKUP(PICKUP_MONEY_CASE, playerPosition.x, playerPosition.y + height, playerPosition.z + 1.5, 0, amount, modelDrop(droptype), FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelDrop(droptype));
				});
				TimeTest1 = timeGetTime();
			}

		}
	}
	int TimeMetric;
	void dropCashv2(bool enable, int Delay, int height)
	{
		if (enable) {
			if ((timeGetTime() - TimeMetric) > Delay)
			{
				g_CallbackScript->AddCallback<ModelCallback>(-1666779307, [=] {
					Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
					NativeVector3 pCoords = ENTITY::GET_ENTITY_COORDS(iPed, 0);
					OBJECT::CREATE_AMBIENT_PICKUP(0x1E9A99F8, pCoords.x, pCoords.y + height, pCoords.z, 0, 2500, -1666779307, FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(-1666779307);
					});
				TimeMetric = timeGetTime();
			}
		}
	}

	Hash pickUps(int ID) {
		switch (ID) {
		case 0: return PickupTypeArmour; break;
		case 1: return PickupTypeHealth; break;
		case 2: return PickupTypeHealthSnack; break;
		case 3: return PickupTypeParachute; break;
		}
	}
	void clearBadSport()
	{
		STATS::STAT_SET_FLOAT($("MPPLY_OVERALL_BADSPORT"), 0.0f, TRUE);
		STATS::STAT_SET_INT($("MPPLY_DESTROYED_PVEHICLES"), 0, TRUE);
		STATS::STAT_SET_INT($("MPPLY_BADSPORT_MESSAGE"), 0, TRUE);
	}
	void pickupDrops(int pickuptype) {
		Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		if (!ENTITY::DOES_ENTITY_EXIST(selectedplayer)) return;
		NativeVector3 pos = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
		OBJECT::CREATE_AMBIENT_PICKUP(pickUps(pickuptype), pos.x, pos.y, pos.z + 1.5, 0, 100000, 1, 0, 1);
	}
	Hash ammoPickUps(int ID) {
		switch (ID) {
		case 0: return PickupTypeAmmoPistol; break;
		case 1: return PickupTypeAmmoSMG; break;
		case 2: return PickupTypeAmmoMG; break;
		case 3: return PickupTypeAmmoRifle; break;
		case 4: return PickupTypeAmmoShotgun; break;
		case 5: return PickupTypeAmmoSniper; break;
		case 6: return PickupTypeAmmoGrenadeLauncher; break;
		case 7: return PickupTypeAmmoRPG; break;
		case 8: return PickupTypeAmmoMinigun; break;
		case 9: return PickupTypeAmmoMissileMP; break;
		case 10: return PickupTypeAmmoBulletMP; break;
		case 11: return PickupTypeAmmoGrenadeLauncherMP; break;
		}
	}
	void ammoDrops(int ammotype) {
		Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		if (!ENTITY::DOES_ENTITY_EXIST(selectedplayer)) return;
		NativeVector3 pos = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
		OBJECT::CREATE_AMBIENT_PICKUP(ammoPickUps(ammotype), pos.x, pos.y, pos.z + 1.5, 0, 9999, 1, 0, 1);
	}
	Hash weaponPickup(int WeaponID) {
		switch (WeaponID) {
		case 0: return 0xF9AFB48F; break;
		}
	}
	void weaponDrops(int weapondroptype) {
		Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		if (!ENTITY::DOES_ENTITY_EXIST(selectedplayer)) return;
		NativeVector3 pos = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
		OBJECT::CREATE_AMBIENT_PICKUP(weaponPickup(weapondroptype), pos.x, pos.y, pos.z + 1.5, 0, 9999, 1, 0, 1);
	}
	void spectate(bool enable, Player player){
		if (enable) {
			g_GameFunctions->m_SpectatePlayer(enable, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
		}
		else if(!enable){
			g_GameFunctions->m_SpectatePlayer(false, PLAYER::PLAYER_PED_ID());
		}
	}
	//Grief Options
	void cagePlayer(Ped ped)
	{
		NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(ped, 0);
		OBJECT::CREATE_OBJECT(959275690, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
	}

	void airStrikePlayer(Player player) {
		NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1);
		StrikeCoords.z += 15;
		g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET")), [=] {
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 35, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1, 1, 500);
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 30, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 55, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), true, true, 600);
		});
	}
	void attachToPlayer(Player player) {
		if (PLAYER::PLAYER_ID() != PLAYER::PLAYER_PED_ID())
		{
			ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, 2, true);
		}
	}
	void detachFromPlayer(){
		ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
	}
	void clonePlayer(Player player) {
		PED::CLONE_PED(player, 1, 1, 1); //PLAYER::PLAYER_PED_ID()
	}
	void waterLoop(bool enable, Player player) {
		if (enable) {
			Player waterloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
			NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(waterloopedplayer, true);
			FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 13, 5, true, 0, 0, 0);
		}
	}
	void fireLoop(bool enable, Player player) {
		if (enable) {
			Player fireloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
			NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(fireloopedplayer, true);
			FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 12, 5, true, 0, 0, 0);
		}
	}
	void sendMessage(Player player, const char* message) {

		int handle[26];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
		if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13)) {
			NETWORK::NETWORK_SEND_TEXT_MESSAGE(message, &handle[0]);
		}
	}
	//recovery options
	void setPlayerLevel(int level)
	{
		std::string com = CString::format("%0_CHAR_SET_RP_GIFT_ADMIN", g_Settings.m_CharSlot);
		std::string notif = CString::format("Level set to: %0", level);

		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(com.c_str()), levels[level - 1], 0);
		notifyAboveMap(notif.c_str(), 0);
		notifyAboveMap("Please change sessions for the change to apply.", 0);
	}

	void maxStats()
	{
		std::string stam = CString::format("%0_SCRIPT_INCREASE_STAM", g_Settings.m_CharSlot);
		std::string strn = CString::format("%0_SCRIPT_INCREASE_STRN", g_Settings.m_CharSlot);
		std::string lung = CString::format("%0_SCRIPT_INCREASE_LUNG", g_Settings.m_CharSlot);
		std::string driv = CString::format("%0_SCRIPT_INCREASE_DRIV", g_Settings.m_CharSlot);
		std::string fly = CString::format("%0_SCRIPT_INCREASE_FLY", g_Settings.m_CharSlot);
		std::string sho = CString::format("%0_SCRIPT_INCREASE_SHO", g_Settings.m_CharSlot);
		std::string stl = CString::format("%0_SCRIPT_INCREASE_STL", g_Settings.m_CharSlot);
		std::string mech = CString::format("%0_SCRIPT_INCREASE_MECH", g_Settings.m_CharSlot);

		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(stam.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(strn.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(lung.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(driv.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(fly.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(sho.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(stl.c_str()), 100, 0);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(mech.c_str()), 100, 0);
	}
	void unlockChrome() {
		std::string captures = CString::format("%0_AWD_WIN_CAPTURES", g_Settings.m_CharSlot);
		std::string dropoff = CString::format("%0_AWD_DROPOFF_CAP_PACKAGES", g_Settings.m_CharSlot);
		std::string carrier = CString::format("%0_AWD_KILL_CARRIER_CAPTURE", g_Settings.m_CharSlot);
		std::string heists = CString::format("%0_AWD_FINISH_HEISTS", g_Settings.m_CharSlot);
		std::string setups = CString::format("%0_AWD_FINISH_HEIST_SETUP_JOB", g_Settings.m_CharSlot);
		std::string nightvision = CString::format("%0_AWD_NIGHTVISION_KILLS", g_Settings.m_CharSlot);
		std::string team = CString::format("%0_AWD_WIN_LAST_TEAM_STANDINGS", g_Settings.m_CharSlot);
		std::string alive = CString::format("%0_AWD_ONLY_PLAYER_ALIVE_LTS", g_Settings.m_CharSlot);

		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(captures.c_str()), 25, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(dropoff.c_str()), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(carrier.c_str()), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(heists.c_str()), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(setups.c_str()), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(nightvision.c_str()), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(team.c_str()), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(alive.c_str()), 50, 1);

	}
	void giveAchievements() {
		for (int i = 1; i < 100; i++) {
			PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(i);
		}
	}
	void spoofLevel(bool enable)
	{
		if (enable) {
			globalHandle(1590446).At(PLAYER::PLAYER_ID(), 871).At(211).At(6).As<int>() = MISC::GET_RANDOM_INT_IN_RANGE(0, 8000);
		}
	}

	void spoofKills(bool enable)
	{
		if (enable)
		{
			globalHandle(1590446).At(PLAYER::PLAYER_ID(), 871).At(211).At(28).As<int>() = MISC::GET_RANDOM_INT_IN_RANGE(0, 8000);
		}
		
	}

	void spoofWalletMoney(bool enable)
	{
		if (enable) {
			globalHandle(1590446).At(PLAYER::PLAYER_ID(), 871).At(211).At(3).As<int>() = MISC::GET_RANDOM_INT_IN_RANGE(0, 2147000000);
		}
	}

	void spoofBankMoney(bool enable)
	{
		if (enable)
		{
			globalHandle(1590446).At(PLAYER::PLAYER_ID(), 871).At(211).At(56).As<int>() = MISC::GET_RANDOM_INT_IN_RANGE(0, 2147000000);
		}

	}

	void spoofKD(bool enable)
	{
		if (enable)
		{
			globalHandle(1590446).At(PLAYER::PLAYER_ID(), 871).At(211).At(26).As<int>() = MISC::GET_RANDOM_INT_IN_RANGE(0, 99);
		}

	}
	
	void spawnVehicle(Ped ped, uint32_t hash, bool isSpawnIn, bool isMax, bool isSpawnVehicleInAir, bool isInvincible) {
		Hash model = hash;
		g_FiberScript->addTask([=] {
			setNetworkedScript([=] {
				STREAMING::REQUEST_MODEL(model);
				g_CallbackScript->AddCallback<ModelCallback>(model, [=] {
					globalHandle(4268190).As<bool>() = true;
					NativeVector3 ourCoords = ENTITY::GET_ENTITY_COORDS(ped, false);

					Vehicle veh{};

					bool isFlyingVehicle = VEHICLE::IS_THIS_MODEL_A_HELI(model) || VEHICLE::IS_THIS_MODEL_A_PLANE(model);

					float forward = 5.f;
					float heading = ENTITY::GET_ENTITY_HEADING(ped);
					float xVector = forward * sin(heading * 3.141592653589793f / 180.0f) * -1.0f;
					float yVector = forward * cos(heading * 3.141592653589793f / 180.0f);
					float zVector = isFlyingVehicle ? 320.0f : 0.0f;

					if (isSpawnVehicleInAir) {
						veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x + xVector, ourCoords.y + yVector, ourCoords.z + zVector, heading, true, 0); // thisScriptCheck = false for the new bypass
						lastVeh = veh;
					}
					else {
						veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x + xVector, ourCoords.y + yVector, ourCoords.z, heading, true, 0); // thisScriptCheck = false for the new bypass
						lastVeh = veh;
						VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
					}
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Mystic");
					DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
					auto networkId = NETWORK::VEH_TO_NET(veh);
					ENTITY::_SET_ENTITY_SOMETHING(veh, true);

					if (isSpawnIn || isSpawnVehicleInAir)
						PED::SET_PED_INTO_VEHICLE(ped, veh, -1);

					if (isMax) {
						VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
						for (int i = 0; i < 50; i++) {
							VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
						}
					}

					if (isInvincible) {
						ENTITY::SET_ENTITY_INVINCIBLE(veh, isInvincible);
					}

					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);

					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

					});
				});
		});
	}
	void spawnByKeyboard() {
		Hash model = Utils::getVehicleInput();
		spawnVehicle(PLAYER::PLAYER_PED_ID() ,model, true, true, false, false);
	}
	void bomberMode(bool enable, int type, int r, int g, int b, float scale) {
		NativeVector3 detonateLoc;
		bool coolDown = false;
		bool beginTimer = false;
		if (enable) {
			if (PED::IS_PED_IN_ANY_HELI(PLAYER::PLAYER_PED_ID()) || PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID())) {
				NativeVector3 entityCoord = ENTITY::GET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 1);
				MISC::GET_GROUND_Z_FOR_3D_COORD(entityCoord.x, entityCoord.y, entityCoord.z, &entityCoord.z, 0);
				if (!beginTimer)
					detonateLoc = entityCoord;
				GRAPHICS::DRAW_MARKER(6, detonateLoc.x, detonateLoc.y, detonateLoc.z, 0.f, 0.f, 0.f, 90.f, 0.f, 0.f, scale, scale, scale, r, g, b, 150, 0, 0, 0, 0, NULL, NULL, 0);

				if (GetAsyncKeyState(VK_KEY_E)) {
					FIRE::ADD_EXPLOSION(detonateLoc.x, detonateLoc.y, detonateLoc.z, type, 10.f, true, false, 0.f, false);

				}
			}
		}
	}
	void maxVehicle(Vehicle vehicle)
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
		for (int i = 0; i < 50; i++)
		{
			VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
		}
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, "Mystic");
	}
	void repairVehicle(Vehicle vehicle)
	{
		VEHICLE::SET_VEHICLE_FIXED(vehicle);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0);
	}
	void popTires()
	{
		Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
		bool isPedInVeh = PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
		if (isPedInVeh)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=] {
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, true);
				for (int i = 5; i--;)
					VEHICLE::SET_VEHICLE_TYRE_BURST(vehicle, i, true, 1000.f);
			});
		}
	}
	void slingShot(Vehicle vehicle) {
		ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, 0, 0, 20, 0, 0, 0, 1, false, true, true, true, true);
	}
	void hornBoost(bool enable, Player player, int speed) {
		if (enable) {
			if (PLAYER::IS_PLAYER_PRESSING_HORN(player))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(player, false);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
				g_CallbackScript->AddCallback<NetworkControlCallback>((veh), [=] {
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
				});
			}
		}
	}
	void engineAlwaysOn(bool enable, Player player) {
		if (enable) {
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(player, false);
			g_CallbackScript->AddCallback<NetworkControlCallback>((veh), [=] {
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, enable, enable, enable);
				VEHICLE::SET_VEHICLE_LIGHTS(veh, 0);
				VEHICLE::_SET_VEHICLE_LIGHTS_MODE(veh, 2);
			});
		}
	}
	void carGod(bool enable) {
		if (enable) {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
				ENTITY::SET_ENTITY_INVINCIBLE(vehicle, enable);
			
				if (VEHICLE::_IS_VEHICLE_DAMAGED(vehicle))
				{
					for (int i = 0; i < 10; i++)
					{
						try {
							VEHICLE::SET_VEHICLE_TYRE_FIXED(vehicle, i);
						}
						catch (...) {
							return;
						}
					}
				}
			}
		}
	}
	void autoPilot(int style) {
		int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);
		int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);

		if (HUD::DOES_BLIP_EXIST(WaypointHandle)) {
			NativeVector3 WaypointPos = HUD::GET_BLIP_COORDS(WaypointHandle);
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD(PLAYER::PLAYER_PED_ID(), VehID, WaypointPos.x, WaypointPos.y, WaypointPos.z, 18.f, 0, VehID, style, 10.f, true);
		}
		else
			Utils::notifyAboveMap("~r~Waypointneeds to be set.");
	}
	void bulletProofTyres(bool enable) {
		if (enable) {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
				if (VEHICLE::_IS_VEHICLE_DAMAGED(vehicle))
				{
					for (int i = 0; i < 10; i++)
					{
						try {
							VEHICLE::SET_VEHICLE_TYRE_FIXED(vehicle, i);
						}
						catch (...) {
							return;
						}
					}
				}
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, !enable);
				VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(vehicle, !enable);
			}
		}
	}
	void stickToGround(bool enable)
	{
		if (enable)
		{
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]
			{
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);
			});
		}

	}
	void noBikeFall(bool enable)
	{

		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), enable);
	}
	void rainbowColor(bool enable)
	{

		if (enable)
		{
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			g_CallbackScript->AddCallback<NetworkControlCallback>((vehicle), [=]
			{
				RGBFade();
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, r, g, b);
					VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, r, g, b);
				}
			});

		}
	}
	Hash VehicleWeapon(int NumSwitch) {
		switch (NumSwitch) {
		case 0: return MISC::GET_HASH_KEY("WEAPON_FIREWORK"); break;
		case 1: return MISC::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_BULLET"); break;
		case 2: return MISC::GET_HASH_KEY("WEAPON_BALL"); break;
		case 3: return MISC::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET"); break;
		case 4: return MISC::GET_HASH_KEY("VEHICLE_WEAPON_ENEMY_LASER"); break;
		}
	}

	void vehicleWeapons(bool enable, Player player, int ID) {
		DWORD LastShot = 0;
		if (enable) {
			if (GetAsyncKeyState(VK_ADD) && PED::IS_PED_IN_ANY_VEHICLE(player, 0))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(player);
				NativeVector3 v0, v1;
				MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

				g_CallbackScript->AddCallback<AssetCallback>((Functions::VehicleWeapon(ID)), [=] {
					if (!WEAPON::HAS_WEAPON_ASSET_LOADED(Functions::VehicleWeapon(ID)))
					{
						WEAPON::REQUEST_WEAPON_ASSET(Functions::VehicleWeapon(ID), 31, 0);
						while (!WEAPON::HAS_WEAPON_ASSET_LOADED(Functions::VehicleWeapon(ID))) g_FiberScript->Wait(0);
					}

					NativeVector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
					NativeVector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
					NativeVector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
					NativeVector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
						coords0to.x, coords0to.y, coords0to.z,
						250, 1, Functions::VehicleWeapon(ID), player, 1, 0, -1.0);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
						coords1to.x, coords1to.y, coords1to.z,
						250, 1, Functions::VehicleWeapon(ID), player, 1, 0, -1.0);
					});
			}
		}
	}
	void speedometer(const char* unit, bool enable, bool numplate) {
		if (enable) {
			Ped ped = PLAYER::PLAYER_PED_ID();

			bool driving = PED::IS_PED_IN_ANY_VEHICLE(ped, 0);
			std::ostringstream Speed1; Speed1 << "Speed:~s~ ";
			std::ostringstream Speed2; Speed2 << "Speed:~s~ ";
			std::ostringstream Speed3; Speed3 << "Speed:~s~ ";
			if (driving) {
				float vehSpeed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(ped, 0));
				float meterSpeed = round(vehSpeed);
				float convertedMetric;
				float milesHourSpeed;
				convertedMetric = round(vehSpeed * 3.6);
				milesHourSpeed = round((convertedMetric / 2) / 0.8);

				Speed1 << convertedMetric << " KM/H";
				Speed2 << meterSpeed << " M/S";
				Speed3 << milesHourSpeed << " MPH";
				if (!numplate) {
					if (unit == "KM/H") {
						g_UiManager->Speedometer((const char*)Speed1.str().c_str());
					}
					else if (unit == "M/S") {
						g_UiManager->Speedometer((const char*)Speed2.str().c_str());
					}
					else if (unit == "MPH") {
						g_UiManager->Speedometer((const char*)Speed3.str().c_str());
					}
				}
				else {
					if (unit == "KM/H") {
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(ped, 0), (const char*)std::to_string(convertedMetric).c_str());
					}
					else if (unit == "M/S") {
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(ped, 0), (const char*)std::to_string(meterSpeed).c_str());
					}
					else if (unit == "MPH") {
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(ped, 0), (const char*)std::to_string(milesHourSpeed).c_str());
					}
				}
			}
		}
	}

	void drift(bool enable)
	{
		if (enable)
		{
			if (GetAsyncKeyState(0x10) || PAD::IS_CONTROL_PRESSED(2, 73))
			{
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1), 1);
			}
			else
			{
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 0);
			}
		}
	}

	//give weapons
	void GiveallWeapons()
	{
		g_FiberScript->Wait(0);
		int Weapons[] = { 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x476BF155, 0xAF3696A1, 0xB62D1F67 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), Weapons[i], false))
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void GiveallMktwo() {
		g_FiberScript->Wait(0);
		int Weapons[] = { 3219281620 ,2024373456 ,961495388 ,4208062921 ,3686625920 ,177293209 , };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), Weapons[i], 9999, 1);
		}
	}
	void GivestunGun() {
		int Weapons[] = { 0x3656C8C1,911657153 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), Weapons[i], 9999, 1);
		}
	}
	void Givedigiscanner() {
		int Weapons[] = { 0xFDBADCED,4256881901 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), Weapons[i], 9999, 1);
		}
	}
	//weapon options
	void allWeapons()
	{
		g_FiberScript->Wait(0);
		int Weapons[] = { 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x476BF155, 0xAF3696A1, 0xB62D1F67 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			if (!WEAPON::HAS_PED_GOT_WEAPON(PLAYER::PLAYER_PED_ID(), Weapons[i], false))
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void allMktwo() {
		g_FiberScript->Wait(0);
		int Weapons[] = { 3219281620 ,2024373456 ,961495388 ,4208062921 ,3686625920 ,177293209 , };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void refillAmmo()
	{
		Hash weaponhash;
		WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &weaponhash, 1);
		WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), weaponhash, 9999, 0);
	}

	void setInfAmmo(bool enable)
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), enable);
	}
	void rapidFire(bool enable) {
		if (enable) {
			Player playerPed = PLAYER::PLAYER_PED_ID();
			if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
				NativeVector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
				NativeVector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
				NativeVector3 gameplayCamDirection = Utils::rot_to_direction(&gameplayCamRot);
				NativeVector3 startCoords = Utils::add(&gameplayCam, (&Utils::multiply(&gameplayCamDirection, 1.0f)));
				NativeVector3 endCoords = Utils::add(&startCoords, &Utils::multiply(&gameplayCamDirection, 500.0f));
				Hash weaponhash;
				WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
				if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, playerPed, 1, 1, 0xbf800000);
				}
			}
		}
	}
	void oneShotKill(bool enable)
	{
		if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		{
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 9999.f);
		}
	}

	void setWeaponTint(int weaponID) {
		Hash weaponhash;
		WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &weaponhash, true);
		WEAPON::SET_WEAPON_OBJECT_TINT_INDEX(weaponhash, weaponID);
	}
	void aimbot(bool enable) //it just shoots at coord, not a real aimbot
	{
		if (enable)
		{
			const int ElementAmount = 10;
			const int ArrSize = ElementAmount * 4 + 2;

			Entity* peds = new Entity[ArrSize];
			peds[0] = ElementAmount;
			NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_INDEX(), 1);
			int PedFound = PED::GET_PED_NEARBY_PEDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()), peds, -1);
			for (int i = 0; i < PedFound; i++)
			{
				int OffsetID = i * 2 + 2;
				int bone = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[OffsetID], "SKEL_Head");
				NativeVector3 pos = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[OffsetID], bone);
					if (ENTITY::DOES_ENTITY_EXIST(peds[OffsetID]) && PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()) != peds[OffsetID])
					{
						g_CallbackScript->AddCallback<ModelCallback>((PLAYER::PLAYER_PED_ID()), [=] {
							PED::SET_PED_SHOOTS_AT_COORD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()), pos.x, pos.y, pos.z, true);
						});
					};
			}
		}
	}
	void giveAllmkUpgrades() {
		std::vector<Hash> pistolMk2{1329061674, 2396306288, 1140676955, 1709866683, 568543123, 2467084625};
		std::vector<Hash> smgMk2{190476639, 2076495324, 2681951826, 3842157419, 1038927834, 1303784126, 2774849419, 966612367};
		std::vector<Hash> assltRifleMk2{1675665560, 2640679034, 2076495324, 1108334355, 77277509, 3328927042, 2805810788, 1303784126, 1447477866, 3115408816};
		std::vector<Hash> carbineRifleMk2{ 1141059345, 2640679034, 2076495324, 1108334355, 77277509, 3328927042, 2205435306, 1303784126, 2335983627, 3663056191};
		std::vector<Hash> combatMgMk2{ 1475288264, 2640679034, 1108334355, 1060929921, 3328927042, 1303784126, 3051509595, 3607349581};
		std::vector<Hash> heavySniperMk2{ 2313935527, 2193687427, 3159677559, 3061846192, 776198721, 2890063729, 1764221345, 277524638, 1815270123};
		std::vector<Hash> WeaponHash{ 0xBFE256D4, 0x78A97CD0, 0xDBBD7280, 0x394F415C, 0xFAD1F1C9, 0xA914799};
		//now lets give them
		for (Hash hash : pistolMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_PISTOL_MK2"), hash);

		for (Hash hash : smgMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_SMG_MK2"), hash);

		for (Hash hash : assltRifleMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_ASSAULTRIFLE_MK2"), hash);

		for (Hash hash : carbineRifleMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_CARBINERIFLE_MK2"), hash);

		for (Hash hash : combatMgMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_COMBATMG_MK2"), hash);

		for (Hash hash : heavySniperMk2)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("WEAPON_HEAVYSNIPER_MK2"), hash);

		for (Hash hash : WeaponHash)
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 9999, 0);
	}
	void stunGun() {
		int Weapons[] = { 0x3656C8C1,911657153 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void digiscanner() {
		int Weapons[] = { 0xFDBADCED,4256881901 };
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void valkyrieRocket(bool enable) {
		static std::int32_t rocket, cam;
		static std::uint8_t y;
		static float bar;
		static bool init;
		if(enable){
			if (Mystic::Utils::isPedShooting(PLAYER::PLAYER_PED_ID())) {
				if (!init) init = true;
			}
			if (init) {
				if (!ENTITY::DOES_ENTITY_EXIST(rocket)) {
					auto weapon = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PLAYER::PLAYER_PED_ID());
					auto c = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(weapon, 0.f, 1.f, 0.f);
					rocket = OBJECT::CREATE_OBJECT($("w_lr_rpg_rocket"), c.x, c.y, c.z, 1, 1, 0);
					CAM::DESTROY_ALL_CAMS(true);
					cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
					CAM::ATTACH_CAM_TO_ENTITY(cam, rocket, 0.f, 0.f, 0.f, 1);
					CAM::RENDER_SCRIPT_CAMS(1, true, 700, 1, 1, 0);
					CAM::SET_CAM_ACTIVE(cam, 1);
					ENTITY::SET_ENTITY_VISIBLE(rocket, 0, 0);
					bar = .5f; y = 255;
				}
				else {
					CAM::SET_CAM_ROT(cam, CAM::GET_GAMEPLAY_CAM_ROT(0).x, CAM::GET_GAMEPLAY_CAM_ROT(0).y, CAM::GET_GAMEPLAY_CAM_ROT(0).z, 0);
					ENTITY::SET_ENTITY_ROTATION(rocket, CAM::GET_GAMEPLAY_CAM_ROT(0).x, CAM::GET_GAMEPLAY_CAM_ROT(0).y, CAM::GET_GAMEPLAY_CAM_ROT(0).z, 0, 1);

					auto c = add(&ENTITY::GET_ENTITY_COORDS(rocket, 1), &multiply(&rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), .8f));
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(rocket, c.x, c.y, c.z, 0, 0, 0);

					HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
					PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
					ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), 1);
					HUD::_BLOCK_WEAPON_WHEEL_THIS_FRAME();

					GRAPHICS::DRAW_RECT(0.5f, 0.5f - 0.025f, 0.050f, 0.002f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f + 0.025f, 0.050f, 0.002f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f - 0.025f, 0.5f, 0.002f, 0.050f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f + 0.025f, 0.5f, 0.002f, 0.050f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f + 0.05f, 0.5f, 0.050f, 0.002f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f - 0.05f, 0.5f, 0.050f, 0.002f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f + 0.05f, 0.002f, 0.050f, 255, 255, 255, 255, 0);
					GRAPHICS::DRAW_RECT(0.5f, 0.5f - 0.05f, 0.002f, 0.050f, 255, 255, 255, 255, 0);
					GRAPHICS::SET_TIMECYCLE_MODIFIER("CAMERA_secuirity");

					GRAPHICS::DRAW_RECT(0.25f, 0.5f, 0.03f, 0.5f, 255, 255, 255, 255, 0);
					static auto ticker = GetTickCount();
					if (GetTickCount() - ticker >= 100) {
						bar -= .01f; y -= 4; ticker = GetTickCount();
					}
					GRAPHICS::DRAW_RECT(0.25f, 0.75f - (bar / 2), 0.03f, bar, 255, y, 0, 255, 0);

					float groundZ;
					MISC::GET_GROUND_Z_FOR_3D_COORD(ENTITY::GET_ENTITY_COORDS(rocket, 1).x, ENTITY::GET_ENTITY_COORDS(rocket, 1).y, ENTITY::GET_ENTITY_COORDS(rocket, 1).z, &groundZ, 0);
					if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(rocket) ||
						(std::abs(ENTITY::GET_ENTITY_COORDS(rocket, 1).z - groundZ) < .5f) ||
						bar <= 0.01) {
						auto impact_coord = ENTITY::GET_ENTITY_COORDS(rocket, 1); ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), 0);
						FIRE::ADD_EXPLOSION(impact_coord.x, impact_coord.y, impact_coord.z, 0, 10.f, 1, 0, .4f, 0);
						ENTITY::DELETE_ENTITY(&rocket);
						rocket = 0;
						PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 0);
						CAM::RENDER_SCRIPT_CAMS(0, true, 700, 1, 1, 0);
						CAM::DESTROY_CAM(cam, 1);
						GRAPHICS::SET_TIMECYCLE_MODIFIER("DEFAULT");
						init = false;
					}
				}
			}
		}
		
		else {
			CAM::DESTROY_CAM(cam, 1);
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
			rocket = 0;
			bar = .5f;
			y = 255;
		}
	}
	void killSpoofer(bool enable, int selected) {
		if (enable) {
			Entity AimedAtEntity;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &AimedAtEntity) && IsKeyPressed(VK_LBUTTON)) {
				if (PED::IS_PED_A_PLAYER(AimedAtEntity)) {
					g_GameVariables->OwnedExplosionBypass(true);
					g_GameFunctions->m_AddOwnedExplosion(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selected), coordsOf(AimedAtEntity), EXP_TAG_GRENADE, 5.0f, false, true, 0.1f);
					g_GameVariables->OwnedExplosionBypass(false);
				}
			}
		}
	}
	void shootVehicles(bool enable) {
		if (enable) {
			setNetworkedScript([=] {
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				FLOAT heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				if (PED::IS_PED_SHOOTING(playerPed))
				{
					float offset;
					const int Veh[] = { 0xb779a091, 0x25c5af13, 0x9ae6dda1, 0xb1d95da0, 0xb2fe5cf9,	//"ADDER", "BANSHEE2", "BULLET", "CHEETAH", "ENTITYXF",
					0x5502626c, 0x4992196c, 0x30d3f6d8, 0x18f25ac7, 0x767164d6,	//"FMJ", "GP1", "SHEAVA", "INFERNUS", "OSIRIS",
					0xb6846a55, 0x92ef6e04, 0x7e8f677f, 0xdf381e5, 0xee6024bc,	//"LE7B", "PFISTER811", "PROTOTIPO", "REAPER", "SULTANRS",
					0x6322b39a, 0x185484e1, 0x7b406efb, 0x142e0dc3, 0x9f4b77be,	//"T20", "TURISMOR", "TYRUS", "VACCA", "VOLTIC",
					0xac5df515,	0xE99011C2, 0x3D7C6410, 0xBC5DC07E, 0x8198AEDC,	//"ZENTORNO", "TYRANT", "TEZERACT", "TAIPAN", "ENTITY XXR" 
					0xED552C74, 0x5097F589, 0x52FF9437, 0x59A9E570, 0x7397224C,  // AUTARCH COMMET SR SC 1 COIL CYCLONE VISIONE
					0x36B4A8A9 };
					int vehicle;
					int random = MISC::GET_RANDOM_INT_IN_RANGE(0, 31);
					int Player = PLAYER::PLAYER_PED_ID();
					Hash vehmodel = Veh[random];
					NativeVector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
					NativeVector3 dir = Utils::rot_to_direction(&rot);
					NativeVector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
					NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
					float spawnDistance = Utils::get_distance(&camPosition, &playerPosition);
					spawnDistance += 5;
					NativeVector3 spawnPosition = Utils::add(&camPosition, &Utils::multiply(&dir, spawnDistance));


					NativeVector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
					if (PED::IS_PED_ON_FOOT(playerPed))
					{
						STREAMING::REQUEST_MODEL(vehmodel);
						g_CallbackScript->AddCallback<ModelCallback>(vehmodel, [=] {

							Vehicle veh = VEHICLE::CREATE_VEHICLE(vehmodel, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, true, 0);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 120.0);
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehmodel);
							ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
							});
					}
				}
				});
		}
	}
	bool grav_target_locked = false;
	void gravityGun(bool enable)
	{
		if (enable) {
			Entity EntityTarget;
			Hash equippedWeapon;
			WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &equippedWeapon, 1);
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			NativeVector3 rot;
			CAM::GET_GAMEPLAY_CAM_ROT(2);
			NativeVector3 dir = Utils::rot_to_direction(&rot);
			NativeVector3 camPosition;
			g_GameFunctions->m_GetGameplayCamCoord(&camPosition);
			NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			float spawnDistance = Utils::get_distance(&camPosition, &playerPosition);
			spawnDistance += 6;
			NativeVector3 spawnPosition = Utils::add(&camPosition, &Utils::multiply(&dir, spawnDistance));


			Player tempPed = PLAYER::PLAYER_ID();

			if (grav_target_locked)
			{
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &EntityTarget) && GetAsyncKeyState(VK_RBUTTON))
				{
					NativeVector3 EntityTargetPos = ENTITY::GET_ENTITY_COORDS(EntityTarget, 0);
					PLAYER::DISABLE_PLAYER_FIRING(tempPed, true);
					if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 0))
					{
						EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
					}

					Utils::requestControlEnt(EntityTarget);

					if (ENTITY::IS_ENTITY_A_VEHICLE(EntityTarget))
						ENTITY::SET_ENTITY_HEADING(
							EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));

					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(EntityTarget, spawnPosition.x, spawnPosition.y, spawnPosition.z, 0, 0, 0);

					if (GetAsyncKeyState(VK_LBUTTON))
					{
						ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));
						ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f,
							0.0f, 0, 0, 1, 1, 0, 1);
						grav_target_locked = false;
						PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
					}
				}
			}
			if (!PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &EntityTarget))
			{
				grav_target_locked = true;
				PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
			}
		}
	}
	void projectileEditor(bool enable, Hash hash) {
		if (enable) {

			Ped playerPed = PLAYER::PLAYER_PED_ID();
			float startDistance = Utils::get_distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
			float endDistance = Utils::get_distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
			startDistance += 5.25;
			endDistance += 1000.0;

			if (PED::IS_PED_ON_FOOT(playerPed) && PED::GET_PED_CONFIG_FLAG(playerPed, 58, 1))
			{
				g_CallbackScript->AddCallback<AssetCallback>((hash), [=] {
					if (!WEAPON::HAS_WEAPON_ASSET_LOADED(hash))
					{
						WEAPON::REQUEST_WEAPON_ASSET(hash, 31, false);
						while (!WEAPON::HAS_WEAPON_ASSET_LOADED(hash))
							g_FiberScript->Wait(0);
					}
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).x, Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).y, Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).z, Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).x, Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).y, Utils::add(&CAM::GET_GAMEPLAY_CAM_COORD(), &Utils::multiply(&Utils::rot_to_direction(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).z, 250, 1, hash, playerPed, 1, 0, -1.0);
					});
			}
		}
	}
	void rainbowGun(bool enable) {
		if (enable) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			static LPCSTR weaponNames[] = {
				"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
				"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
				"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
				"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
				"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
				"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
				"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE",
				"WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER", "WEAPON_PROXMINE", "WEAPON_SNOWBALL",
				"WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET", "WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN",
				"WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN", "WEAPON_FLAREGUN",
				"WEAPON_KNUCKLE", "GADGET_NIGHTVISION", "GADGET_PARACHUTE", "WEAPON_MARKSMANPISTOL"
			};
			for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
			{
				if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, MISC::GET_HASH_KEY((char*)weaponNames[i]), 0))
				{

					WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, MISC::GET_HASH_KEY((char*)weaponNames[i]), rand() % 8);

				}
			}
		}
	}
	Hash modelgun(int ID) {
		switch (ID) {
		case 0: return PROP_MONEY_BAG_01; break;
		case 1: return MISC::GET_HASH_KEY("prop_xmas_tree_int"); break;
		case 2: return MISC::GET_HASH_KEY("prop_alien_egg_01"); break;
		case 3: return MISC::GET_HASH_KEY("prop_cs_dildo_01"); break;
		case 4: return MISC::GET_HASH_KEY("prop_ld_int_safe_01"); break;
		case 5: return MISC::GET_HASH_KEY("prop_med_bag_01"); break;
		case 6: return MISC::GET_HASH_KEY("prop_gun_case_01"); break;
		case 7: return MISC::GET_HASH_KEY("prop_acc_guitar_01"); break;
		case 8: return MISC::GET_HASH_KEY("prop_golf_bag_01");; break;
		}
	}
	void moneygun(bool enable, int proptype, int price) {
		if (enable) {
			STREAMING::REQUEST_MODEL(modelgun(proptype));
			Player playerPed = PLAYER::PLAYER_PED_ID();
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000))
			{
				NativeVector3 iCoord;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord);
				if (!STREAMING::HAS_MODEL_LOADED(modelgun(proptype))) {

				}
				else {
					OBJECT::CREATE_AMBIENT_PICKUP(PICKUP_MONEY_CASE, iCoord.x, iCoord.y, iCoord.z + 0.5f, 0, price, modelgun(proptype), FALSE, TRUE);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelgun(proptype));
				}
			}
		}
	}
	void triggerBot(bool enable) //powerful at least
	{
		if (enable)
		{
			Entity PlayerTarget;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &PlayerTarget))
			{
				if (ENTITY::IS_ENTITY_A_PED(PlayerTarget) && !ENTITY::IS_ENTITY_DEAD(PlayerTarget) && ENTITY::GET_ENTITY_ALPHA(PlayerTarget) == 255)
				{
					NativeVector3 Head = PED::GET_PED_BONE_COORDS(PlayerTarget, SKEL_Head, 0.1f, 0.0f, 0.0f);
					PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), Head.x, Head.y, Head.z, true);
				}
			}
		}
	}

	void deleteGun(bool enable)
	{
		if (enable)
		{
			Entity del_entity;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &del_entity))

				if (ENTITY::DOES_ENTITY_EXIST(del_entity) && NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(del_entity) && PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
				{
					NativeVector3 to_delete = ENTITY::GET_ENTITY_COORDS(del_entity, 1);
					ENTITY::DETACH_ENTITY(del_entity, 1, 1);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(del_entity, 0, 0, 0, 0, 0, 0);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(del_entity, 0, 1);
					ENTITY::DELETE_ENTITY(&del_entity);
				}
		}
	}
	void pedSwapGun(bool enable)
	{
		if (enable)
		{
			Entity EntityTarget;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &EntityTarget) && GetAsyncKeyState(VK_LBUTTON)) {
				if (ENTITY::IS_ENTITY_A_PED(EntityTarget)) {
					Hash model = ENTITY::GET_ENTITY_MODEL(EntityTarget);
					g_CallbackScript->AddCallback<ModelCallback>((model), [=] {
						if (STREAMING::IS_MODEL_VALID(model)) {
							STREAMING::REQUEST_MODEL(model);
							while (!STREAMING::HAS_MODEL_LOADED(model)) g_FiberScript->Wait(0);
							PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
							PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
						}
					});
				}
			}
		}
	}

	void dildoKnife()
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), $("WEAPON_KNIFE"), 1, TRUE);
		WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), $("WEAPON_KNIFE"), 1);
		STREAMING::REQUEST_MODEL($("prop_cs_dildo_01"));
		auto knife = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PLAYER::PLAYER_PED_ID());
		auto c = ENTITY::GET_ENTITY_COORDS(knife, 1);
		auto dildo = OBJECT::CREATE_OBJECT($("prop_cs_dildo_01"), c.x, c.y, c.z, 1, 1, 0);
		ENTITY::SET_ENTITY_ALPHA(knife, 0, 0);
		NativeVector3 v0, v1;
		MISC::GET_MODEL_DIMENSIONS($("prop_cs_dildo_01"), &v0, &v1);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(dildo, knife, v1.z, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 1);
	}
	void explosiveGun(bool enable, int type, int damage, bool audible, bool invisible)
	{
		if (enable)
		{
			Player playerPed = PLAYER::PLAYER_PED_ID();
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000))
			{
				NativeVector3 iCoord;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord);
				FIRE::ADD_EXPLOSION(iCoord.x, iCoord.y, iCoord.z, type, damage, audible, invisible, 0, 0);
			}
		}
	}
	void airstrikeGun(bool enable)
	{
		if (enable)
		{
			Player playerPed = PLAYER::PLAYER_PED_ID();
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000))
			{
				NativeVector3 iCoord;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord);
				iCoord.z += 15;
				g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET")), [=] {
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(iCoord.x, iCoord.y, iCoord.z + 35, iCoord.x, iCoord.y, iCoord.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), playerPed, 1, 1, 500);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(iCoord.x, iCoord.y, iCoord.z + 30, iCoord.x, iCoord.y, iCoord.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(iCoord.x, iCoord.y, iCoord.z + 55, iCoord.x, iCoord.y, iCoord.z, 100, false, 0xF8A3939F, playerPed, true, true, 600);
					});
			}
		}
	}
	void fireworkGun(bool enable)
	{
		if (enable)
		{
			Player playerPed = PLAYER::PLAYER_PED_ID();
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000))
			{
				NativeVector3 iCoord;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord);
				FIRE::ADD_EXPLOSION(iCoord.x, iCoord.y, iCoord.z, 38, 100, 1, 0, 0, 0);
			}
		}
	}
	Hash animals(int ID) {
		switch (ID) {
		case 0: return -832573324; ///boar
		case 1: return -1430839454; //chickienhawk
		case 2: return -1469565163; //chimp
		case 3: return 351016938; //chop
		case 4: return 1457690978; //Cormorant
		case 5: return -50684386; //cow
		case 6: return 1682622302; //coyote
		case 7: return 402729631; //crow
		case 8: return -664053099; //deer
		case 9: return 802685111; //fish
		case 10: return 1794449327; //Hen
		case 11: return 1318032802; //husky
		case 12: return 307287994; //Mountain lion
		case 13: return -1323586730; //pig
		case 14: return 111281960; //pigeon
		case 15: return 1125994524; //poodle
		case 16: return -1011537562; //rat
		case 17: return 882848737; //retriever
		case 18: return -1026527405; //Rhesus
		case 19: return -1788665315; //rottweiler
		case 20: return -745300483; //seagull
		case 21: return 1126154828; //sheppard

		}
	}
	void meowGun(bool enable, int ID)
	{
		if (enable)
		{
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
			{
				g_CallbackScript->AddCallback<ModelCallback>((animals(ID)), [=] {
					NativeVector3 impact;
					STREAMING::REQUEST_MODEL(animals(ID));
					while (!STREAMING::HAS_MODEL_LOADED(animals(ID))) g_FiberScript->Wait(0);
					WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &impact);
					PED::CREATE_PED(PED_TYPE_ANIMAL, animals(ID), impact.x, impact.y, impact.z + 0.1f, 0, 1, 1);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(animals(ID));
					});
			}
		}
	}

	void pedGun(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
			{
				NativeVector3 coords;
				if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))
				{
					PED::CREATE_RANDOM_PED(coords.x, coords.y, coords.z);
				}

			}
		}
	}

	//Gun effects
	void cartoonGun(bool enable)
	{
		if (enable)
		{
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
				g_CallbackScript->AddCallback<PTFXAssetCallback>(("scr_rcbarry2"), [=] {
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
					while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_rcbarry2")) g_FiberScript->Wait(0);

					auto weaponIndexed = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PLAYER::PLAYER_PED_ID());
					NativeVector3 v0, v1;
					MISC::GET_MODEL_DIMENSIONS(WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), &v0, &v1);
					GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_rcbarry2");
					GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_ENTITY_2("muz_clown", weaponIndexed, (v0.x - v1.x) / 2.0f + 0.7f, 0.f, 0.f, 0.f, 180.f, 0.f, 1.f, 1, 1, 1);
					});
			}
		}
	}

	//misc options
	void resetVehicleSTime()
	{
		std::string com = CString::format("%0_MONEY_EARN_SELLING_VEH", g_Settings.m_CharSlot);

		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_VEHICLE_SELL_TIME"), 0, true);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY(com.c_str()), 50000, true);
	}
	void FPS(bool enable) {
		if (enable) {
			float LastFrameTime = MISC::GET_FRAME_TIME();
			int getFPS = (1.0f / LastFrameTime);
			char FPStext[60];
			snprintf(FPStext, sizeof(FPStext), "FPS : %d", getFPS);
			
		}
	}
	void merryweather(const char* type)
	{
		if (type == "AMMO") {
			globalHandle(2531497).At(833).As<int>() = 1;
		}
		else if (type == "BOAT") {
			globalHandle(2531497).At(834).As<int>() = 1;
		}
		else if (type == "HELI") {
			globalHandle(2531497).At(835).As<int>() = 1;
		}
		else if (type == "BACKUPHELI") {
			globalHandle(2531497).At(4389).As<int>() = 1;
		}
		else if (type == "AIRSTRIKE") {
			globalHandle(2531497).At(4390).As<int>() = 1;
		}
	}

	void noOrbCooldown(bool enable)
	{
		if (enable)
		{
			std::string canonchar = CString::format("%0_ORBITAL_CANNON_COOLDOWN", g_Settings.m_CharSlot);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY(canonchar.c_str()), 0, 0);
		}
	}

	void bypassChat(bool enable) {
		if (enable) {
			NETWORK::NETWORK_OVERRIDE_RECEIVE_RESTRICTIONS(PLAYER::PLAYER_PED_ID(), true);
			NETWORK::NETWORK_OVERRIDE_CHAT_RESTRICTIONS(PLAYER::PLAYER_PED_ID(), true);
		}
	}
	void freeCam(bool enable)
	{
		static bool lock;
		static std::int32_t cam;
		static float dist;
		auto rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		auto coord = CAM::GET_GAMEPLAY_CAM_COORD();
		NativeVector3 p_coord = { 0,0,0 };
		if (!CAM::DOES_CAM_EXIST(cam)) {
			cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
			CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);
			CAM::SET_CAM_COORD(cam, coord.x, coord.y, coord.z);
		}

		if (enable)
		{

			CAM::RENDER_SCRIPT_CAMS(true, true, 700, true, true, 1);
			CAM::SET_CAM_ACTIVE(cam, 1);
			CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);

			p_coord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), p_coord.x, p_coord.y, p_coord.z, 0, 0, 0);
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();

			auto speed = .5f;
			if (IsKeyPressed(VK_LSHIFT))
			{
				speed += .3f;
			}

			if (IsKeyPressed(VK_KEY_S))
			{
				speed /= -1;
				auto c = add(&CAM::GET_CAM_COORD(cam), &multiply(&rot_to_direction(&rot), speed));
				CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
			}

			if (IsKeyPressed(VK_KEY_W))
			{
				auto c = add(&CAM::GET_CAM_COORD(cam), &multiply(&rot_to_direction(&rot), speed));
				CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
			}
		}
		if (!enable)
		{
			//CAM::RENDER_SCRIPT_CAMS(false, true, 700, true, true, 1);
			CAM::SET_CAM_ACTIVE(cam, 0);
			CAM::DESTROY_CAM(cam, true);
		}
	}

	void moonGravity(bool enable)
	{
		if (enable)
		{
			MISC::SET_GRAVITY_LEVEL(2);
		}
		else
		{
			MISC::SET_GRAVITY_LEVEL(0);
		}
	}
	void Chaos(bool enable)
	{
		if (enable) {
			updateNearbyArray();
			for (auto& ped : closestPeds)
			{
				g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
					ENTITY::SET_ENTITY_HAS_GRAVITY(ped, false);
					NativeVector3 velocity = ENTITY::GET_ENTITY_VELOCITY(ped);
					velocity.z = 2.f;
					if (ENTITY::IS_ENTITY_A_PED(ped) && !PED::IS_PED_RAGDOLL(ped))
					{
						PED::SET_PED_TO_RAGDOLL(ped, 2000, 0, 2, false, false, false);
						velocity.z = 5.f;
						ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0, 0, ENTITY::IS_ENTITY_A_PED(ped) ? 10.f : 5.f, 0, 0, 0, 1, 1, 1, 1, 1, 1);
					}
					ENTITY::SET_ENTITY_VELOCITY(ped, velocity.x, velocity.y, velocity.z);
				});
			}
			for (auto& vehicles : closestVehicles)
			{
				g_CallbackScript->AddCallback<NetworkControlCallback>(vehicles, [=] {
					ENTITY::SET_ENTITY_HAS_GRAVITY(vehicles, false);
					NativeVector3 velocity = ENTITY::GET_ENTITY_VELOCITY(vehicles);
					velocity.z = 2.f;
					ENTITY::SET_ENTITY_VELOCITY(vehicles, velocity.x, velocity.y, velocity.z);
					ENTITY::APPLY_FORCE_TO_ENTITY(vehicles, 1, 0, 0, ENTITY::IS_ENTITY_A_PED(vehicles) ? 10.f : 5.f, 0, 0, 0, 1, 1, 1, 1, 1, 1);
				});
			}
		}
	}
	void killNearbyPed() {
		Functions::updateNearbyArray();
		for (auto& ped : Functions::closestPeds)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
				PED::APPLY_DAMAGE_TO_PED(ped, 1000, false);
				});
		}
	}
	void deleteNearbyPeds() {
		Functions::updateNearbyArray();
		for (auto& ped : Functions::closestPeds)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, 6400.f, 6400.f, 0.f, false, false, false);
				});
		}
	}
	void killNearbyLoop(bool enable) {
		if (enable) {
			Functions::updateNearbyArray();
			for (auto& ped : Functions::closestPeds)
			{
				g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
					PED::APPLY_DAMAGE_TO_PED(ped, 1000, false);
					});
			}
		}

	}
	void deleteNearbyPedsLoop(bool enable) {
		if (enable) {
			Functions::updateNearbyArray();
			for (auto& ped : Functions::closestPeds)
			{
				g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, 6400.f, 6400.f, 0.f, false, false, false);
					});
			}
		}

	}
	void forceAgressiveDriving()
	{
		Functions::updateNearbyArray();
		for (auto& ped : Functions::closestPeds)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(ped, [=] {
				AUDIO::SET_AGGRESSIVE_HORNS(true);
				PED::SET_DRIVER_AGGRESSIVENESS(ped, 1.0f);
				PED::SET_PED_CAN_EVASIVE_DIVE(ped, true);
				PED::SET_PED_STEERS_AROUND_OBJECTS(ped, true);
				PED::SET_PED_STEERS_AROUND_VEHICLES(ped, true);
				PED::SET_PED_STEERS_AROUND_PEDS(ped, true);
				});
		}
	}
	void deleteNearbyVehicles() {
		Functions::updateNearbyArray();
		for (auto& vehicles : Functions::closestVehicles)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(vehicles, [=] {
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(vehicles, 6400.f, 6400.f, 0.f, false, false, false);
				});
		}
	}
	void maxNearby()
	{
		Functions::updateNearbyArray();
		for (auto& vehicles : Functions::closestVehicles)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(vehicles, [=] {
				VEHICLE::SET_VEHICLE_MOD_KIT(vehicles, 0);
				for (int i = 0; i < 50; i++)
				{
					VEHICLE::SET_VEHICLE_MOD(vehicles, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicles, i) - 1, false);
				}
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicles, "Kitsune");
				});
		}
	}
	void killEngine()
	{
		Functions::updateNearbyArray();
		for (auto& vehicle : Functions::closestVehicles)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(vehicle, [=] {
				VEHICLE::SET_VEHICLE_ENGINE_ON(vehicle, 0, 1, 0);
				VEHICLE::SET_VEHICLE_LIGHTS(vehicle, 4);
				VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, 0);
				VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, -1);
				});
		}
	}
	void outOfControl()
	{
		Functions::updateNearbyArray();
		for (auto& vehicle : Functions::closestVehicles)
		{
			g_CallbackScript->AddCallback<NetworkControlCallback>(vehicle, [=] {
				VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(vehicle, true, true);

				});
		}
	}
	void skipCutscenes()
	{
		CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
	}
	void restricted(bool enable)
	{
		if (enable) {
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
			MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_doors");
		}
	}
	void teleportNearestCar(Player player) {
		Ped playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
		NativeVector3 position = coordsOf(playerPed);
		Vehicle veh = VEHICLE::GET_CLOSEST_VEHICLE(position.x, position.y, position.z, 500.f, 0, 0);
		g_CallbackScript->AddCallback<NetworkControlCallback>((veh), [=]
		{
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
				DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
		});
	}
	//protection options
	/*void ScriptedGameEventTest(bool enable) {
		if (enable) {
			g_GameFunctions->m_ScriptedGameEvent();
		}
	}*/
	void antiKick(bool enable)
	{
		if (enable) {
			int kickprtx[44] = {
				16, 20, 21, 22, 23, 24, 52, 300, 339, 342, 452, 456, 462, 464, 472, 473, 502, 510, 539,
				541, 546, 571, 583, 584, 589, 591, 603, 604, 605, 606, 608, 609, 615, 616, 617, 624, 666,
				700, 703, 705, 708, 709, 741, -1
			};
			for (int i = 0; i < 43; i++) {
				globalHandle(1386431).At(kickprtx[i]).As<int>() = kickprtx[i];
			}
		}
	}
	void antiCEOKick(bool enable) {
		if (enable) {
			globalHandle(1386431).At(543).As<int>() = 543;
		}
	}
	void antiCEOBan(bool enable) {
		if (enable) {
			globalHandle(1386431).At(565).As<int>() = 565;
		}
	}
	void antiSendMission(bool enable) {
		if (enable) {
			globalHandle(1386431).At(692).As<int>() = 692;
		}
	}
	void antiNotifications(bool enable) {
		if (enable) {
			globalHandle(1386431).At(1387240).As<int>() = 1387240;
		}
	}
	void antiKickFromVeh(bool enable) {
		if (enable) {
			globalHandle(1386431).At(63).As<int>() = 63;
		}
	}
	void antiBlockPassive(bool enable) {
		if (enable) {
			globalHandle(1386431).At(555).As<int>() = 555;
		}
	}
	void antiAll(bool enable)
	{
		if (enable) {
		g_GameFunctions->DefuseEvent(SCRIPT_ARRAY_DATA_VERIFY_EVENT, enable);
		g_GameFunctions->DefuseEvent(REQUEST_CONTROL_EVENT, enable);
		g_GameFunctions->DefuseEvent(GIVE_CONTROL_EVENT, enable);
		g_GameFunctions->DefuseEvent(WEAPON_DAMAGE_EVENT, enable);
		g_GameFunctions->DefuseEvent(REQUEST_PICKUP_EVENT, enable);
		g_GameFunctions->DefuseEvent(REQUEST_MAP_PICKUP_EVENT, enable);
		g_GameFunctions->DefuseEvent(GAME_CLOCK_EVENT, enable);
		g_GameFunctions->DefuseEvent(GAME_WEATHER_EVENT, enable);
		g_GameFunctions->DefuseEvent(RESPAWN_PLAYER_PED_EVENT, enable);
		g_GameFunctions->DefuseEvent(GIVE_WEAPON_EVENT, enable);
		g_GameFunctions->DefuseEvent(REMOVE_WEAPON_EVENT, enable);
		g_GameFunctions->DefuseEvent(REMOVE_ALL_WEAPONS_EVENT, enable);
		g_GameFunctions->DefuseEvent(VEHICLE_COMPONENT_CONTROL_EVENT, enable);
		g_GameFunctions->DefuseEvent(FIRE_EVENT, enable);
		g_GameFunctions->DefuseEvent(EXPLOSION_EVENT, enable);
		g_GameFunctions->DefuseEvent(START_PROJECTILE_EVENT, enable);
		g_GameFunctions->DefuseEvent(ALTER_WANTED_LEVEL_EVENT, enable);
		g_GameFunctions->DefuseEvent(CHANGE_RADIO_STATION_EVENT, enable);
		g_GameFunctions->DefuseEvent(RAGDOLL_REQUEST_EVENT, enable);
		g_GameFunctions->DefuseEvent(PLAYER_TAUNT_EVENT, enable);
		g_GameFunctions->DefuseEvent(PLAYER_CARD_STAT_EVENT, enable);
		g_GameFunctions->DefuseEvent(DOOR_BREAK_EVENT, enable);
		g_GameFunctions->DefuseEvent(SCRIPTED_GAME_EVENT, enable);
		}
		
	}
	void antiPtfx(bool enable)
	{
		if (enable) {
			g_GameFunctions->DefuseEvent(NETWORK_PTFX_EVENT, enable);
		}
	}
	void antiStat(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(NETWORK_BANK_REQUEST_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_INCREMENT_STAT_EVENT, enable);
		}
	}
	void antiSound(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(NETWORK_SOUND_CAR_HORN_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_PLAY_SOUND_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_STOP_SOUND_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT, enable);
		}
	}
	void antiFreeze(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(RAGDOLL_REQUEST_EVENT, enable);
			g_GameFunctions->DefuseEvent(INCIDENT_ENTITY_EVENT, enable);
			g_GameFunctions->DefuseEvent(CLEAR_AREA_EVENT, enable);
			g_GameFunctions->DefuseEvent(CLEAR_RECTANGLE_AREA_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_CLEAR_PED_TASKS_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_SOUND_CAR_HORN_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_PLAY_SOUND_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_STOP_SOUND_EVENT, enable);
			g_GameFunctions->DefuseEvent(NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT, enable);
		}
	}
	void antiWeapon(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(GIVE_WEAPON_EVENT, enable);
			g_GameFunctions->DefuseEvent(REMOVE_WEAPON_EVENT, enable);
			g_GameFunctions->DefuseEvent(REMOVE_ALL_WEAPONS_EVENT, enable);
		}
	}
	void antiControl(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(REQUEST_CONTROL_EVENT, enable);
			g_GameFunctions->DefuseEvent(GIVE_CONTROL_EVENT, enable);
			g_GameFunctions->DefuseEvent(VEHICLE_COMPONENT_CONTROL_EVENT, enable);
		}
	}
	void antiPickup(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(REQUEST_PICKUP_EVENT, enable);
			g_GameFunctions->DefuseEvent(REQUEST_MAP_PICKUP_EVENT, enable);
		}
	}
	void antiweatherTime(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(GAME_CLOCK_EVENT, enable);
			g_GameFunctions->DefuseEvent(GAME_WEATHER_EVENT, enable);
		}
	}
	void antiExplosion(bool enable)
	{
		if (enable)
		{
			g_GameFunctions->DefuseEvent(FIRE_EVENT, enable);
			g_GameFunctions->DefuseEvent(EXPLOSION_EVENT, enable);
			g_GameFunctions->DefuseEvent(REQUEST_PHONE_EXPLOSION_EVENT, enable);
			g_GameFunctions->DefuseEvent(BLOW_UP_VEHICLE_EVENT, enable);
		}
	}
	//teleports
	NativeVector3 TPCoords;
	void TPto(NativeVector3 result)
	{
		if (PED::IS_PED_IN_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), true))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(veh, result.x, result.y, result.z, false, false, false);
		}
		else {
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), result.x, result.y, result.z, false, false, false);
		}
	}

	void tpToDestination(Entity e, NativeVector3 coords)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
		g_FiberScript->Wait(0);
	}

	NativeVector3 get_blip_marker()
	{
		static NativeVector3 zero;
		NativeVector3 coords;

		bool blipFound = false;
		// search for marker blip
		int blipIterator = HUD::_GET_BLIP_INFO_ID_ITERATOR();
		for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = HUD::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				break;
			}
		}
		if (blipFound)
		{
			return coords;
		}

		return zero;
	}
#pragma region unlocks
	void unlockAll()
	{
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STAM"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STRN"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_LUNG"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_DRIV"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_FLY"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_SHO"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STL"), 100, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP0_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_ENEMYDRIVEBYKILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_USJS_COMPLETED"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_USJS_FOUND"), 50, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP0_AWD_FMWINALLRACEMODES"), 1, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP0_AWD_FMWINEVERYGAMEMODE"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_DB_PLAYER_KILLS"), 1000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_KILLS_PLAYERS"), 1000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FMHORDWAVESSURVIVE"), 21, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_TDM_MVP"), 60, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_HOLD_UP_SHOPS"), 20, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_RACES_WON"), 101, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_NO_ARMWRESTLING_WINS"), 21, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP0_AWD_FMATTGANGHQ"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FMBBETWIN"), 50000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 51, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP0_AWD_FMFULLYMODDEDCAR"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_DM_TOTALKILLS"), 500, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_DM_TOTAL_DEATHS"), 412, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TIMES_FINISH_DM_TOP_3"), 36, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_PLAYER_HEADSHOTS"), 623, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 63, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_TDM_WINS"), 13, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_GTA_RACES_WON"), 12, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_GOLF_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_TG_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_RT_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_CT_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_GRAN_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_FM_TENNIS_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TENNIS_MATCHES_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TOTAL_TDEATHMATCH_WON"), 63, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TOTAL_RACES_WON"), 101, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TOTAL_DEATHMATCH_LOST"), 23, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_TOTAL_RACES_LOST"), 36, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_25_KILLS_STICKYBOMBS"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_50_KILLS_GRENADES"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_AWD_20_KILLS_MELEE"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CMBTPISTOL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_PISTOL50_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_APPISTOL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MICROSMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTSMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CRBNRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ADVRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CMBTMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_PUMP_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SAWNOFF_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_BULLPUP_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTSHTGN_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SNIPERRFL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_HVYSNIPER_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GRNLAUNCH_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_RPG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MINIGUNS_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SMKGRENADE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_STKYBMB_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_8"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_9"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_10"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_OUTFIT"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_5"), -1, 1);;
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_6"), -1, 1);;
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_8"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_9"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_10"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_OUTFIT"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TORSO"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL2_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_DECL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TORSO"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL2_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_DECL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TEETH"), -1, 1);
		for (int i = 0; i < 2; i++)
		{
			char str[10];
			sprintf(str, "%d", i);

			Hash hash = MISC::GET_HASH_KEY(AddStrings((char*)"MP0_CLTHS_ACQUIRED_TEETH_", str));
			STATS::STAT_SET_INT(hash, -1, 1);
			Hash hashs = MISC::GET_HASH_KEY(AddStrings((char*)"MP1_CLTHS_ACQUIRED_TEETH_2", str));
			STATS::STAT_SET_INT(hashs, -1, 1);
		}
		for (int i = 0; i < 140; i++)
		{
			char str[10];
			sprintf(str, "%d", i);

			Hash hash = MISC::GET_HASH_KEY(AddStrings((char*)"MP0_DLC_APPAREL_ACQUIRED_", str));
			STATS::STAT_SET_INT(hash, -1, 1);
			Hash hashs = MISC::GET_HASH_KEY(AddStrings((char*)"MP1_DLC_APPAREL_ACQUIRED_", str));
			STATS::STAT_SET_INT(hashs, -1, 1);
		}
		for (int i = 0; i < 26; i++)
		{
			char str[10];
			sprintf(str, "%d", i);

			Hash hash = MISC::GET_HASH_KEY(AddStrings((char*)"MP0_ADMIN_CLOTHES_GV_BS_", str));
			STATS::STAT_SET_INT(hash, -1, 1);
			Hash hashs = MISC::GET_HASH_KEY(AddStrings((char*)"MP1_ADMIN_CLOTHES_GV_BS_", str));
			STATS::STAT_SET_INT(hashs, -1, 1);
		}
		for (int i = 0; i < 38; i++)
		{
			char str[10];
			sprintf(str, "%d", i);

			Hash hash = MISC::GET_HASH_KEY(AddStrings((char*)"MP0_TATTOO_FM_UNLOCKS_", str));
			STATS::STAT_SET_INT(hash, -1, 1);
			Hash hashs = MISC::GET_HASH_KEY(AddStrings((char*)"MP1_TATTOO_FM_UNLOCKS_", str));
			STATS::STAT_SET_INT(hashs, -1, 1);
		}
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STAM"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STRN"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_LUNG"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_DRIV"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_FLY"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_SHO"), 100, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STL"), 100, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP1_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_ENEMYDRIVEBYKILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_USJS_COMPLETED"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_USJS_FOUND"), 50, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP1_AWD_FMWINALLRACEMODES"), 1, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP1_AWD_FMWINEVERYGAMEMODE"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_DB_PLAYER_KILLS"), 1000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_KILLS_PLAYERS"), 1000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FMHORDWAVESSURVIVE"), 21, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_TDM_MVP"), 60, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_HOLD_UP_SHOPS"), 20, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_RACES_WON"), 101, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_NO_ARMWRESTLING_WINS"), 21, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP1_AWD_FMATTGANGHQ"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FMBBETWIN"), 50000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 51, 1);
		STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY("MP1_AWD_FMFULLYMODDEDCAR"), 1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_DM_TOTALKILLS"), 500, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_DM_TOTAL_DEATHS"), 412, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TIMES_FINISH_DM_TOP_3"), 36, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_PLAYER_HEADSHOTS"), 623, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 63, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_TDM_WINS"), 13, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_GTA_RACES_WON"), 12, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_GOLF_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_TG_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_RT_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_CT_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_GRAN_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_FM_TENNIS_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TENNIS_MATCHES_WON"), 2, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TOTAL_TDEATHMATCH_WON"), 63, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TOTAL_RACES_WON"), 101, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TOTAL_DEATHMATCH_LOST"), 23, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MPPLY_TOTAL_RACES_LOST"), 36, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_25_KILLS_STICKYBOMBS"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_50_KILLS_GRENADES"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_AWD_20_KILLS_MELEE"), 50, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CMBTPISTOL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_PISTOL50_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_APPISTOL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MICROSMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTSMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CRBNRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ADVRIFLE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CMBTMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTMG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_PUMP_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SAWNOFF_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_BULLPUP_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTSHTGN_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SNIPERRFL_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_HVYSNIPER_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GRNLAUNCH_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_RPG_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MINIGUNS_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SMKGRENADE_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_STKYBMB_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 600, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_8"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_9"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_10"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_OUTFIT"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_5"), -1, 1);;
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_6"), -1, 1);;
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_8"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_9"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_10"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_OUTFIT"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TORSO"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL2_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_DECL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TORSO"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_3"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_4"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_5"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_6"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_7"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL2"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL2_1"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_DECL"), -1, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TEETH"), -1, 1);



		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_REV_DA_IN_POSSESSION"), -1, 1);
	}
	void officeClutter()
	{
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_LIFETIME_CONTRA_EARNINGS"), 25000000, true);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_LIFETIME_CONTRA_EARNINGS"), 25000000, true);
		Utils::notifyAboveMap("You must sell something now...");
	}
	void maxSnacks() {
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_YUM_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_HEALTH_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_EPIC_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_ORANGE_BOUGHT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CIGARETTES_BOUGHT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_NO_BOUGHT_YUM_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_NO_BOUGHT_HEALTH_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_NO_BOUGHT_EPIC_SNACKS"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_NUMBER_OF_ORANGE_BOUGHT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CIGARETTES_BOUGHT"), 2000000000, 1);
	}
	void unlockArmor()
	{
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_1_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_2_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_3_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_4_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MP_CHAR_ARMOUR_5_COUNT"), 2000000000, 1);
		//
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_1_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_2_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_3_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_4_COUNT"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MP_CHAR_ARMOUR_5_COUNT"), 2000000000, 1);
	}
	void unlockFireWorks() {
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_BLUE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_WHITE"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_RED"), 2000000000, 1);
		STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_BLUE"), 2000000000, 1);
	}
	void unlockParachutes() {
		Hash Parachutes[] =
		{
			// MP0_CHAR_KIT_1_FM_UNLCK MP0_CHAR_KIT_2_FM_UNLCK MP0_CHAR_KIT_3_FM_UNLCK MP0_CHAR_KIT_4_FM_UNLCK MP0_CHAR_KIT_5_FM_UNLCK
			0xad72fbf2, 0x40bd0b85, 0xeda9551e, 0x5b09abba, 0xb2bef7f5,
			// MP0_CHAR_KIT_6_FM_UNLCK MP0_CHAR_KIT_7_FM_UNLCK MP0_CHAR_KIT_8_FM_UNLCK MP0_CHAR_KIT_9_FM_UNLCK MP0_CHAR_KIT_10_FM_UNLCK
			0xd6ae148f, 0x6ef3639f, 0x4e93ab72, 0xd958d97b, 0xabe64158,
			// MP0_CHAR_KIT_11_FM_UNLCK MP0_CHAR_KIT_12_FM_UNLCK
			0x9305c000, 0xe93fe20f,
			// MP0_CHAR_KIT_FM_PURCHASE MP0_CHAR_KIT_FM_PURCHASE2 MP0_CHAR_KIT_FM_PURCHASE3 MP0_CHAR_KIT_FM_PURCHASE4 MP0_CHAR_KIT_FM_PURCHASE5
			0x3d76c26c, 0x591945de, 0x66e0e16d, 0x3d268df9, 0xbc960cd2,
			// MP0_CHAR_KIT_FM_PURCHASE6 MP0_CHAR_KIT_FM_PURCHASE7 MP0_CHAR_KIT_FM_PURCHASE8 MP0_CHAR_KIT_FM_PURCHASE9 MP0_CHAR_KIT_FM_PURCHASE10
			0xca64a86f, 0xa1da575f, 0xb00bf3c2, 0xd8cac53f, 0x72f137e0,
			// MP0_CHAR_KIT_FM_PURCHASE11 MP0_CHAR_KIT_FM_PURCHASE12
			0xf2bc376c, 0x12d219
		};
		for (Hash h : Parachutes)
			STATS::STAT_SET_INT(h, -1, 1);
	}
	void unlockHairstyles() {
		Hash HairStyles[] = { 0x3cc6316a, 0x50763097 ,0x67ad5f05 ,0x2d1769da, 0x4269947e, 0x2c646868, 0x42e09560, 0xa07a3af, 0x2A9FD8CE, 0x485A4609, 0x569FE294, 0xD6F4633B, 0xAC7E0E4F, 0xF94D47F, 0x1DC670E2, 0x81DA390C };
		for (Hash h : HairStyles)
			STATS::STAT_SET_INT(h, -1, 1);
	}
	void unlockWeapons() {
		Hash StatsMinusOne[] =
		{
			// MP0_ADMIN_WEAPON_GV_BS_1 MP0_ADMIN_WEAPON_GV_BS_2 MP0_ADMIN_WEAPON_GV_BS_3 MP0_BOTTLE_IN_POSSESSION MP0_CHAR_FM_WEAP_UNLOCKED
			0x6b779763, 0x2fcea012, 0x4608cc86, 0xe0682331, 0xc0362f66, 0x2AC777EC, 0x1894D387, 0x1261471C, 0x9099E601, 0x811FAD39,
			// MP0_CHAR_FM_WEAP_UNLOCKED2 MP0_CHAR_WEAP_FM_PURCHASE MP0_CHAR_WEAP_FM_PURCHASE2 MP0_CHAR_FM_WEAP_ADDON_1_UNLCK MP0_CHAR_FM_WEAP_ADDON_2_UNLCK
			0xddff5128, 0x1b739cce, 0xf4b34427, 0xb0ffbb50, 0x2002f91, 0xE1551844, 0x25F16D26, 0x3211E42, 0x2A79D08A, 0xA9EA99F2,
			// MP0_CHAR_FM_WEAP_ADDON_3_UNLCK MP0_CHAR_FM_WEAP_ADDON_4_UNLCK MP0_CHAR_FM_WEAP_ADDON_5_UNLCK MP0_WEAP_FM_ADDON_PURCH MP0_WEAP_FM_ADDON_PURCH2
			0x77238ee3, 0x8BB0F098, 0x5D5099E7, 0xc0e8c793, 0xa06b70a7, 0x66310122, 0x77c2416b, 0xEC16309B, 0xE1EA41EC, 0x97DEB095,
			// MP0_WEAP_FM_ADDON_PURCH3 MP0_WEAP_FM_ADDON_PURCH4 MP0_WEAP_FM_ADDON_PURCH5  
			0xdb9d091b, 0xe97424c9, 0xbf3e505e, 0x3BA605AD, 0x4FCECF44, 0xBBF89BB8, 0xD2F7A23A, 0xB2F2F868, 0x3588ED7D, 0x56D18796, 0xA98D53F2, 0x1ED43E86, 0x2CC3DA65
		};
		for (Hash h : StatsMinusOne)
			STATS::STAT_SET_INT(h, -1, 1);
		//skins
		Hash Skins[] =
		{
			0x9FAB8BE1, 0x22F04348, 0xA0094888, 0x93FE1A03, 0x3CE45119,
			0xE076B62, 0xE31E5E37, 0xACCE32B0, 0x68DC8501, 0xE00B3050,
			0x5356F2FB, 0x75836942, 0x555D99A8, 0x97020ED3, 0x2BE59E29,
			0xDD2AB0F1, 0xB7D02DAF, 0xFF18AD04, 0xB51962A0, 0xABA404EB,
			0x17855E7C, 0x5FAC9C28, 0x792B2764, 0xA68AA848, 0x6E028B1B,
			0x9FAB8BE1,

			0x9ce23b8d, 0x34551d31, 0x72c8b289, 0xb3b0246b, 0xb1c7ed0b,
			0x7a42f6b3, 0x356623f1, 0xaa58ca9d, 0x5a9b4cdf, 0xab0ec253,
			0x2dc218c3, 0x9163995c, 0x3fe1312b, 0x1e6148ff, 0x1b17a92,
			0xf0a2b61a, 0x4d474708, 0x422d310, 0x1e6d5243, 0x8ac8e746,
			0xb82d3c53, 0x56baf664, 0x85cb967f, 0x4b31cd19, 0x2087c74c,
			0x9ce23b8d,
		};
		for (Hash h : Skins)
			STATS::STAT_SET_INT(h, 600, 1);
	}
	void unlockClothes() {
		Hash Clothes[] =
		{
			// MP0_CLTHS_AVAILABLE_HAIR MP0_CLTHS_AVAILABLE_HAIR_1 MP0_CLTHS_AVAILABLE_HAIR_2 MP0_CLTHS_AVAILABLE_HAIR_3 MP0_CLTHS_AVAILABLE_HAIR_4
			//0x3cc6316a, 0x50763097, 0x67ad5f05, 0x2d1769da, 0x4269947e,
			// MP0_CLTHS_AVAILABLE_HAIR_5 MP0_CLTHS_AVAILABLE_HAIR_6 MP0_CLTHS_AVAILABLE_HAIR_7
			//0x2c646868, 0x42e09560, 0xa07a3af,
			// MP0_CLTHS_AVAILABLE_JBIB MP0_CLTHS_AVAILABLE_JBIB_1 MP0_CLTHS_AVAILABLE_JBIB_2 MP0_CLTHS_AVAILABLE_JBIB_3 MP0_CLTHS_AVAILABLE_JBIB_4
			0x106936D7, 0x69EE4187, 0xea3729c5, 0x8c5f29f1, 0x65915c56, 0x67d160d6, 0xd143b3d5, 0x32D0534C, 0x676DBC86, 0xEA68AF9,
			// MP0_CLTHS_AVAILABLE_JBIB_5 MP0_CLTHS_AVAILABLE_JBIB_6 MP0_CLTHS_AVAILABLE_JBIB_7
			0xe38ad863, 0xb0e672ff, 0xc027119c, 0x4450F64D, 0x28A2BEED, 0x16C71B36,
			// MP0_CLTHS_AVAILABLE_LEGS MP0_CLTHS_AVAILABLE_LEGS_1 MP0_CLTHS_AVAILABLE_LEGS_2 MP0_CLTHS_AVAILABLE_LEGS_3 MP0_CLTHS_AVAILABLE_LEGS_4
			0xb8b3e0c6, 0xd1ebfd1,  0xc3aeacfa, 0xd7c408c,  0xe81375c3, 0xA17820E7, 0x942B0F3C, 0x2536B8B, 0xFC27DF34, 0xEA593B97,
			// MP0_CLTHS_AVAILABLE_LEGS_5 MP0_CLTHS_AVAILABLE_LEGS_6 MP0_CLTHS_AVAILABLE_LEGS_7
			0xb1730883, 0xce014193, 0xdf45e41c, 0xD7B3164B, 0x45B8725C, 0x355AD1A1,
			// MP0_CLTHS_AVAILABLE_FEET MP0_CLTHS_AVAILABLE_FEET_1 MP0_CLTHS_AVAILABLE_FEET_2 MP0_CLTHS_AVAILABLE_FEET_3 MP0_CLTHS_AVAILABLE_FEET_4
			0x97573104, 0x93ed984b, 0x623c34e5, 0x7369d740, 0x1cb129e0, 0xF48915A2, 0x5067864E, 0x5E3A21F3, 0x7BCC5D17, 0x8985F88A,
			// MP0_CLTHS_AVAILABLE_FEET_5 MP0_CLTHS_AVAILABLE_FEET_6 MP0_CLTHS_AVAILABLE_FEET_7
			0x2ce3ca45, 0xfb1866ab, 0x9a003be, 0x177C1478, 0x2D023F84, 0x3FB0E4E5,
			// MP0_CLTHS_AVAILABLE_BERD MP0_CLTHS_AVAILABLE_BERD_1 MP0_CLTHS_AVAILABLE_BERD_2 MP0_CLTHS_AVAILABLE_BERD_3 MP0_CLTHS_AVAILABLE_BERD_4
			0x14b08e07, 0x6f8f492d, 0x21a72d5e, 0x74f2d3f8, 0x269db74f, 0x72F2DA40, 0xE0B6E4BD, 0xCE604010, 0xCE1A3F84, 0xBC0C9B69,
			// MP0_CLTHS_AVAILABLE_BERD_5 MP0_CLTHS_AVAILABLE_BERD_6 MP0_CLTHS_AVAILABLE_BERD_7
			0x4f5708c1, 0x8133ec7a, 0xca99ff41, 0xE989F65B, 0xD8F75536, 0x8F4F41DF,
			// MP0_CLTHS_AVAILABLE_PROPS MP0_CLTHS_AVAILABLE_PROPS_1 MP0_CLTHS_AVAILABLE_PROPS_2 MP0_CLTHS_AVAILABLE_PROPS_3 MP0_CLTHS_AVAILABLE_PROPS_4
			0xb051c305, 0x7d5c4692, 0x7555b685, 0x62da918f, 0xd8217c1b, 0xC6EF3846, 0xB4FC36E4, 0xA641196E, 0xD967FFBB, 0xBAA5C237,
			// MP0_CLTHS_AVAILABLE_PROPS_5 MP0_CLTHS_AVAILABLE_PROPS_6 MP0_CLTHS_AVAILABLE_PROPS_7 MP0_CLTHS_AVAILABLE_PROPS_8 MP0_CLTHS_AVAILABLE_PROPS_9
			0xc665d8a4, 0xb6e439a1, 0xa431143b, 0xda520080, 0x9ca5f70, 0x59A48036, 0x8F1FEB2C, 0x7E7A49E1, 0xAF9B2C26, 0x22ED12C8,
			// MP0_CLTHS_AVAILABLE_PROPS_10 MP0_CLTHS_AVAILABLE_OUTFIT MP0_CLTHS_ACQUIRED_HAIR MP0_CLTHS_ACQUIRED_HAIR_1 MP0_CLTHS_ACQUIRED_HAIR_2
			0x4c7f28e9, 0xd4b41ccb, 0xa01511e1, 0x2415af29, 0x12630bc4, 0x5985C64E, 0x39BD13C2, 0xDD89D4C6, 0xE012DCE9, 0xCAECB29D,
			// MP0_CLTHS_ACQUIRED_HAIR_3 MP0_CLTHS_ACQUIRED_HAIR_4 MP0_CLTHS_ACQUIRED_HAIR_5 MP0_CLTHS_ACQUIRED_HAIR_6 MP0_CLTHS_ACQUIRED_HAIR_7
			0x41e4eabf, 0x2f96c623, 0x2b48bd87, 0x1922193a, 0x888a7809, 0xBD9917F6, 0x7340035D, 0xA58EE7FA, 0x5FBB5C54, 0x81C7A06C,
			// MP0_CLTHS_ACQUIRED_JBIB MP0_CLTHS_ACQUIRED_JBIB_1 MP0_CLTHS_ACQUIRED_JBIB_2 MP0_CLTHS_ACQUIRED_JBIB_3 MP0_CLTHS_ACQUIRED_JBIB_4
			0xaea92fe3, 0xa05ef149, 0x93bfd80b, 0x7a90a5ad, 0x69730372, 0x262E94CA, 0xA89BC9DF, 0x8EDA965D, 0xFBCBF03E, 0xF205DCB2,
			// MP0_CLTHS_ACQUIRED_JBIB_5 MP0_CLTHS_ACQUIRED_JBIB_6 MP0_CLTHS_ACQUIRED_JBIB_7
			0x5e0eeca6, 0x281f80d4, 0x1d856ba0, 0x6790C7CA, 0x5553234F, 0x4B2A0EFD,
			// MP0_CLTHS_ACQUIRED_LEGS MP0_CLTHS_ACQUIRED_LEGS_1 MP0_CLTHS_ACQUIRED_LEGS_2 MP0_CLTHS_ACQUIRED_LEGS_3 MP0_CLTHS_ACQUIRED_LEGS_4
			0x66e4ca67, 0x1b3dca99, 0x598d4737, 0x7c223a2,  0x3ce78dec, 0x7AD6ED44, 0x3180E8E0, 0x23384C4F, 0xDFE21DB, 0xFFA88530,
			// MP0_CLTHS_ACQUIRED_LEGS_5 MP0_CLTHS_ACQUIRED_LEGS_6 MP0_CLTHS_ACQUIRED_LEGS_7
			0x6b266a69, 0xa01cd459, 0x4df12fff, 0xFA5B7A96, 0xED0CDFF9, 0xD6C6B36D,
			// MP0_CLTHS_ACQUIRED_FEET MP0_CLTHS_ACQUIRED_FEET_1 MP0_CLTHS_ACQUIRED_FEET_2 MP0_CLTHS_ACQUIRED_FEET_3 MP0_CLTHS_ACQUIRED_FEET_4
			0x12b66dab, 0xcfd67d8b, 0x7964d0a9, 0xaeaf3b3d, 0x56a80b30, 0xBD702716, 0x2EEE3A4B, 0x212A9EC4, 0x824A6102, 0xAED8BA1E,
			// MP0_CLTHS_ACQUIRED_FEET_5 MP0_CLTHS_ACQUIRED_FEET_6 MP0_CLTHS_ACQUIRED_FEET_7
			0x88eaefb5, 0x327342bf, 0x63b5254a, 0x661BA8A5, 0x58760D5A, 0xCB61F32C,
			// MP0_CLTHS_ACQUIRED_BERD MP0_CLTHS_ACQUIRED_BERD_1 MP0_CLTHS_ACQUIRED_BERD_2 MP0_CLTHS_ACQUIRED_BERD_3 MP0_CLTHS_ACQUIRED_BERD_4
			0x2de14803, 0x19299a7d, 0x2b3e3ea6, 0x7574d312, 0x87e277ed, 0xD7C1EC04, 0x1039C4E0, 0x4A183894, 0x3D529F09, 0x6727F2B3,
			// MP0_CLTHS_ACQUIRED_BERD_5 MP0_CLTHS_ACQUIRED_BERD_6 MP0_CLTHS_ACQUIRED_BERD_7
			0x74a9d180, 0x86d375d3, 0x50ec0a05, 0x58ED563E, 0xD711D285, 0x88E6B630,
			// MP0_CLTHS_ACQUIRED_PROPS MP0_CLTHS_ACQUIRED_PROPS_1 MP0_CLTHS_ACQUIRED_PROPS_2 MP0_CLTHS_ACQUIRED_PROPS_3 MP0_CLTHS_ACQUIRED_PROPS_4
			0xd846c640, 0x18ea3634, 0x3460aec,  0xa1d5c80d, 0x5573af4a, 0xEF73F981, 0xF889C25C, 0xEB4F27E7, 0xD820818A, 0xC971E42D,
			// MP0_CLTHS_ACQUIRED_PROPS_5 MP0_CLTHS_ACQUIRED_PROPS_6 MP0_CLTHS_ACQUIRED_PROPS_7 MP0_CLTHS_ACQUIRED_PROPS_8 MP0_CLTHS_ACQUIRED_PROPS_9
			0x3f3182c6, 0x6ad05a03, 0xeaecda3a, 0x9d25bead, 0x87779351, 0xC47CDA43, 0xB6C33ED0, 0xA35917FC, 0x7C96CA7C, 0x6BC528D9,
			// MP0_CLTHS_ACQUIRED_PROPS_10 MP0_CLTHS_ACQUIRED_OUTFIT MP0_CLTHS_AVAILABLE_TORSO MP0_CLTHS_AVAILABLE_SPECIAL MP0_CLTHS_AVAILABLE_SPECIAL_1
			0x466c7ee5, 0xe667df2c, 0x8eb5a109, 0x135b7256, 0x17e66c11, 0x8987AF23, 0x558AD3B, 0x594926A5, 0xF72A91D0, 0x1AEE2BA3,
			// MP0_CLTHS_AVAILABLE_SPECIAL_2 MP0_CLTHS_AVAILABLE_SPECIAL_3 MP0_CLTHS_AVAILABLE_SPECIAL_4 MP0_CLTHS_AVAILABLE_SPECIAL_5 MP0_CLTHS_AVAILABLE_SPECIAL_6
			0x5e5b78fa, 0x2f9c9b7d, 0xeb7a1335, 0x11f3e7f,  0x753cae8, 0x6895C6F5, 0xE5D2416C, 0xD3A01D08, 0xE1E6B995, 0x2F8E54E3,
			// MP0_CLTHS_AVAILABLE_SPECIAL_7 MP0_CLTHS_AVAILABLE_DECL MP0_CLTHS_AVAILABLE_TEETH MP0_CLTHS_AVAILABLE_TEETH_1 MP0_CLTHS_AVAILABLE_TEETH_2
			0x1c91f564, 0x9c7866c7, 0xba50aedd, 0x2e99d7e,  0x112339f1, 0xACDA4F79, 0xA45F8FA5, 0x50EE4BB3, 0xA38FAC3D, 0x91C108A0,
			// MP0_CLTHS_ACQUIRED_TORSO MP0_CLTHS_ACQUIRED_SPECIAL MP0_CLTHS_ACQUIRED_SPECIAL_1 MP0_CLTHS_ACQUIRED_SPECIAL_2 MP0_CLTHS_ACQUIRED_SPECIAL_3
			0x85648a0a, 0xd1c336af, 0x5a1bdf57, 0x70020b23, 0x7cc0249f, 0xC7E1C824, 0x93CD00B9, 0xE1034060, 0x60FEC059, 0x733D64D6,
			// MP0_CLTHS_ACQUIRED_SPECIAL_4 MP0_CLTHS_ACQUIRED_SPECIAL_5 MP0_CLTHS_ACQUIRED_SPECIAL_6 MP0_CLTHS_ACQUIRED_SPECIAL_7 MP0_CLTHS_ACQUIRED_SPECIAL2
			0x42733006, 0xa132ed84, 0xd96fddfd, 0xe5b1f681, 0x8803887c, 0xA08CBF74, 0xAAD1D3FE, 0x2A4D52F7, 0x3BD5F608, 0x5E745104,
			// MP0_CLTHS_ACQUIRED_SPECIAL2_1 MP0_CLTHS_ACQUIRED_DECL MP0_CLTHS_ACQUIRED_TEETH MP0_CLTHS_ACQUIRED_TEETH_1 MP0_CLTHS_ACQUIRED_TEETH_2
			0x7e9b5630, 0x9e36043,  0xa1425b8,  0x91050cea, 0x68fc3cd5, 0x662CE27A, 0xAFF1B575, 0x9919F993, 0xDEC71135, 0xF48A3CBB,
			// MP0_DLC_APPAREL_ACQUIRED_0 MP0_DLC_APPAREL_ACQUIRED_1 MP0_DLC_APPAREL_ACQUIRED_2 MP0_DLC_APPAREL_ACQUIRED_3 MP0_DLC_APPAREL_ACQUIRED_4
			0x58087574, 0x4dffe16f, 0x402645bc, 0xf0942689, 0xea6a1a35, 0x94B71349, 0x9F8628E7, 0xAE0945ED, 0xBBCAE170, 0xDDF4A5C3,
			// MP0_DLC_APPAREL_ACQUIRED_5 MP0_DLC_APPAREL_ACQUIRED_6 MP0_DLC_APPAREL_ACQUIRED_7 MP0_DLC_APPAREL_ACQUIRED_8 MP0_DLC_APPAREL_ACQUIRED_9
			0x1b87fc70, 0x6e4d32a,  0xb80eb57f, 0xd156e80f, 0xa00e858b, 0xEC6E42B6, 0x9A319E42, 0xA9003BDF, 0x26AE3735, 0x31554C83,
			// MP0_DLC_APPAREL_ACQUIRED_10 MP0_DLC_APPAREL_ACQUIRED_11 MP0_DLC_APPAREL_ACQUIRED_12 MP0_DLC_APPAREL_ACQUIRED_13 MP0_DLC_APPAREL_ACQUIRED_14
			0x786ce981, 0x5e25b4f3, 0x851d82e2, 0xfa586d56, 0xb14a5b3b, 0x5E1EBA6B, 0x31BD61A9, 0x7BE775FC, 0x6C21D671, 0x966C2AF5,
			// MP0_DLC_APPAREL_ACQUIRED_15 MP0_DLC_APPAREL_ACQUIRED_16 MP0_DLC_APPAREL_ACQUIRED_17 MP0_DLC_APPAREL_ACQUIRED_18 MP0_DLC_APPAREL_ACQUIRED_19
			0x978b27bd, 0x41c27c29, 0x2fffd8a4, 0xde5e3562, 0xcca491ef, 0x85AD8978, 0xB2B6E38A, 0xA3F54607, 0x5D32B883, 0x50781F0E,
			// MP0_DLC_APPAREL_ACQUIRED_21 MP0_DLC_APPAREL_ACQUIRED_22 MP0_DLC_APPAREL_ACQUIRED_23 MP0_DLC_APPAREL_ACQUIRED_24 MP0_DLC_APPAREL_ACQUIRED_24
			0x6bc7cf17, 0xc7a886d7, 0xd543220c, 0x1d1931b7, 0x1d1931b7, 0xBE78FC56, 0xC8F61150, 0xC1D1831B, 0xB7606E39, 0xED675A46,
			// MP0_DLC_APPAREL_ACQUIRED_25 MP0_DLC_APPAREL_ACQUIRED_26 MP0_DLC_APPAREL_ACQUIRED_27 MP0_DLC_APPAREL_ACQUIRED_28 MP0_DLC_APPAREL_ACQUIRED_29
			0x32be5d01, 0xd594a2b7, 0xeb77ce7d, 0x3304dd96, 0x40c67919, 0xD33D25F2, 0x7ECB7D10, 0x6D3CD9F3, 0x9A6F3457, 0x889F90B8,
			// MP0_DLC_APPAREL_ACQUIRED_30 MP0_DLC_APPAREL_ACQUIRED_31 MP0_DLC_APPAREL_ACQUIRED_32 MP0_DLC_APPAREL_ACQUIRED_33 MP0_DLC_APPAREL_ACQUIRED_34
			0x21993997, 0x76cee405, 0x3d23f0ac, 0x32addbc0, 0xe09e37a2, 0xE7D8D00D, 0xDA163488, 0xCB5D9717, 0xB5C26BE1, 0x11CC23F7,
			// MP0_DLC_APPAREL_ACQUIRED_35 MP0_DLC_APPAREL_ACQUIRED_36 MP0_DLC_APPAREL_ACQUIRED_37 MP0_DLC_APPAREL_ACQUIRED_38 MP0_DLC_APPAREL_ACQUIRED_39
			0xce3492cf, 0x94611f25, 0x2c57bf0,  0x98902783, 0x865d831e, 0x30D867A, 0xED4D5AFA, 0xDE8FBD7F, 0x59E5B429, 0x442C88B7,
			// MP0_DLC_APPAREL_ACQUIRED_40 MP0_ADMIN_CLOTHES_GV_BS_1 MP0_ADMIN_CLOTHES_GV_BS_2 MP0_ADMIN_CLOTHES_GV_BS_3 MP0_ADMIN_CLOTHES_GV_BS_4
			0xd3c59cd1, 0x62e2d64,  0xbc5d19bb, 0xaaedf6dd, 0xd130c362, 0x64AC5C36, 0xF8AA28B5, 0x5EC2F4E9, 0x4EC854F4, 0x3C77B053,
			// MP0_ADMIN_CLOTHES_GV_BS_5 MP0_ADMIN_CLOTHES_GV_BS_6 MP0_ADMIN_CLOTHES_GV_BS_7 MP0_ADMIN_CLOTHES_GV_BS_8 MP0_ADMIN_CLOTHES_GV_BS_9
			0xcf3cbf7a, 0x65536ba9, 0x4f824007, 0x85d0aca3, 0x746309c8, 0x32E59D2F, 0x979E669F, 0x8C104F83, 0x7A00AB64, 0xDEA074A6,
			// MP0_ADMIN_CLOTHES_GV_BS_10 MP0_ADMIN_CLOTHES_GV_BS_11 MP0_ADMIN_CLOTHES_GV_BS_12 MP0_ADMIN_CLOTHES_GV_BS_13
			0xcb76a780, 0xe6a9dde6, 0xefe2f058, 0x1f90cd, 0xE5B0D20F, 0xFC21FEF1, 0xD2D02C32, 0xE267CB61
		};
		for (Hash h : Clothes)
			STATS::STAT_SET_INT(h, -1, 1);

		//tshirts
		STATS::STAT_SET_BOOL(0x9cf3d019, 1, 1); // MP0_AWD_FINISH_HEIST_NO_DAMAGE
		STATS::STAT_SET_BOOL(0x59D644B9, 1, 1); // MP1_AWD_FINISH_HEIST_NO_DAMAGE

		STATS::STAT_SET_BOOL(0x45b6712, 1, 1); // MP0_AWD_STORE_20_CAR_IN_GARAGES
		STATS::STAT_SET_BOOL(0xA5F06324, 1, 1); // MP1_AWD_STORE_20_CAR_IN_GARAGES

		STATS::STAT_SET_INT(0x2ae837e4, 1, 1); // MP0_AWD_FMPICKUPDLCCRATE1ST also in trophies
		STATS::STAT_SET_INT(0x3804231B, 1, 1); // MP1_AWD_FMPICKUPDLCCRATE1ST also in trophies

		STATS::STAT_SET_INT(0x833d3192, 10, 1); // MP0_AWD_FMHORDWAVESSURVIVE also in trophies
		STATS::STAT_SET_INT(0x18F27D05, 10, 1); // MP1_AWD_FMHORDWAVESSURVIVE also in trophies

												// MP0_AWD_WIN_CAPTURE_DONT_DYING MP0_AWD_DO_HEIST_AS_MEMBER MP0_AWD_WIN_GOLD_MEDAL_HEISTS MP0_AWD_KILL_TEAM_YOURSELF_LTS MP0_AWD_DO_HEIST_AS_THE_LEADER
		Hash Stats25[] = { 0x68d14c4e, 0x470055dc, 0x9f9ff03c, 0x4930d051, 0xc32a1dd9, 0x7B245DB0, 0xFFCB5243, 0x3D6A96C, 0x5C7450F8, 0xC13E46BC };
		for (Hash h : Stats25)
			STATS::STAT_SET_INT(h, 25, 1);

		STATS::STAT_SET_INT(0x30418014, 100, 1); // MP0_AWD_PICKUP_CAP_PACKAGES
		STATS::STAT_SET_INT(0x87B6D2D, 100, 1); // MP1_AWD_PICKUP_CAP_PACKAGES

		STATS::STAT_SET_INT(0xe2a9b0c4, 100, 1); // MP0_AWD_KILL_PSYCHOPATHS
		STATS::STAT_SET_INT(0xE5E29EEA, 100, 1); // MP1_AWD_KILL_PSYCHOPATHS
	}
	std::string DocumentsPath()
	{
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			return AddStrings(str, (char*)"\\Mystic");
		}
		return "";
	}
	std::string MainDocumentsPath()
	{
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			char temp[1024];
			std::snprintf(&temp[0], sizeof(temp) - 1, "%s//Mystic//", &str);
			try
			{

				if (!std::filesystem::exists(&temp[0])) {
					g_Logger->Info("No such directory exists.");
					std::filesystem::create_directory(&temp[0]);
					g_Logger->Info("Created The New Directory");
				}
			}
			catch (fs::filesystem_error const&)
			{
			}
			return AddStrings(str, (char*)"\\Mystic\\main.ini");
			g_Logger->Info("Return");
		}
		return "";
	}
#pragma endregion
}


