#include "Common.hpp"

namespace Mystic::Functions {

	void setFunctionLoop(Ped ped, Player player, Vehicle vehicle, NativeVector3 coords);
	void setPlayerLoop(Ped ped, int index);
	int getInputs();
	std::string DocumentsPaths();
	char* AddStrings(char* string1, char* string2);
	//Volatile
	void setStealthMoney(int amount, bool isBank);
	void resetStealth();
	void setNetworkedScript(std::function<void()> script);
	// Player Information
	BOOL IsPlayerFriend(Player player);
	std::string getRole(int player);
	int getPlayerLevel(Player p);
	int getPlayerHealth(Ped ped);
	int getPlayerArmor(Ped ped, Player p);
	const char* getPlayerVehicle(Ped ped);
	char* getPlayerHeading(Player p);
	std::string getPlayerWeapon(Ped ped);
	int getPlayerSpeed(Ped ped);
	int getPlayerAmmo(Ped ped, Hash WeaponHash);
	char* getPlayerKDRatio(Player p);
	char* getPlayerWantedLevel(Player p);

	//helpers
	float degToRad(float degs);
	void notifyMap(const char* fmt, ...);
	void notifyMap(std::string str);
	void notifyAboveMap(std::string msg, BOOL blink);
	NativeVector3 coordsOf(Entity entity);
	void RBGFade();
	NativeVector3 add(NativeVector3* vectorA, NativeVector3* vectorB);
	NativeVector3 multiply(NativeVector3* vector, float x);
	Vector2 WorldToScreen(NativeVector3 pos);
	NativeVector3 rot_to_direction(NativeVector3* rot);
	std::string DocumentsPath();
	std::string MainDocumentsPath();
	void LoadPlugins(fs::path asi_path);

	void spotifyHook();
	void spotifyPlayPause();
	void spotifyNext();
	void spotifyPrevious();
	void songInfo();

	

	void addMoney();
	void removeMoney();
	Ped* nearbyPeds(Player player);

	//self options
	void setGodMode(bool enable);
	void setNeverWanted(bool enable);
	void maxHealth();
	void setWantedLevel(int level);
	void setSuperJump(bool enable);
	void antiAFK(bool enable);
	void camZoom(float zoom);
	void slideRun(bool enable);
	void slowMotion(bool enable);
	void cleanPed();
	void tiny(bool enable);
	void rainbowTextures();
	void sixstar(bool enable);
	void disphone(bool enable);
	void invisible(bool enable);
	void maxarmor();
	void wardrobe(int index, int clothes);
	void randoutfit();
	void setSuperSpeed(bool enable, float speed);
	void setSuperSwim(bool enable);
	void mobileRadio(bool enable);
	void noClip(bool enable);
	void forcefield(bool enable);
	void superMan(bool enable);
	void ragdoll(bool enable);
	void anim(int ID);
	void dragonBreath(bool enable);
	void walkStyle(int ID);
	const char* walkDict(int ID);
	void rainbowhaircolor(bool enable);
	void setPedHairColor(int HairID);

	//online options
	void setNetworkTime(int h, int m, int s);
	void otr(bool enable);
	void bribeAuthorities(bool enable);
	void joinMsg(bool enable);
	void kick_to_new_lobby();
	void ceo_kick();
	void ceo_ban();
	void soundSpam(bool enable, int player, int sound);
	void setName(std::string& name);
	void changeName(std::string name);

	void non_host_kick();
	void inv_inf_loading();
	void gentle_kick_from_vehicle();
	void blame(Player player);
	void copyOutfit(Player player);
	void bullSharkTestosterone(bool enable);
	void joinCrew(Player player);
	Hash modelDrop(int ID);
	void moneyDrop(bool enable, int amount, int droptype, int delay, int height);
	void dropCashv2(bool enable, int Delay, int height);
	Hash pickUps(int ID);
	void pickupDrops(int ammotype);
	Hash ammoPickUps(int ID);
	void ammoDrops(int pickuptype);
	Hash weaponPickup(int WeaponID);
	void weaponDrops(int weapondroptype);
	void spectate(bool enable, Player player);
	void instantCrash();
	void hostilePeds(bool enable, Player player);
	void whoIsAnimed(bool enable);
	void attachObjToPlayer(char* object);
	void detachAll();
	void dupeCar(int player);
	void send_to_mission();
	void remoteRemWanted();
	void transFailed();
	void set10Bounty();
	void GiveallWeapons();
	void GiveallMktwo();
	void Givedigiscanner();
	void GivestunGun();
	void destroyPersonalVeh();
	void remoteTP();
	void ramPlayer();

