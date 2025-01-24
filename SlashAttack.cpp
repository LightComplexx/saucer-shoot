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
	// Sets object to type Slash
	setType("SlashAttack");

	// Sets sprite for when saucer is slashed
	setSprite("saucerslashed");

	// Makes the SaucerSlashed sprite spectral so it can pass through 
	// objects and not impede movement
	setSolidness(df::HARD);

	// Draws the SaucerSlashed sprite in the background
	setAltitude(df::MAX_ALTITUDE);

	// Registers SLASHEND event
	registerInterest(SLASHEND_EVENT);

	// Set position
	setPosition(location);
}

SlashAttack::~SlashAttack() {
	df::ObjectList object_list = WM.objectsAtPosition(getPosition());
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object* p_o = i.currentObject();
		if (p_o->getType() == "Saucer") {
			// Create an explosion
			Explosion* p_explosion = new Explosion;
			p_explosion->setPosition(p_o->getPosition());

			// Delete Saucer
			WM.markForDelete(p_o);
		}
	}

	// Play "game over" sound.
	df::Sound* p_sound = RM.getSound("game over");
	if (p_sound)
		p_sound->play();
}


int SlashAttack::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == SLASHEND_EVENT) {
		WM.markForDelete(this);
		return 1;
	}

	return 0;
}

int SlashAttack::draw() {
	return df::Object::draw();
}