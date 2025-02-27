
#include <cmath>
#include <cstdlib>
#include "Game.hpp"

int main()
{
	Game	game;

	// game loop
	while (game.isOpen()) {
		// // game polling
		game.pollEvents();

		// game update
		game.update();

		// game render
		game.render();
	}

	return EXIT_SUCCESS;
}
