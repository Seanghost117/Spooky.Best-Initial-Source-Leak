#pragma once
#include "Script.hpp"

namespace Mystic
{
	class MenuScript : public Script
	{
	public:
		explicit MenuScript() = default;
		~MenuScript() noexcept = default;

		bool IsInitialized() override;
		ScriptType GetType() override;
		void Initialize() override;
		void Destroy() override;
		void Tick() override;
		void InitializeVariables();
		inline static bool f_joiner{};
		inline static int f_join{};
		inline static bool IsKeyboardActive{};


		//protections
		inline static bool f_antiAll{};
		inline static bool f_logEvents{};
		inline static bool f_redirectEvents{};
		inline static bool f_notifyEvents{};
		inline static bool f_antiFreeze{};
		inline static bool f_antiPickups{};
		inline static bool f_antiPTFX{};
		inline static bool antiCamReset{};
		inline static bool f_antibounty{};
		inline static bool f_antibanner{};
		inline static bool f_antiotr{};
		inline static bool f_antiTPApt{};
		inline static bool f_antiInfLoading{};
		inline static bool f_antiSoundspam{};
		inline static bool f_antiTransError{};
		inline static bool f_antiRemoteWanted{};
		inline static bool f_antiStats{};
		inline static bool f_antiSound{};
		inline static bool f_antiControl{};
		inline static bool f_antiWeapon{};
		inline static bool f_antiWeatherTime{};
		inline static bool f_antiExplosions{};
		inline static bool f_antiKick{};
		inline static bool f_antiCEOBan{};
		inline static bool f_antiCEOKick{};
		inline static bool f_antiSendToMission{};
		inline static bool f_antiNotifs{};
		inline static bool f_antiKickFromVeh{};
		inline static bool f_antiBlockPassive{};
		inline static bool f_blockdrops{};
		inline static bool f_chatrestrict{};
	private:
		bool m_Initialized{};
		//Initialize bools here. Config/saving logic can be done easily in the InitializeVariables() function.
		//I'll just throw everything here for now.
		inline static bool f_GodMode{};
		inline static bool f_NeverWanted{};
		inline static bool f_superJump{};
		inline static bool f_antiAFK{};
		inline static bool f_SlowMoBool{};
		inline static float f_runspeed{};
		inline static bool f_slideRun{};
		inline static bool f_tiny{};
		inline static bool f_sixstars{};
		inline static int f_walkindex{};
		inline static bool f_noClip{};
		inline static bool f_forcefield{};
		inline static bool f_superMan{};
		inline static bool f_disphone{};
		inline static bool f_invisibleplayer{};
		inline static bool f_mobileRadio{};
		inline static bool f_superSpeed{};
		inline static bool f_superSwim{};
		inline static bool f_norag{};
		inline static int f_WantedLevel{};
		inline static bool f_dragonbreath{};
		inline static bool f_rainbowHair{};

		//wardrobe
		inline static int face{};
		inline static int mask{};
		inline static int hair{};
		inline static int gloves{};
		inline static int tops{};
		inline static int collar{};
		inline static int pants{};
		inline static int accessories{};
		inline static int shoes{};
		inline static int mission_stuff{};
		inline static int decals{};
		inline static bool auto_update{};

		inline static bool f_PlayerInfo{};
		inline static bool f_hostilepeds{};
		inline static bool f_whoaimed{};
		inline static bool f_PedEsp{};
		inline static bool f_PlayerEsp{};
		inline static bool f_nearbyPedEsp{};
		

