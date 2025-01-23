//
// game.cpp
// 

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Pause.h"

// Game includes
#include "Star.h"
#include "GameStart.h"
#include "GameOver.h"

// Function Prototypes
void loadResources();
void populateWorld();


int main(int argc, char *argv[]) {
	// Start up game manager
	if (GM.startUp()) {
		LM.writeLog("Error starting game manager!");
		GM.shutDown();
		return 1;
	}

	// Set flush of logfile during development (when done, make false)
	LM.setFlush(true);

	// Show splash screen
	df::splash();


	//------------------------------------------------------------------
	// Load game resources
	loadResources();

	// Populate game world with some objects
	populateWorld();

	// Enable player to pause game
	new df::Pause(df::Keyboard::ESCAPE);

	// Run game (this blocks until game loop is over)
	GM.run();
	//------------------------------------------------------------------


	// Shut everything down
	GM.shutDown();
	return 0;
}

void loadResources() {
	// Load needed game sprites
	RM.loadSprite("sprites/bullet-spr.txt", "bullet");
	RM.loadSprite("sprites/explosion-spr.txt", "explosion");
	RM.loadSprite("sprites/gameover-spr.txt", "gameover");
	RM.loadSprite("sprites/gamestart-spr.txt", "gamestart");
	RM.loadSprite("sprites/saucer-spr.txt", "saucer");
	RM.loadSprite("sprites/ship-spr.txt", "ship");
	RM.loadSprite("sprites/timebar-spr.txt", "timebar");
	RM.loadSprite("sprites/slashtext-spr.txt", "slashtext");
	RM.loadSprite("sprites/flash-spr.txt", "flash");
}

void populateWorld() {
	RM.loadSound("sounds/fire.wav", "fire");
	RM.loadSound("sounds/explode.wav", "explode");
	RM.loadSound("sounds/nuke.wav", "nuke");
	RM.loadSound("sounds/game-over.wav", "game over");
	RM.loadMusic("sounds/custom-start-music.wav", "start music");

	// Create some Stars
	for (int i = 0; i < 16; i++)
		new Star;

	// Spawn GameStart object
	new GameStart();
}

