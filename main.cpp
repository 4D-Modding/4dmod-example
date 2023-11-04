//#define DEBUG_CONSOLE // Uncomment this if you want a debug console

// Mod Name. Make sure it matches the mod folder's name. Also don't forget to change the output DLL name in Project Properties->General->Target Name
#define MOD_NAME "4DMod"
#define MOD_VER "1.0"

#include <Windows.h>
#include <cstdio>
#include <4dm.h>
using namespace fdm;


void(__thiscall* StateGame_init)(StateGame* self, StateManager& s);
void __fastcall StateGame_init_H(StateGame* self, StateManager& s)
{
	// Your code that runs at first frame here (it calls when you load into the world)

	StateGame_init(self, s);
}

void(__thiscall* Player_update)(Player* self, World* world, double dt, EntityPlayer* entityPlayer);
void __fastcall Player_update_H(Player* self, World* world, double dt, EntityPlayer* entityPlayer) 
{
	// Your code that runs every frame here (it only calls when you play in world, because its Player's function)

	Player_update(self, world, dt, entityPlayer);
}

bool(__thiscall* Player_keyInput)(Player* self, GLFWwindow* window, World* world, int key, int scancode, int action, int mods);
bool __fastcall Player_keyInput_H(Player* self, GLFWwindow* window, World* world, int key, int scancode, int action, int mods) 
{
	// Your code that runs when Key Input happens (check GLFW Keyboard Input tutorials)|(it only calls when you play in world, because its Player's function)

	return Player_keyInput(self, window, world, key, scancode, action, mods);
}

DWORD WINAPI Main_Thread(void* hModule)
{
	// Create console window if DEBUG_CONSOLE is defined
#ifdef DEBUG_CONSOLE
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
	
	glfwInit();
	glewInit();

	// Hook to the StateGame::init function
	Hook(reinterpret_cast<void*>(FUNC_STATEGAME_INIT), reinterpret_cast<void*>(&StateGame_init_H), reinterpret_cast<void**>(&StateGame_init));

	// Hook to the Player::update function
	Hook(reinterpret_cast<void*>(FUNC_PLAYER_UPDATE), reinterpret_cast<void*>(&Player_update_H), reinterpret_cast<void**>(&Player_update));

	// Hook to the Player::keyInput function
	Hook(reinterpret_cast<void*>(FUNC_PLAYER_KEYINPUT), reinterpret_cast<void*>(&Player_keyInput_H), reinterpret_cast<void**>(&Player_keyInput));

	EnableHook(0);
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD _reason, LPVOID lpReserved)
{
	if (_reason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main_Thread, hModule, 0, NULL);
	return TRUE;
}
