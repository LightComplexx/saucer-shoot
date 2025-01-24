//
// SlashAttack.cpp
//

// System includes
#include <stdlib.h>

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventStep.h"
#include "EventSlashEnd.h"
#include "Sound.h"

// Game includes
#include "SlashAttack.h"
#include "Reticle.h"
#include "Explosion.h"

SlashAttack::SlashAttack(df::Vector location) {
	// Sets object to type SlashAttack
	setType("SlashAttack");

	// Sets sprite for 'saucerslashed' attack
	if (setSprite("saucerslashed") == 0)
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
	else
		time_to_live = 0;

	// Makes the SaucerSlashed sprite spectral so it can pass through 
	// objects and not impede movement
	setSolidness(df::HARD);

	// Draws the SaucerSlashed sprite in the background
	setAltitude(df::MAX_ALTITUDE);

	// Register events
	registerInterest(df::STEP_EVENT);

	// Set position
	setPosition(location);
}


int SlashAttack::eventHandler(const df::Event* p_e) {
	// Animation countdown
	if (p_e->getType() == df::STEP_EVENT) {
		time_to_live--;
		return 1;
	}

	return 0;
}

int SlashAttack::draw() {
	// Play animation once; delete once it ends
	if (time_to_live <= 0) {
		WM.markForDelete(this);
		return -1;
	}
	return df::Object::draw();
}