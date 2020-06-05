#include "engine.h"

int main()
{
	{
		Engine engine = Engine("GAME", 640, 480);
		engine.init();
		engine.gameLoop();
	}

	return 0;
}