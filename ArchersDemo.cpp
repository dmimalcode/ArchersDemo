#include "framework.h"
#include "ArchersDemo.h"

int main(int argc, char** args)
{
	try
	{
		ArchersDemo::Game game;

		game.Create();

		game.Loop();

		game.Destroy();
	}
	catch (std::exception& e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Exception", e.what(), nullptr);
	}

	return 0;
}
