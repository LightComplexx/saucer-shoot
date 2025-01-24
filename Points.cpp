//
// Points.cpp
//

// Engine includes
#include "GameManager.h"
#include "WorldManager.h"
#include "EventStep.h"
#include "EventView.h"
#include "EventSlash.h"
#include "EventSlashEnd.h"

// Game includes
#include "Points.h"

Points::Points() {
	// Sets points to display at top right of window
	setLocation(df::TOP_RIGHT);

	// Sets POINTS_STRING to be displayed
	setViewString(POINTS_STRING);

	// Sets the text to the color yellow
	setColor(df::YELLOW);

	// Need to update score each second, so count "step" events
	registerInterest(df::STEP_EVENT);
	registerInterest(SLASH_EVENT);
	registerInterest(SLASHEND_EVENT);

	// Intialize pause step to false
	pause_step = false;
}

int Points::eventHandler(const df::Event* p_e) {
	// Parent handles event if score update
	if (df::ViewObject::eventHandler(p_e)) {
		return 1;
	}

	// If step and not slash event, increment score every second (30 steps)
	if (!pause_step && p_e->getType() == df::STEP_EVENT) {
		if (dynamic_cast <const df::EventStep*> (p_e)
			->getStepCount() % 30 == 0)
			setValue(getValue() + 1);
		return 1;
	}
	// If slash event, stop incrementing
	if (p_e->getType() == SLASH_EVENT) {
		pause_step = true;
		return 1;
	}
	// If slash event, start incrementing
	if (p_e->getType() == SLASHEND_EVENT) {
		pause_step = false;
		return 1;
	}
	return 0;
}