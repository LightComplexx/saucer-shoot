//
// GameStart.cpp
//

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventKeyboard.h"
#include "Color.h"

// Game includes
#include "GameStart.h"
#include "Hero.h"
#include "Points.h"
#include "Saucer.h"

GameStart::GameStart() {
	// Link to "gamestart" sprite
	setSprite("gamestart");

	// Sets object to type GameStart
	setType("GameStart");

	// Registers keyboard events
	registerInterest(df::KEYBOARD_EVENT);

	// Sets location to center of window
	df::Vector p(WM.getBoundary().getHorizontal() / 2, WM.getBoundary().getVertical() / 2);
	setPosition(p);

	// Play start music.
	p_music = RM.getMusic("start music");
	playMusic();
}

void GameStart::playMusic() {
	p_music->play();
}

// Override default draw so as not to display "value"
int GameStart::draw() {
	return df::Object::draw();
}

int GameStart::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		df::EventKeyboard* p_keyboard_event = (df::EventKeyboard*)p_e;
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::P: 			// play
			start();
			break;
		case df::Keyboard::Q:			// quit
			GM.setGameOver();
			break;
		default: // Key is not handled.
			break;
		}
		return 1;
	}

	return 0;
}

// Count down to end of "message"
void GameStart::start() {
	// Spawn player ship
	new Hero;

	// Spawn some saucers to shoot
	for (int i = 0; i < 16; i++)
		new Saucer;

	// Setup heads-up display
	new Points;	// points display

	df::ViewObject* p_vo = new df::ViewObject; // nuke_count display
	p_vo->setLocation(df::TOP_LEFT);
	p_vo->setViewString("Nukes");
	p_vo->setValue(1);
	p_vo->setColor(df::YELLOW);

	// When game starts, become inactive
	setActive(false);
	// Stop start music
	p_music->stop();
}