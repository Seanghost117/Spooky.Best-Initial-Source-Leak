#pragma once

#include "Common.hpp"

namespace Mystic::Utils {

	NativeVector3 rot_to_direction(NativeVector3*rot);
	NativeVector3 add(NativeVector3*vectorA, NativeVector3*vectorB);
	NativeVector3 multiply(NativeVector3*vector, float x);
	NativeVector3 getWaypoint();
	NativeVector3 getMissionObjective();
	NativeVector3 getGroundCoords(Vector2 coords, int tries);

	Vector2 getSpriteScale(float size);

	Hash getVehicleInput();

	extern bool checkChatMessage(std::string& str, std::string check);
	extern bool isPlayerFriend(Player player);
	extern bool isPlayerRockstarAdmin(Player player);
	extern bool isPedShooting(Ped ped);
	extern bool isPlayerDead();
	extern bool isPlayerInvincible(Ped ped);
	Player getSessionPlayerId(const char* name);

	char* getInput();
	char toLowerCase(const char c);

	extern int red, green, blue;

	float get_distance(NativeVector3*pointA, NativeVector3*pointB);
	float get_vector_length(NativeVector3*vector);
	float degToRad(float degs);

	int getNumberInput();
	int getWantedLevel(Player player);

	void notifyMap(const char* fmt, ...);
	void notifyMap(std::string str);

	void setForceToEntity(Entity entity, float x, float y, float z);
	void notifyAboveMap(std::string message);
	void notifyEventBlocked(std::string message);
	void notifyInfo(const char *text, const char *text2, const char *Subject);
	void notifyColor(std::string message, int color);
	void setFrameFlag(uint32_t flag);
	void RGBFade();
	void drawLine(Vector3 pos1, Vector3 pos2);
	void drawLine2(NativeVector3 pos1, NativeVector3 pos2);
	void drawText(const char * text, RGBAF_COL rgbaf, Vector2_x position, Vector2_w size, bool center);
	void requestControlId(int network_id);
	void requestControlEnt(Entity entity);
	void allocateNameAddresses();
	int GlobForNeverWanted(int plr);

	void notifyBlocked(char* fmt, ...);
	void notifyBlocked(std::string str);
	std::vector<DWORD64> getNameAddresses();

	template <typename T> inline std::uint64_t getSubAddress(T ptr)
	{
		return (reinterpret_cast<std::uint64_t>(ptr) - reinterpret_cast<std::uint64_t>(g_Module));
	}

	template <>
	inline std::uint64_t getSubAddress<std::uint64_t>(std::uint64_t ptr)
	{
		return (ptr - reinterpret_cast<std::uint64_t>(g_Module));
	}
	void tpToMarker();
}
