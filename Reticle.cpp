//
// Reticle.cpp
//

// Engine includes
#include "DisplayManager.h"
#include "WorldManager.h"
#include "EventCollision.h"

// Game includes
#include "Reticle.h"

Reticle::Reticle(df::Color c) {
	// Sets object type to Reticle
	setType("Reticle");

	// Makes the Reticle spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SOFT);

	// Draws the Reticle in the foreground
	setAltitude(df::MAX_ALTITUDE);

	// Registers mouse events
	registerInterest(df::MSE_EVENT);

	// Sets location to center of window
	df::Vector p(WM.getBoundary().getHorizontal() / 2, WM.getBoundary().getVertical() / 2);
	setPosition(p);

	// Initiate color variable
	color = c;
}

int Reticle::eventHandler(const df::Event* p_e) {
	// Mouse events
	if (p_e->getType() == df::MSE_EVENT) {
		const df::EventMouse* p_mouse_event =
			dynamic_cast <const df::EventMouse*> (p_e);

		mouse(p_mouse_event);
		return 1;
	}

	// If get here, have ignored this event
	return 0;
}

// Draw reticle on window
int Reticle::draw() {
	return DM.drawCh(getPosition(), RETICLE_CHAR, color);
}

void Reticle::mouse(const df::EventMouse* p_mouse_event) {
	if (p_mouse_event->getMouseAction() == df::MOVED) {
		// Change location to new mouse position
		setPosition(p_mouse_event->getMousePosition());
	}
}

