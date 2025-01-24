//
// SaucerDestroyer.cpp
//

// Engine includes
#include "DisplayManager.h"
#include "ResourceManager.h"
#include "EventView.h"

// Game includes
#include "SaucerDestroyer.h"
#include "SlashAttack.h"
#include "Explosion.h"
#include "Saucer.h"
#include "Points.h"

SaucerDestroyer::SaucerDestroyer(df::ObjectList filtered_saucers, int interval, int max_delete) {
	// Set type to SaucerDestroyer
	setType("SaucerDestroyer");

	// Intialize saucer list
	saucers = filtered_saucers;

	// Initialize time interval for destruction
	destroy_interval = interval;

	// Initialize deletion limit and step count
	delete_limit = max_delete;
	step_count = 0;
	deleted_count = 0;

	// Register for step events.
	registerInterest(df::STEP_EVENT);
}

int SaucerDestroyer::eventHandler(const df::Event* p_event) {
	if (p_event->getType() == df::STEP_EVENT) {
		// Increase step counter
		step_count++;
		// Delete saucers 
		if (step_count >= destroy_interval && !saucers.isEmpty() && deleted_count < delete_limit) {
			// Iterator for saucer list
			df::ObjectListIterator it(&saucers);
			if (!it.isDone()) {
				// Create slash attack at saucer location
				new SlashAttack(it.currentObject()->getPosition());

				// Play "input success" sound
				df::Sound* p_sound = RM.getSound("sword slash");
				if (p_sound) {
					p_sound->play();
				}

				// Create an explosion
				Explosion* p_explosion = new Explosion;
				p_explosion->setPosition(it.currentObject()->getPosition());

				// Play "explosion" sound
				df::Sound* e_sound = RM.getSound("explode");
				if (e_sound) {
					e_sound->play();
				}

				// Send "view" event with points to interested ViewObjects.
				// Add 40 extra points.
				df::EventView ev(POINTS_STRING, 40, true);
				WM.onEvent(&ev);

				// Shake screen (severity 5 pixels x&y, duration 5 frames).
				DM.shake(5, 5, 5);

				// Mark saucer for deletion
				WM.markForDelete(it.currentObject());
				saucers.remove(it.currentObject());

				// Reset counter for next destruction
				step_count = 0;
				deleted_count++;
			}
		}

		// Destroy the controller when done
		if (deleted_count >= delete_limit || saucers.isEmpty()) {
			// Create 5 saucers with 2x speed before destroying self
			for (size_t i = 0; i < 5; i++)
			{
				new Saucer(-0.5);
			}
			WM.markForDelete(this);
		}
		return 1;
	}
	return 0;
}
