//
// Slash.cpp
//

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "EventStep.h"

// Game includes
#include "Slash.h"
#include "Reticle.h"
#include "Timebar.h"

Slash::Slash() {
	// Sets object to type Slash
	setType("Slash");

	// Sets sprite for 'Dimensional Slash' text
	if (setSprite("slashtext") == 0)
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown() * 4;
	else
		time_to_live = 0;

	// Makes the Slash text spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SPECTRAL);

	// Draws the Slash text in the foreground
	setAltitude(df::MAX_ALTITUDE);

	// Sets location to top center of window
	setLocation(df::CENTER_CENTER);

	// Registers step event
	registerInterest(df::STEP_EVENT);
}

Slash::~Slash() {
	// Draw timebar once text disappears
	Timebar* time = new Timebar();
	time->draw();
}

int Slash::eventHandler(const df::Event* p_e) {
	// Life countdown
	if (p_e->getType() == df::STEP_EVENT) {
		time_to_live--;
		return 1;
	}

	return 0;
}

int Slash::draw() {
	// Flash text until time delay ends
	if (time_to_live <= 0) {
		WM.markForDelete(this);
		return -1;
	}
	return df::Object::draw();
}
