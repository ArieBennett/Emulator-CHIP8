#include <SDL2/SDL.h>			// initializing and shutdown functions
#include <SDL2/SDL_image.h>		// rendering images and graphics on screen
#include <SDL2/SDL_timer.h> 	// SDL_Delay() functions
#include <string>
#include "chip8.h"

using namespace std;

// variables
chip8 myChip8;
string gameName;

// define functions
void setupGraphics();
void setupInput();
void drawGraphics();

int main(int argc, char **argv) {
	// setup
	setupGraphics();
	setupInput();

	// initialize chip8 system
	myChip8.initialize();
	myChip8.loadGame(gameName);

	// emulation loop
	while(true) {
		myChip8.emulateCycle();

		if(myChip8.drawFlag)
			drawGraphics();

		myChip8.setKeys();
	}

	return 0;
}

void setupGraphics() {

}

void setupGraphics() {
	
}

void drawGraphics() {

}