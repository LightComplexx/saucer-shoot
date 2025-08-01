//
// Timebar.cpp
//

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "EventStep.h"
#include "EventSlashEnd.h"
#include "ObjectList.h"
#include "ObjectListIterator.h"

// Game includes
#include "Hero.h"
#include "Timebar.h"
#include "SlashAttack.h"

Timebar::Timebar() {
	// Sets object to type Timebar
	setType("Timebar");

	// Sets sprite
	if (setSprite("timebar") == 0)
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
	else
		time_to_live = 0;

	// Makes the Timebar spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SPECTRAL);

	// Draws the Timebar in the background
	setAltitude(df::MAX_ALTITUDE);

	// Registers step event
	registerInterest(df::STEP_EVENT);

	// Set position
	setLocation(df::TOP_CENTER);

	// Find Hero object and enable inputs
	df::ObjectList all_objects = df::WorldManager::getInstance().getAllObjects();
	df::ObjectListIterator it(&all_objects);
	while (!it.isDone()) {
		df::Object* obj = it.currentObject();
		if (obj->getType() == "Hero") {
			Hero* hero = dynamic_cast<Hero*>(obj);
			if (hero) {
				hero->setDisableInput(false);
			}
			break;
		}
		it.next();
	}
}

Timebar::~Timebar() {
	EventSlashEnd slashend;
	WM.onEvent(&slashend);
}

int Timebar::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		time_to_live--;
		return 1;
	}

	return 0;
}

int Timebar::draw() {
	if (time_to_live <= 0) {
		WM.markForDelete(this);
		return -1;
	}

	return df::Object::draw();
}