	//Grief Options
	void cagePlayer(Ped ped);
	void airStrikePlayer(Player player);
	void attachToPlayer(Player player);
	void detachFromPlayer();
	void clonePlayer(Player player);
	void waterLoop(bool enable, Player player);
	void fireLoop(bool enable, Player player);

	//recovery options
	void setPlayerLevel(int level);
	void maxStats();
	void giveAchievements();
	void unlockChrome();
	void spoofLevel(bool enable);
	void spoofKills(bool enable);
	void spoofWalletMoney(bool enable);
	void spoofBankMoney(bool enable);
	void spoofKD(bool enable);
	void stealthmoney(bool enable, int amount, int Delay);
	void clearBadSport();

	//vehicle options
	void spawnVehicle(Ped ped, uint32_t hash, bool isSpawnIn, bool isMax, bool isSpawnVehicleInAir, bool isInvincible);
	void spawnByKeyboard();
	void carGod(bool enable);
	void maxVehicle(Vehicle vehicle);
	void repairVehicle(Vehicle vheicles);
	void stickToGround(bool enable);
	void noBikeFall(bool enable);
	void speedometer(const char* unit, bool enable, bool numplate);
	void driveonwater(bool enable);
	void rainbowColor(bool enable);
	void drift(bool enable);
	Hash VehicleWeapon(int NumSwitch);
	void vehicleWeapons(bool enable, Player player, int ID);
	void autoPilot(int style);
	void popTires();
	void personalhornBoost(bool enable, int speed);
	void slingShot(Vehicle vehicle);
	void hornBoost(bool enable, Player player, int speed);
	void engineAlwaysOn(bool enable, Player player);
	void bomberMode(bool enable, int type, int r, int g, int b, float scale);

	//weapon options
	void allWeapons();
	void allMktwo();
	void refillAmmo();
	void setInfAmmo(bool enable);
	void oneShotKill(bool enable);
	void triggerBot(bool enable);
	void rapidFire(bool enable);
	void deleteGun(bool enable);
	void pedSwapGun(bool enable);
	void dildoKnife();
	void explosiveGun(bool enable, int type, int damage, bool audible, bool invisible);
	void airstrikeGun(bool enable);
	void meowGun(bool enable, int ID);
	void aimbot(bool enable);
	void pedGun(bool enable);
	void cartoonGun(bool enable);
	void moneygun(bool enable, int proptype, int price);
	void valkyrieRocket(bool enable);
	void killSpoofer(bool enable, int selected);
	void giveAllmkUpgrades();
	void stunGun();
	void digiscanner();
	void setWeaponTint(int weaponID);
	void projectileEditor(bool enable, Hash hash);
	void rainbowGun(bool enable);
	void shootVehicles(bool enable);
	void gravityGun(bool enable);

	//misc options 
	void resetVehicleSTime();
	void merryweather(const char* type);
	void noOrbCooldown(bool enable);
	void freeCam(bool enable);
	void moonGravity(bool enable);
	void skipCutscenes();
	void restricted(bool enable);
	void teleportToWay();
	void teleportNearestCar(Player player);
	void tpToDestination(Entity e, NativeVector3 coords);
	void TPto(NativeVector3 result);
	NativeVector3 get_blip_marker();
	void bypassChat(bool enable);
	void FPS(bool enable);

	//world options
	void Chaos(bool enable);
	void slowMotion(bool enable);
	void killNearbyPed();
	void killNearbyLoop(bool enable);
	void deleteNearbyPeds();
	void deleteNearbyPedsLoop(bool enable);
	void revealMiniMap(bool enable);
	void forceAgressiveDriving();
	void deleteNearbyVehicles();
	void maxNearby();
	void killEngine();
	void outOfControl();
	//protection options
	void antiAll(bool enable);
	void antiPtfx(bool enable);
	void antiExplosion(bool enable);
	void antiweatherTime(bool enable);
	void antiPickup(bool enable);
	void antiControl(bool enable);
	void antiWeapon(bool enable);
	void antiFreeze(bool enable);
	void antiSound(bool enable);
	void antiPtfx(bool enable);
	void antiStat(bool enable);
	void antiKick(bool enable);
	void antiCEOKick(bool enable);
	void antiCEOBan(bool enable);
	void antiSendMission(bool enable);
	void antiNotifications(bool enable);
	void antiKickFromVeh(bool enable);
	void antiBlockPassive(bool enable);

	void unlockAll();
	void maxSnacks();
	void unlockArmor();
	void unlockFireWorks();
	void unlockParachutes();
	void unlockHairstyles();
	void unlockWeapons();
	void unlockClothes();
	void officeClutter();
	void spawn_vehicle(Hash toSpawn, Ped player);
	void ModelChange(const char* model);

}