
#pragma warning (push)
#pragma warning (disable: 4477 4189)

namespace Mystic::Utils {

	NativeVector3 rot_to_direction(NativeVector3*rot) {
		float radiansZ = rot->z*0.0174532924f;
		float radiansX = rot->x*0.0174532924f;
		float num = abs((float)cos((double)radiansX));
		NativeVector3 dir;
		dir.x = (float)((double)((float)(-(float)sin((double)radiansZ)))*(double)num);
		dir.y = (float)((double)((float)cos((double)radiansZ))*(double)num);
		dir.z = (float)sin((double)radiansX);
		return dir;
	}

	NativeVector3 add(NativeVector3*vectorA, NativeVector3*vectorB) {
		NativeVector3 result;
		result.x = vectorA->x;
		result.y = vectorA->y;
		result.z = vectorA->z;
		result.x += vectorB->x;
		result.y += vectorB->y;
		result.z += vectorB->z;
		return result;
	}

	NativeVector3 multiply(NativeVector3*vector, float x) {
		NativeVector3 result;
		result.x = vector->x;
		result.y = vector->y;
		result.z = vector->z;
		result.x *= x;
		result.y *= x;
		result.z *= x;
		return result;
	}

	float get_distance(NativeVector3*pointA, NativeVector3*pointB) {
		float a_x = pointA->x;
		float a_y = pointA->y;
		float a_z = pointA->z;
		float b_x = pointB->x;
		float b_y = pointB->y;
		float b_z = pointB->z;
		double x_ba = (double)(b_x - a_x);
		double y_ba = (double)(b_y - a_y);
		double z_ba = (double)(b_z - a_z);
		double y_2 = y_ba * y_ba;
		double x_2 = x_ba * x_ba;
		double sum_2 = y_2 + x_2;
		return(float)sqrt(sum_2 + z_ba);
	}

	float get_vector_length(NativeVector3*vector) {
		double x = (double)vector->x;
		double y = (double)vector->y;
		double z = (double)vector->z;
		return(float)sqrt(x*x + y * y + z * z);
	}

	float degToRad(float degs)
	{
		return degs * 3.141592653589793f / 180.f;
	}

