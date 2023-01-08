//#define DEBUG_CONSOLE // Define that if you want debug console

#include <Windows.h>
#include <cstdio>
#include <4dm.h>
using namespace fdm;
#include <MinHook.h>

void(__thiscall* GameState_init)(GameState* self, StateManager& s);
void __fastcall GameState_init_H(GameState* self, StateManager& s)
{
	glfwInit(); // for glfw functions to work
	glewInit(); // for glew/gl functions to work

	// Your code that runs at first frame here (it calls when you load into world)

	GameState_init(self, s);
}

void(__thiscall* Player_update)(Player* self, GLFWwindow* window, World* world, double dt);
void __fastcall Player_update_H(Player* self, GLFWwindow* window, World* world, double dt) 
{
	// Your code that runs every frame here (it only calls when you play in world, because its Player function)

	Player_update(self, window, world, dt);
}

bool(__thiscall* Player_keyInput)(Player* self, GLFWwindow* window, int key, int scancode, int action, int mods);
bool __fastcall Player_keyInput_H(Player* self, GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	// Your code that runs when Key Input happens (check GLFW Keyboard Input tutorials)|(it only calls when you play in world, because its Player function)

	return Player_keyInput(self, window, key, scancode, action, mods);
}

DWORD WINAPI Main_Thread(void* hModule)
{
	// Create console window if DEBUG_CONSOLE is defined
#ifdef DEBUG_CONSOLE
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
	// Initialize MinHook and make sure its initialized
	auto mhInitialize = MH_Initialize();
	if (mhInitialize != MH_OK)
	{
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
		return false;
	}

	// Hook to the GameState::init function
	MH_CreateHook(reinterpret_cast<void*>(base + idaOffsetFix(0x50690)), reinterpret_cast<void*>(&GameState_init_H), reinterpret_cast<void**>(&GameState_init));

	// Hook to the Player::update function
	MH_CreateHook(reinterpret_cast<void*>(base + idaOffsetFix(0x7EB40)), reinterpret_cast<void*>(&Player_update_H), reinterpret_cast<void**>(&Player_update));

	// Hook to the Player::keyInput function
	MH_CreateHook(reinterpret_cast<void*>(base + idaOffsetFix(0x81880)), reinterpret_cast<void*>(&Player_keyInput_H), reinterpret_cast<void**>(&Player_keyInput));

	MH_EnableHook(MH_ALL_HOOKS);
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD _reason, LPVOID lpReserved)
{
	if (_reason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main_Thread, hModule, 0, NULL);

	return TRUE;
}