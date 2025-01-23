//
// Hero.cpp
//

// Engine includes
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "ResourceManager.h"
#include "EventStep.h"
#include "EventNuke.h"
#include "EventSlash.h"
#include "EventSlashEnd.h"
#include "EventView.h"

// Game includes
#include "Hero.h"
#include "Bullet.h"
#include "GameOver.h"
#include "Slash.h"
#include "SlashAttack.h"

Hero::Hero() {
	// Link to "ship" sprite
	setSprite("ship");

	// Registers keyboard, mouse, step, and slash events 
	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::STEP_EVENT);
	registerInterest(df::MSE_EVENT);
	registerInterest(SLASHEND_EVENT);

	// Sets object type to Hero 
	setType("Hero");

	// Sets position to the left of the window
	df::Vector p(7, WM.getBoundary().getVertical() / 2);
	setPosition(p);

	// Sets the slowdown and countdown time for moving the Hero
	move_slowdown = 2;
	move_countdown = move_slowdown;

	// Sets the slowdown and countdown time for bullets
	fire_slowdown = 7;
	fire_countdown = fire_slowdown;

	// Sets the nuke count
	nuke_count = 1;

	// Sets the Hero's ability to use slash attack 
	can_slash = true;
	slash_state = false;

	// Create reticle for firing bullets
	p_reticle = new Reticle(df::RED);
	p_reticle->draw();
}

Hero::~Hero() {
	// Destroys Hero and ends game
	// Create GameOver object
	new GameOver;

	// Shake screen (severity 20 pixels x&y, duration 10 frames).
	DM.shake(20, 20, 10);

	// Make a big explosion with particles.
	df::addParticles(df::SPARKS, getPosition(), 2, df::BLUE);
	df::addParticles(df::SPARKS, getPosition(), 2, df::YELLOW);
	df::addParticles(df::SPARKS, getPosition(), 3, df::RED);
	df::addParticles(df::SPARKS, getPosition(), 3, df::RED);

	// Mark Reticle for deletion
	WM.markForDelete(p_reticle);
}

// Records keyboard and step events
int Hero::eventHandler(const df::Event* p_e) {
	// Keyboard events
	if (!slash_state && p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard* p_keyboard_event =
			dynamic_cast <const df::EventKeyboard*> (p_e);
		kbd(p_keyboard_event);
		return 1;
	}

	// Mouse events
	if (p_e->getType() == df::MSE_EVENT) {
		const df::EventMouse* p_mouse_event =
			dynamic_cast <const df::EventMouse*> (p_e);
		mouse(p_mouse_event);
		return 1;
	}

	// Step events
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// SlashEnd events
	if (slash_state && p_e->getType() == SLASHEND_EVENT) {
		slash_state = false;
		return 1;
	}
	return 0;
}

// Take appropriate action according to key pressed
void Hero::kbd(const df::EventKeyboard* p_keyboard_event) {

	switch (p_keyboard_event->getKey()) {
	case df::Keyboard::Q:	// quit
		if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
			WM.markForDelete(this);
		break;

	case df::Keyboard::W:    // up
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
			move(-1);
		break;

	case df::Keyboard::S:    // down
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
			move(+1);
		break;

	case df::Keyboard::SPACE:    // nuke
		if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
			nuke();
		break;
	}
}

void Hero::mouse(const df::EventMouse* p_mouse_event) {
	// Pressed button?
	if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
		(p_mouse_event->getMouseButton() == df::Mouse::LEFT)) {
		if (slash_state) {
			mark(p_mouse_event->getMousePosition());
		}
		else
			fire(p_mouse_event->getMousePosition());
	}

	if (can_slash && (p_mouse_event->getMouseAction() == df::CLICKED) &&
		(p_mouse_event->getMouseButton() == df::Mouse::RIGHT))
		slash();
}

// Move up or down
void Hero::move(int dy) {
	// See if time to move
	if (move_countdown > 0)
		return;
	move_countdown = move_slowdown;

	// If stays on window, allow move
	df::Vector new_pos(getPosition().getX(), getPosition().getY() + dy);
	if ((new_pos.getY() > 3) &&
		(new_pos.getY() < WM.getBoundary().getVertical() - (getBox().getVertical() / 2)))
		WM.moveObject(this, new_pos);
}

// Decrease rate restriction counters
void Hero::step() {
	// Move countdown
	move_countdown--;
	if (move_countdown < 0)
		move_countdown = 0;

	// Fire countdown
	fire_countdown--;
	if (fire_countdown < 0)
		fire_countdown = 0;
}

void Hero::fire(df::Vector target) {
	if (fire_countdown > 0)
		return;
	fire_countdown = fire_slowdown;

	// Fire Bullet towards target
	// Compute normalized vector to position, then scale by speed (1)
	df::Vector v = target - getPosition();
	v.normalize();
	v.scale(1);
	Bullet* p = new Bullet(getPosition());
	p->setVelocity(v);

	// Play "fire" sound
	df::Sound* p_sound = RM.getSound("fire");
	if (p_sound) {
		p_sound->play();
	}
}

void Hero::nuke() {
	// Check if nukes left
	if (!nuke_count)
		return;
	nuke_count--;

	// Create "nuke" event and send to interested Objects
	EventNuke nuke;
	WM.onEvent(&nuke);

	// Send "view" event with nukes to interested ViewObjects
	df::EventView ev("Nukes", -1, true);
	WM.onEvent(&ev);

	// Shake screen (severity 15 pixels x&y, duration 5 frames).
	DM.shake(15, 15, 5);

	// Play "nuke" sound
	df::Sound* p_sound = RM.getSound("nuke");
	if (p_sound)
		p_sound->play();
}

void Hero::slash() {
	// Set chain_state to true
	slash_state = true;

	// Create new "Chain" object and draw text
	Slash* slashtext = new Slash();
	slashtext->draw();

	// Create "chain" event and send to interested Objects
	EventSlash slash;
	WM.onEvent(&slash);

	// Shake screen (severity 10 pixels x&y, duration 5 frames).
	//DM.shake(10, 10, 5);

	// Make a big explosion with particles.
	df::addParticles(df::RINGS, df::Vector(getPosition().getX(), getPosition().getY() + (getBox().getVertical() / 4)), 2, df::GREEN);

	// Sets can_chain to false
	can_slash = false;
}

void Hero::mark(df::Vector loc) {
	SlashAttack* slashAtk = new SlashAttack(loc);
	slashAtk->draw();
}