	NativeVector3 getWaypoint()
	{
		static NativeVector3 zero;
		NativeVector3 coords;

		bool isBlipFound = false;
		int blipIterator = HUD::_GET_BLIP_INFO_ID_ITERATOR();
		for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = HUD::GET_BLIP_INFO_ID_COORD(i);
				isBlipFound = true;
				break;
			}
		}
		if (isBlipFound)
		{
			return coords;
		}

		return zero;
	}
	void notifyBlocked(char* fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		HUD::_SET_NOTIFICATION_BACKGROUND_COLOR(6);
		HUD::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		HUD::_DRAW_NOTIFICATION(1, 1);
	} void notifyBlocked(std::string str) { notifyBlocked(&str[0]); }
	NativeVector3 getMissionObjective()
	{
		NativeVector3 blipCoords;

		for (Blip x = HUD::GET_FIRST_BLIP_INFO_ID(1); HUD::DOES_BLIP_EXIST(x) != 0; x = HUD::GET_NEXT_BLIP_INFO_ID(1))
		{
			if (HUD::GET_BLIP_COLOUR(x) == 0x42 || HUD::GET_BLIP_COLOUR(x) == 0x5 || HUD::GET_BLIP_COLOUR(x) == 0x3C || HUD::GET_BLIP_COLOUR(x) == 0x2)
			{
				blipCoords = HUD::GET_BLIP_COORDS(x);
				break;
			}
		}
		for (Blip y = HUD::GET_FIRST_BLIP_INFO_ID(38); HUD::DOES_BLIP_EXIST(y) != 0; y = HUD::GET_NEXT_BLIP_INFO_ID(38))
		{
			if (HUD::GET_BLIP_COLOUR(y) == 0x0)
			{
				blipCoords = HUD::GET_BLIP_COORDS(y);
				break;
			}
		}
		for (Blip z = HUD::GET_FIRST_BLIP_INFO_ID(431); HUD::DOES_BLIP_EXIST(z) != 0; z = HUD::GET_NEXT_BLIP_INFO_ID(431))
		{
			if (HUD::GET_BLIP_COLOUR(z) == 0x3C)
			{
				blipCoords = HUD::GET_BLIP_COORDS(z);
				break;
			}
		}

		return blipCoords;
	}

	NativeVector3 getGroundCoords(Vector2 coords, int tries)
	{
		float groundZ;

		for (int handle = 0; handle < tries; ++handle)
		{
			if (handle)
			{
				for (float zCoord = 1000.f; zCoord >= 0.f; zCoord -= 100.f)
				{
					STREAMING::REQUEST_COLLISION_AT_COORD(NativeVector3(coords.x, coords.y, zCoord));

					g_FiberScript->Wait(0);

				}
			}

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, 1000.f, &groundZ, 0))
			{
				return NativeVector3(coords.x, coords.y, groundZ); // or groundZ + 1
			}
		}

		return NativeVector3(coords.x, coords.y, 1000.f);
	}

	char toLowerCase(const char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	void setForceToEntity(Entity entity, float x, float y, float z)
	{
		g_CallbackScript->AddCallback<NetworkControlCallback>(entity, [entity, x, y, z]
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
		});
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
	void notifyAboveMap(std::string message) {
		HUD::SET_TEXT_OUTLINE();
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)message.c_str());
		HUD::_DRAW_NOTIFICATION(false, false);
	}

	void notifyEventBlocked(std::string message)
	{
		HUD::_SET_NOTIFICATION_BACKGROUND_COLOR(6);
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)message.c_str());
		HUD::_DRAW_NOTIFICATION(true, true);
	}

	void notifyInfo(const char *text, const char *text2, const char *Subject)
	{
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::_SET_NOTIFICATION_MESSAGE_CLAN_TAG("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", false, 7, text2, Subject, 1.0, "___PHOT");
		HUD::_DRAW_NOTIFICATION(true, true);
	}

	void notifyColor(std::string message, int color)
	{
		HUD::_SET_NOTIFICATION_BACKGROUND_COLOR(color);
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)message.c_str());
		HUD::_DRAW_NOTIFICATION(1, 1);
	}

	void setFrameFlag(uint32_t flag)
	{
		uint32_t actual = Memory::get_value<uint32_t>({ 0x8, 0x10B8, 0x1F8 });
		Memory::set_value<uint32_t>({ 0x8, 0x10B8, 0x1F8 }, actual |= flag);
	}

	bool isPlayerDead()
	{
		return PED::IS_PED_DEAD_OR_DYING(PLAYER::PLAYER_PED_ID(), 1);
	}

	bool isPlayerInvincible(Ped ped)
	{
		auto ped_ptr = g_GameFunctions->m_HandleToPointer(ped);

		NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);

		STREAMING::REQUEST_COLLISION_AT_COORD(coords);

		if (ped_ptr != 0)
		{
			STREAMING::REQUEST_COLLISION_AT_COORD(coords);
			bool isInside = INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(ped));
			if (!isInside)
			{
				auto isInvincible = *reinterpret_cast<bool*>(ped_ptr + 0x189);
				return isInvincible;
			}
		}
		return false;
	}

	bool isPlayerFriend(Player player)
	{
		const int FRIEND_HANDLE_SIZE = 13;
		int NETWORK_HANDLE[FRIEND_HANDLE_SIZE];
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, NETWORK_HANDLE, FRIEND_HANDLE_SIZE);
		return NETWORK::NETWORK_IS_FRIEND(NETWORK_HANDLE) != 0;
	}

	bool isPedShooting(Ped ped) {
		NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(ped, true);
		return PED::IS_PED_SHOOTING_IN_AREA(ped, coords.x, coords.y, coords.z, coords.x, coords.y, coords.z, true, true);
	}

	bool isPlayerRockstarAdmin(Player player) {
		return NETWORK::NETWORK_PLAYER_IS_ROCKSTAR_DEV(player);
	}

	Player getSessionPlayerId(const char* name)
	{
		for (int i = 0; i < 32; i++)
		{
			if (PLAYER::GET_PLAYER_NAME(i) == name)
				return i;
		}
		return -1;
	}

	bool checkChatMessage(std::string& str, std::string check)
	{
		for (int i = 0; i < str.size(); i++)
			str[i] = toLowerCase(str[i]);

		std::size_t found = str.find_first_of(' ');
		if (found != std::string::npos)
		{
			if (str.substr(0, found) == check)
			{
				str = str.substr(found + 1, str.size());
				return true;
			}
		}
		else
		{
			if (str == check)
				return true;
		}
		return false;
	}

	int red = 255, green = 0, blue = 0;
	void RGBFade()
	{
		if (red > 0 && blue == 0) {
			red--;
			green++;
		}
		if (green > 0 && red == 0) {
			green--;
			blue++;
		}
		if (blue > 0 && green == 0) {
			red++;
			blue--;
		}
	}

	Hash getVehicleInput()
	{
		g_GameVariables->getBool("isUsingKeyboard") = true;
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "CELL_EMASH_SODF", "", "", "", "", "", 15);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) g_FiberScript->Wait(0);
		g_GameVariables->getBool("isUsingKeyboard") = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
		return MISC::GET_HASH_KEY((char*)_strdup(MISC::GET_ONSCREEN_KEYBOARD_RESULT()));
	}

	char* getInput()
	{	
		g_GameVariables->getBool("isUsingKeyboard") = true;
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "CELL_EMAIL_BODE", "", "", "", "", "", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) g_FiberScript->Wait(0);
		g_GameVariables->getBool("isUsingKeyboard") = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return (char*)"";
		return (char*)MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	int getNumberInput()
	{
		g_GameVariables->getBool("isUsingKeyboard") = true;
		MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "CELL_EMAIL_SODE", "", "", "", "", "", 20);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) g_FiberScript->Wait(0);
		g_GameVariables->getBool("isUsingKeyboard") = false;
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
		return (int)atof(MISC::GET_ONSCREEN_KEYBOARD_RESULT());
	}

	void drawLine(Vector3 pos1, Vector3 pos2)
	{
		GRAPHICS::DRAW_LINE(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, 200, 200, 200, 255);
	}

	void drawLine2(NativeVector3 pos1, NativeVector3 pos2)
	{
		GRAPHICS::DRAW_LINE(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, 200, 200, 200, 255);
	}

	void drawText(const char * text, RGBAF_COL rgbaf, Vector2_x position, Vector2_w size, bool center)
	{
		HUD::SET_TEXT_CENTRE(center);
		HUD::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
		HUD::SET_TEXT_FONT(rgbaf.f);
		HUD::SET_TEXT_SCALE(size.w, size.h);
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y, 0);
	}

	void requestControlId(int network_id)
	{
		int tick = 0;

		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(network_id) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(network_id);
			tick++;
		}
	}

	void requestControlEnt(Entity entity)
	{
		int tick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);

			tick++;
		}
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			int network_id = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
			requestControlId(network_id);
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(network_id, 1);
		}
	}

	int getWantedLevel(Player player) {
		return PLAYER::GET_PLAYER_WANTED_LEVEL(player);
	}

	Vector2 getSpriteScale(float size)
	{
		int x;
		int y;
		GRAPHICS::_GET_ACTIVE_SCREEN_RESOLUTION(&x, &y);

		return { (static_cast<float>(y) / static_cast<float>(x)) * size, size };
	}

	static std::vector<DWORD64> nameAddresses;
	std::vector<DWORD64> getNameAddresses()
	{
		return nameAddresses;
	}

	void allocateNameAddresses() {

		static DWORD64 localNameAddress = NULL;

		localNameAddress = Memory::get_multilayer_pointer(g_GameVariables->m_WorldPtr, { 0x08, 0x10B8, 0x7C });

		if (nameAddresses.empty())
		{
			std::string currentMask;
			const char* playerName = g_GameFunctions->m_GetPlayerName(PLAYER::PLAYER_ID());
			for (uint8_t i = 0; i < strlen(playerName); i++) currentMask += "x";
			const char *mask = currentMask.c_str();

			for (DWORD64 address : Memory::get_string_addresses(playerName)) {
				char buff[100];
				_snprintf_s(buff, sizeof(buff), "OtherName\t\t 0x%p (0x%.8X)", address, address - Memory::get_base());
				nameAddresses.push_back((address));
			}

			if (localNameAddress) {
				nameAddresses.push_back((localNameAddress));
			}		
		}
	}
	int GlobForNeverWanted(int plr) {
		return globalHandle(GLOBAL_FOR_REMOVE_WANTED_LEVEL + (1 + (plr * 614)) + 532).As<int>();
	}
	//TELEPORTATION
	void tpToMarker()
	{
		NativeVector3 coords = Functions::get_blip_marker();

		if (coords.x == 0 && coords.y == 0)
		{
			return;
		}

		// get entity to teleport
		Entity entity = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(entity, 0))
		{
			entity = PED::GET_VEHICLE_PED_IS_USING(entity);
		}
		bool worldLevel = false;
		static float groundCheckHeight[] =
		{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
		for (int i = 0; i < 800 / sizeof(float); i++)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
			g_FiberScript->Wait(0);
			if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0))
			{
				worldLevel = true;
				coords.z += 3.0;
				break;
			}
		}
		if (!worldLevel)
		{
			coords.z = 1000.0;
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
		}
		Functions::tpToDestination(entity, coords);
	}
}

#pragma warning (pop)
