//
// Star.cpp
//

// System includes
#include <stdlib.h>

// Engine includes
#include "EventOut.h"
#include "DisplayManager.h"
#include "WorldManager.h"

// Game includes
#include "Star.h"

Star::Star() {
	// Draw Stars with circle
	// Closer Stars are bigger and move faster
	df::Shape s;
	s.setColor(df::WHITE);
	s.setType(df::CIRCLE);
	s.setSize(5 * getVelocity().getMagnitude());
	setShape(s);

	// Sets object type to Star
	setType("Star");

	// Makes the Star spectral so it can pass through objects 
	// and not impede movement
	setSolidness(df::SPECTRAL);

	// Draws the Star in the background
	setAltitude(0);

	// Sets position to random place in window
	df::Vector p((float)(rand() % (int)WM.getBoundary().getHorizontal()),
		(float)(rand() % (int)WM.getBoundary().getVertical()));
	setPosition(p);

	// Gives motion parallax effect for Star movement
	setVelocity(df::Vector((float)-1.0 / (rand() % 10 + 1), 0));
}

int Star::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}

	return 0;
}

// If Star moved off window, move back to far right
void Star::out() {
	df::Vector p((float)(WM.getBoundary().getHorizontal() + rand() % 20),
		(float)(rand() % (int)WM.getBoundary().getVertical()));
	setPosition(p);
	setVelocity(df::Vector(-1.0 / (rand() % 10 + 1), 0));

	// Draw Stars with circle
	// Closer Stars are bigger and move faster
	df::Shape s;
	s.setColor(df::WHITE);
	s.setType(df::CIRCLE);
	s.setSize(5 * getVelocity().getMagnitude());
	setShape(s);
}