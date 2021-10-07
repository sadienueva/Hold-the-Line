#include "Game.h"
#include <time.h>

int main()
{
	srand(time(0));
	Game HoldTheLine;

	HoldTheLine.Run();

	return 0;
}