//
// Explosion.cpp
//

// Engine includes
#include "WorldManager.h"
#include "EventStep.h"

// Game includes
#include "Explosion.h"
#include "Hero.h"

Explosion::Explosion() {
	// Links to "explosion" sprite
	if (setSprite("explosion") == 0)
		// Set live time as long as sprite length
		time_to_live = getAnimation().getSprite()->getFrameCount();
	else
		time_to_live = 0;

	// Sets object type to Explosion
	setType("Explosion");

	// Registers step events
	registerInterest(df::STEP_EVENT);

	// Makes the Explosion spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SPECTRAL);
}

int Explosion::eventHandler(const df::Event* p_e) {
	// Step events
	if (p_e->getType() == df::STEP_EVENT) {
		const df::EventStep* p_step_event =
			dynamic_cast <const df::EventStep*> (p_e);
		step();
		return 1;
	}
	return 0;
}

void Explosion::step() {
	time_to_live--;
	if (time_to_live <= 0)
		WM.markForDelete(this);
}