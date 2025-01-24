//
// Hero.cpp
//

// System includes
#include <stdlib.h> // For rand()

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
#include "TextBox.h"

// Game includes
#include "Hero.h"
#include "Bullet.h"
#include "GameOver.h"
#include "Slash.h"
#include "SaucerDestroyer.h"

Hero::Hero() {
	// Link to "ship" sprite
	setSprite("ship");

	// Registers keyboard, mouse, step, and SlashEnd events 
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

	// Sets slowdown and countdown for slash
	slash_slowdown = 600; // default 600
	slash_countdown = slash_slowdown;

	// Sets the nuke count
	nuke_count = 1;

	// Sets the Hero's ability to use slash attack
	can_slash = false;
	slash_state = false;

	// Whether keys will register or not
	disable_input = false;

	// Initialize direction list to empty string
	direction_list = "";

	// Initialize variables for direction list
	spot_in_direc_list = 0;
	correct_dir_count = 0;

	// Initialize display
	comb_display = NULL;
	slash_notice = NULL;

	// Set can draw notice
	can_draw_notice = true;

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

	// Mark slash notice for deletion
	slash_notice->clearText();
	WM.markForDelete(slash_notice);
}

// Records keyboard and step events
int Hero::eventHandler(const df::Event* p_e) {
	// Keyboard events
	if (!disable_input && p_e->getType() == df::KEYBOARD_EVENT) {
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
	if (p_e->getType() == SLASHEND_EVENT) {
		slash(correct_dir_count);
		return 1;
	}
	return 0;
}

// Take appropriate action according to key pressed
void Hero::kbd(const df::EventKeyboard* p_keyboard_event) {
	// Actions to take during slash_state
	if (slash_state) {
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::W:    // up
			if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
				modifyDisplay(df::Keyboard::W);
			break;

		case df::Keyboard::S:    // down
			if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
				modifyDisplay(df::Keyboard::S);
			break;

		case df::Keyboard::A:	// left
			if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
				modifyDisplay(df::Keyboard::A);
			break;

		case df::Keyboard::D:	// right
			if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
				modifyDisplay(df::Keyboard::D);
			break;
		}
	}
	// Actions to take outside slash_state
	else {
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
}

void Hero::mouse(const df::EventMouse* p_mouse_event) {
	// Fire bullets if not in slash_state
	if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
		(p_mouse_event->getMouseButton() == df::Mouse::LEFT)) {
		if (!slash_state)
			fire(p_mouse_event->getMousePosition());
	}
	// Start slash_state if available
	if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
		(p_mouse_event->getMouseButton() == df::Mouse::RIGHT))
		startSlash();
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

	// Slash countdown
	if (!slash_state)
		slash_countdown--;

	if (slash_countdown < 0) {
		// Update slash notice when available
		if (can_draw_notice) {
			slash_notice = new df::TextBox();
			slash_notice->setPosition(df::Vector(WM.getBoundary().getHorizontal() / 2, 2));
			slash_notice->setViewString("notice");
			slash_notice->setSize(df::Vector(28, 1));
			slash_notice->setText("Dimensional Slash Available!");
			slash_notice->setColor(df::YELLOW);

			can_draw_notice = false;
		}
		can_slash = true;
		slash_countdown = 0;
	}
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

void Hero::startSlash() {
	// Check if player can slash
	if (!can_slash)
		return;

	// Disable slash notice
	slash_notice->clearText();
	WM.markForDelete(slash_notice);

	// Disable input
	disable_input = true;

	// Set Slash_state to true
	slash_state = true;

	// Count visible saucers and assign direction
	std::string curr_list = updateDirectionList("Saucer");

	// Then display button combination
	comb_display = new df::TextBox;
	comb_display->setLocation(df::CENTER_CENTER);
	comb_display->setViewString("Combination List");
	comb_display->setSize(df::Vector(50, 1 + (1 * (curr_list.length() > 50))));
	comb_display->setText(curr_list);
	comb_display->setColor(df::YELLOW);

	// Create new "Slash" object and draw text
	Slash* slashtext = new Slash();
	slashtext->draw();

	// Create "Slash" event and send to interested Objects
	EventSlash slash;
	WM.onEvent(&slash);

	// Make a big explosion with particles.
	df::addParticles(df::RINGS, df::Vector(getPosition().getX(), getPosition().getY() + (getBox().getVertical() / 4)), 2, df::GREEN);

	// Disable ability to slash
	can_slash = false;
}

