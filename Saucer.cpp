//
// Saucer.cpp
//

// System includes
#include <stdlib.h> // For rand()

// Engine includes
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"
#include "EventOut.h"
#include "EventNuke.h"
#include "EventSlash.h"
#include "EventSlashEnd.h"
#include "EventView.h"

// Game includes
#include "Saucer.h"
#include "Explosion.h"
#include "Points.h"

Saucer::Saucer(float speed) {
	// Setup "saucer" sprite
	setSprite("saucer");

	// Sets object type
	setType("Saucer");

	// Registers Nuke, Slash and SlashEnd events
	registerInterest(NUKE_EVENT);
	registerInterest(SLASH_EVENT);
	registerInterest(SLASHEND_EVENT);

	// Sets speed in horizontal direction
	m_speed = speed;
	setVelocity(df::Vector(m_speed, 0)); // 1 space left every 4 frames

	// Sets starting location in the middle of window
	moveToStart();

	// Initiate slash state
	slash_state = false;
}

Saucer::~Saucer() {
	// Send "view" event with points to interested ViewObjects.
	// Add 10 points.
	df::EventView ev(POINTS_STRING, 10, true);
	WM.onEvent(&ev);
}

int Saucer::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == SLASH_EVENT) {
		setVelocity(df::Vector(0, 0));
	}

	if (p_e->getType() == SLASHEND_EVENT) {
		if (!slash_state) {
			setVelocity(df::Vector(m_speed, 0));
		}
	}

	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}

	if (collisions_active && p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event =
			dynamic_cast <const df::EventCollision*> (p_e);
		hit(p_collision_event);
		return 1;
	}

	if (p_e->getType() == NUKE_EVENT) {
		// Create an explosion
		Explosion* p_explosion = new Explosion;
		p_explosion->setPosition(this->getPosition());
		WM.markForDelete(this);
		// Saucers appear to stay around perpetually
		new Saucer;
		return 1;
	}

	return 0;
}

void Saucer::out() {
	if (getPosition().getX() >= 0)
		return;
	else {
		// Increase saucer speed if player fails to kill
		setVelocity(df::Vector(m_speed - 0.15, 0));
		moveToStart();

		// Spawn new Saucer to make the game get harder
		new Saucer;
	}
}

void Saucer::moveToStart() {
	df::Vector temp_pos;

	float world_horiz = WM.getBoundary().getHorizontal();
	float world_vert = WM.getBoundary().getVertical();

	// x is off right side of window
	temp_pos.setX(world_horiz + rand() % (int)world_horiz + 3.0f);

	// y is in vertical range
	temp_pos.setY(rand() % (int)(world_vert - 6) + 3.0f);

	// If collision, move right slightly until empty space
	df::ObjectList collision_list = WM.getCollisions(this, temp_pos);
	while (!collision_list.isEmpty()) {
		temp_pos.setX(temp_pos.getX() + 1);
		collision_list = WM.getCollisions(this, temp_pos);
	}

	// Moves object to specified position
	WM.moveObject(this, temp_pos);
}

void Saucer::hit(const df::EventCollision* p_c) {
	// If Saucer on Saucer, ignore
	if ((p_c->getObject1()->getType() == "Saucer") &&
		(p_c->getObject2()->getType() == "Saucer"))
		return;

	// If Bullet...
	if ((p_c->getObject1()->getType() == "Bullet") ||
		(p_c->getObject2()->getType() == "Bullet")) {

		// Create an explosion
		Explosion* p_explosion = new Explosion;
		p_explosion->setPosition(this->getPosition());

		// Saucers appear stay around perpetually
		new Saucer;

		// Play "explode" sound
		df::Sound* p_sound = RM.getSound("explode");
		if (p_sound)
			p_sound->play();
	}

	// If Hero, mark both objects for destruction
	if (((p_c->getObject1()->getType()) == "Hero") ||
		((p_c->getObject2()->getType()) == "Hero")) {
		WM.markForDelete(p_c->getObject1());
		WM.markForDelete(p_c->getObject2());
	}
}

void Saucer::disableCollisions() {
	collisions_active = false;
}

