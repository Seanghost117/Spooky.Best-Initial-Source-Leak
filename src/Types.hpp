#pragma once

namespace Mystic
{
	using Void = std::uint32_t;
	using Any = std::uint32_t;
	using Hash = std::uint32_t;
	using Entity = std::int32_t;
	using Player = std::int32_t;
	using FireId = std::int32_t;
	using Tick = std::uintptr_t;
	using Ped = Entity;
	using Vehicle = Entity;
	using Cam = std::int32_t;
	using CarGenerator = std::int32_t;
	using Group = std::int32_t;
	using Train = std::int32_t;
	using Object = Entity;
	using Pickup = Object;
	using Weapon = std::int32_t;
	using Interior = std::int32_t;
	using Blip = std::int32_t;
	using Texture = std::int32_t;
	using TextureDict = std::int32_t;
	using CoverPoint = std::int32_t;
	using Camera = std::int32_t;
	using TaskSequence = std::int32_t;
	using ColourIndex = std::int32_t;
	using Sphere = std::int32_t;
	using ScrHandle = std::int32_t;

	struct RGB_COL
	{
		int r = 0,
			g = 0,
			b = 0;
	};

	struct RGBA_COL
	{
		int r = 0,
			g = 0,
			b = 0,
			a = 0;

		operator RGB_COL() const
		{
			return RGB_COL{ r, g, b };
		}
	};

	struct RGBAF_COL
	{
		int r = 0,
			g = 0,
			b = 0,
			a = 0,
			f = 0;

		operator RGB_COL() const
		{
			return RGB_COL{ r, g, b };
		}

		operator RGBA_COL() const
		{
			return RGBA_COL{ r, g, b, a };
		}
	};

	struct ObjectInfo
	{
		DWORD hash;
		float X;
		float Y;
		float Z;
		float RotX;
		float RotY;
		float RotZ;
	};

	struct Vector2_x
	{
		float x = 0.f, y = 0.f;
	};

	struct Vector2_w
	{
		float w = 0.f, h = 0.f;
	};

#pragma pack(push, 1)
	struct Vector2
	{
		float x{};
		float y{};
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct NativeVector3
	{
	public:
		NativeVector3() = default;
		NativeVector3(float x, float y, float z):
			x(x), y(y), z(z)
		{}

		NativeVector3 operator*(const float& scale)
		{
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}

		NativeVector3 operator+(const NativeVector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		NativeVector3 operator-(const NativeVector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

	public:
		float x{};
	private:
		char paddingX[4];
	public:
		float y{};
	private:
		char paddingY[4];
	public:
		float z{};
	private:
		char paddingZ[4];
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct Vector3
	{
		float x{};
		float y{};
		float z{};
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct Color
	{
		std::uint8_t r{ 255 };
		std::uint8_t g{ 255 };
		std::uint8_t b{ 255 };
		std::uint8_t a{ 255 };
	};
#pragma pack(pop)

	struct Bool
	{
		std::string name;
		bool value;
		int index;
	};

	struct RGB_COLOR
	{
		int r = 0,
			g = 0,
			b = 0;
	};

	struct RGBA : RGB_COLOR
	{
		int a = 0;
	};

	struct RGBAF : RGBA
	{
		int f = 0;
	};

	struct clockTime {
		int hour, minute, second;
	};

	struct eventTypes
	{
		int hash;
		std::string name;
	};

	struct weatherTypes
	{
		std::string name;
		int type;
	};

	struct TPLocations
	{
		const char* name;
		float x{};
		float y{};
		float z{};
		std::vector<const char*> scenery_required;
		std::vector<const char*> scenery_toremove;
		bool isLoaded;
	};

	struct rstarAdmins
	{
		const char* scid;
		const char* name;	
	};

	template <size_t size>
	struct bytes
	{
		BYTE byte[size];
	};

	template <typename T>
	struct Pointer
	{
		std::string str;
		T value;
		int at_index;
	};
}