void Hero::slash(int num_saucers) {
	// Destroy saucers with 5 step interval
	new SaucerDestroyer(filtered_saucers, 5, correct_dir_count);

	// Reinitialize all interested variables
	slash_state = false;
	disable_input = false;
	slash_countdown = slash_slowdown;
	correct_dir_count = 0;
	spot_in_direc_list = 0;
	filtered_saucers.clear();
	direction_list.clear();
	can_draw_notice = true;

	// Clear direction list display
	comb_display->clearText();
	WM.markForDelete(comb_display);
}

void Hero::modifyDisplay(df::Keyboard::Key input) {
	// Checks player input against direction list
	if (checkDirectionInput(input, spot_in_direc_list)) {
		// Display '00' for correct input
		correct_dir_count++;
		direction_list[spot_in_direc_list] = 'O';
		direction_list[spot_in_direc_list + 1] = 'O';
		
		// Play "input success" sound
		df::Sound* p_sound = RM.getSound("input success");
		if (p_sound) {
			p_sound->play();
		}

		// Go to next entry if it exists
		if (spot_in_direc_list + 6 < direction_list.length())
			spot_in_direc_list += 6;
		// If not, disable input
		else {
			comb_display->setColor(df::GREEN);
			disable_input = true;
			// Disable collisions for saucers to be destroyed
			for (size_t i = 0; i < correct_dir_count; i++)
			{
				filtered_saucers[i]->setSolidness(df::SPECTRAL);
			}
		}
	}
	else {
		// Display 'XX' for incorrect input
		direction_list[spot_in_direc_list] = 'X';
		direction_list[spot_in_direc_list + 1] = 'X';
		comb_display->setColor(df::RED);
		// Disable input immediately
		disable_input = true;
		// Play "input fail" sound
		df::Sound* p_sound = RM.getSound("input fail");
		if (p_sound) {
			p_sound->play();
		}

		// Disable collisions for saucers to be destroyed
		for (size_t i = 0; i < correct_dir_count; i++)
		{
			filtered_saucers[i]->setSolidness(df::SPECTRAL);
		}
	}

	// Update display
	comb_display->setText(direction_list);
}

std::string Hero::updateDirectionList(std::string type) {
	df::ObjectList allObjects = WM.getAllObjects();
	df::ObjectListIterator it(&allObjects);

	float world_horiz = WM.getBoundary().getHorizontal();

	// Filters objects by saucers within in the visible play area
	while (!it.isDone() && filtered_saucers.getCount() < 15) {
		df::Object* obj = it.currentObject();
		if (obj->getType() == type && obj->getPosition().getX() < world_horiz) {
			// Assign random direction
			Direction direc = Direction(rand() % 4);
			direction_list.append(mapDirectionToString(direc) + " || ");

			// Add the filtered saucer to the new ObjectList
			filtered_saucers.insert(obj);
		}
		it.next();
	}
	// Returns list of directions
	return direction_list;
}

std::string Hero::mapDirectionToString(Direction direc) {
	// Maps direction enum to string identifier
	switch (direc) {
	case Direction::Left: {
		return "<-";
	}
	case Direction::Right: {
		return "->";
	}
	case Direction::Down: {
		return "vv";
	}
	case Direction::Up: {
		return "^^";
	}
	}
}

bool Hero::checkDirectionInput(df::Keyboard::Key input, int spot_in_string) {
	// Checks key input against string identifier
	switch (input) {
	case df::Keyboard::A: { // Left
		return direction_list[spot_in_string] == '<';
	}
	case df::Keyboard::D: { // Right
		return direction_list[spot_in_string] == '-';
	}
	case df::Keyboard::S: { // Down
		return direction_list[spot_in_string] == 'v';
	}
	case df::Keyboard::W: { // Up
		return direction_list[spot_in_string] == '^';
	}
	}
}

bool Hero::getDisableInput() {
	return disable_input;
}

void Hero::setDisableInput(bool state) {
	disable_input = state;
}