#include "memory.h"
#include <thread>

using namespace std;
namespace offsets
{
	constexpr auto localPlayer = 0xDEB99C;
	constexpr auto entityList = 0xDEB99C;
	constexpr auto glowObjectManager = 0x535BAD0;	
	//entity
	constexpr auto teamNum = 0xF4;
	constexpr auto glowIndex = 0x10488;
}

int main()
{
	auto mem = Memory{ "csgo.exe" };
	const auto client = mem.GetModuleAddress("Client.dll");
	while (true)
	{
		const auto localPlayer = mem.Read<::uintptr_t>(client + offsets::localPlayer);
		const auto glowObjectManager = mem.Read<::uintptr_t>(client + offsets::glowObjectManager);
		//64 is the max players 
		for (auto i = 0; i < 64; ++i)
		{
			const auto entity = mem.Read<::uintptr_t>(client + offsets::entityList + i * 0x10);
			if (mem.Read<::uintptr_t>(entity + offsets::teamNum) == mem.Read<::uintptr_t>(localPlayer + offsets::teamNum))
				continue;
			const auto glowIndex = mem.Read<::int32_t>(entity + offsets::glowIndex);
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, 1.f); //r
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, 0.f); //r
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, 0.f); //r
			mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, 1.f); //r

			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x27, true); 
			mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true); 


		}
		::this_thread::sleep_for(::chrono::milliseconds(1));
	}
	return 0; 
}
