#include "memory.h"

#include <thread>
namespace offsets
{
	constexpr auto localplayer = 0xDEA964;
	constexpr auto  entityList = 0x4DFFFC4;
	constexpr auto glowObjectManager = 0x535AA70;
	constexpr auto dwViewMatrix = 0x4DF0DF4;


	constexpr auto teamNum = 0xF4;
	constexpr auto glowIndex = 0x10488;
}
struct  vec2
{
	float x, y;
};
struct  vec3
{
	float x, y, z;
};
struct  vec4
{
	float x, y, z, w;
};

struct Color 
{
	constexpr Color(float r, float g, float b, float a = 1.f) noexcept :
	r(r), g(g), b(b), a(a) {}
	float r, g, b, a;
};
int main() 
{
	auto mem = Memory{ "csgo.exe" };

	const auto client = mem.GetModuleAddress("client.dll");

	const auto color = Color{ 3.f, 2.f, 2.f };
	

	while (true)
	{
		const auto localplayer = mem.Read<std::uintptr_t>(client + offsets::localplayer);
		const auto glowObjectManager = mem.Read<std::uintptr_t>(client + offsets::glowObjectManager);
		for (auto i = 0; i < 64; ++i)
		{
			const auto entity = mem.Read<std::uintptr_t>(client + offsets::entityList + i * 0x10);
			if (mem.Read<std::uintptr_t>(entity + offsets::teamNum) == mem.Read<std::uintptr_t>(localplayer + offsets::teamNum))
				continue;
			const auto glowIndex = mem.Read<std::int32_t>(entity + offsets::glowIndex);

			mem.Write<Color>(glowObjectManager + (glowIndex * 0x38) + 0x8, color); //color

	    	mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true); //a
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true); //a
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
}
	return 0;
}