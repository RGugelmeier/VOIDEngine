#include "Engine/Core/CoreEngine.h"

int main(int argc, char* argv[])
{
	//Create an engine and error check to ensure it has been initialized.
	if (!CoreEngine::GetInstance()->OnCreate("Game 258 Engine", 800, 600))
	{
		std::cout << "Engine failed to initialize" << std::endl;
		return 0;
	}

	//Run the engine.
	CoreEngine::GetInstance()->Run();
	return 0;
}