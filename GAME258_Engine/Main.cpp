#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"

int main(int argc, char* argv[])
{
	//Set game interface to the game we will run.
	CoreEngine::GetInstance()->SetGameInterface(new Game1);

	//Create an engine and error check to ensure it has been initialized.
	if (!CoreEngine::GetInstance()->OnCreate("Game 258 Engine", 800, 600))
	{
		Debug::FatalError("Engine failed to initialize", "Main.cpp", __LINE__);
		return 0;
	}

	//Run the engine.
	CoreEngine::GetInstance()->Run();
	return 0;
}