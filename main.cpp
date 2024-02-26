//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

// Initialize the DLLMain
initDLL

$hook(void, StateGame, init, StateManager& s)
{
	// Your code that runs at first frame here (it calls when you load into the world)

	original(self, s);
}

$hook(void, Player, update, World* world, double dt, EntityPlayer* entityPlayer)
{
	// Your code that runs every frame here (it only calls when you play in world, because its Player's function)

	original(self, world, dt, entityPlayer);
}

$hook(bool, Player, keyInput, GLFWwindow* window, World* world, int key, int scancode, int action, int mods)
{
	// Your code that runs when Key Input happens (check GLFW Keyboard Input tutorials)|(it only calls when you play in world, because its Player's function)

	return original(self, window, world, key, scancode, action, mods);
}

$hook(void, StateIntro, init, StateManager& s)
{
	original(self, s);

	// initialize opengl stuff
	glewExperimental = true;
	glewInit();
	glfwInit();
}