		inline static bool f_rainbowVeh{};
		inline static bool f_stuckToGround{};
		inline static bool f_noBikefall{};
		inline static bool f_drift{};
		inline static bool f_cargod{};
		inline static bool f_vehweapons{};
		inline static bool f_hornboost{};
		inline static bool f_remotehornboost{};
		inline static bool f_speedometer{};
		inline static int f_speedomterunit{};
		inline static bool f_driveonwater{};
		inline static bool f_numplatespeedometer{};
		inline static int f_hornspeed{};
		inline static int f_remotehornspeed{};
		inline static bool f_enginealwayson{};
		inline static int32_t Spoiler{}, FBumper{}, RBumper{}, SSkirt{}, Exhaust{}, Grille{}, Chasis{}, Hood{}, Fender{}, RFender{}, Roof{}, Engine{}, Brakes{}, Transmission{}, Horns{}, Suspension{}, Armor{}, Turbo{}, TSmoke{}, Xenon{}, Wheels{}, Windows{}, Plateholder{}, Vanity{}, Trims{}, Ornaments{}, Dashboard{}, Dial{}, DoorSpeaker{}, Seats{}, SteeringWheel{}, Shifter{}, Plaques{}, Speakers{}, Trunk{}, Hydraulics{}, EngineBlock{}, Filter{}, Struts{}, Arch{}, Aerials{}, TrimTwo{}, Tank{}, Livery{};
		//bomber mode
		inline static bool f_bomberMode{};
		inline static int f_bombRed{};
		inline static int f_bombBlue{};
		inline static int f_bombGreen{};
		inline static int f_bombType{};
		inline static float f_bomberRadius{};

		inline static bool f_spoofLevel{};
		inline static bool f_spoofKills{};
		inline static bool f_stealth{};
		inline static int f_stealthdelay{};
		inline static int f_stealthamount{};
		inline static int f_stealthlimit{};

		inline static bool f_otr{};
		inline static bool f_bribeAuthorities{};
		inline static bool f_joinMsg{};
		inline static bool f_moneyDrop{};
		inline static bool f_waterloop{};
		inline static bool f_fireloop{};
		inline static bool f_spectate{};
		inline static int f_dropamount{};
		inline static int f_dropheight{};
		inline static bool f_newdrop{};
		inline static int f_delay{};
		inline static bool f_bullshark{};

		inline static bool f_InfAmmo{};
		inline static bool f_oneShotKill{};
		inline static bool f_triggerBot{};
		inline static bool f_projectilegun{};
		inline static bool f_deleteGun{};
		inline static bool f_pedSwapGun{};
		inline static bool f_airstrikeGun{};
		inline static bool f_fireworkGun{};
		inline static bool f_explosiveGun{};
		inline static bool f_audible{};
		inline static bool f_invisible{};
		inline static int f_damagescale{};
		inline static bool f_meowGun{};
		inline static bool f_pedGun{};
		inline static bool f_cartoonGun{};
		inline static bool f_moneygun{};
		inline static bool f_aimbot{};
		inline static int f_price{};
		inline static bool f_valk{};
		inline static int f_selectedPlayer{};
		inline static bool f_killspoofer{};
		inline static bool f_rainbowgun{};
		inline static bool f_rapidfire{};
		inline static float damage;
		inline static float spread;
		inline static float recoil;
		inline static float range;
		inline static float reloadtime;
		inline static float muzzlevelocity;
		inline static bool f_vehShooter{};
		inline static bool f_gravityGun{};

		inline static bool f_noOrbCooldown{};
		inline static bool f_freeCam{};
		inline static bool f_moonGravity{};
		//world options
		inline static bool f_worldrestrict{};
		inline static bool f_blackhole{};
		inline static bool f_chaosMode{};
		inline static bool f_slowmo{};
		inline static bool f_Thunder{};
		inline static bool f_blackout{};
		inline static bool f_nightvision{};
		inline static bool f_heatvision{};
		inline static bool f_setwaveintense{};
		inline static bool f_setrainintense{};
		inline static bool f_setwindspeed{};
		inline static bool f_setwinddirection{};
		inline static float f_waveintense{};
		inline static float f_rainintese{};
		inline static float f_windspeed{};
		inline static float f_winddirection{};
		inline static bool f_killnearbypeds{};
		inline static bool f_deletenearbypeds{};

		inline static bool ResearchUnlocks{};


		inline static bool f_TPKeyPressed{};
		inline static int f_sessionTime{};
		inline static bool f_fps{};
	};

	inline std::shared_ptr<MenuScript> g_MenuScript;
}