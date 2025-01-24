//
// SaucerDestroyer.cpp
//

// Engine includes
#include "DisplayManager.h"

// Game includes
#include "SaucerDestroyer.h"
#include "SlashAttack.h"
#include "Explosion.h"
#include "Saucer.h"

SaucerDestroyer::SaucerDestroyer(df::ObjectList filtered_saucers, int interval, int max_delete) {
	// Set type to SaucerDestroyer
	setType("SaucerDestroyer");

	// Intialize saucer list
	saucers = filtered_saucers;

	// Initialize time interval for destruction
	destroy_interval = interval;

	// Initialize deletion limit and step count
	max_num = max_delete;
	step_count = 0;

	// Register for step events.
	registerInterest(df::STEP_EVENT);
}

int SaucerDestroyer::eventHandler(const df::Event* p_event) {
	// Counter for deleted saucers
	int deleted_count = 0;

	if (p_event->getType() == df::STEP_EVENT) {
		// Increase step counter
		step_count++;
		// Delete saucers 
		if (step_count >= destroy_interval && !saucers.isEmpty() && deleted_count < max_num) {
			// Iterator for saucer list
			df::ObjectListIterator it(&saucers);
			if (!it.isDone()) {
				// Create slash attack at saucer location
				new SlashAttack(it.currentObject()->getPosition());

				// Create an explosion
				Explosion* p_explosion = new Explosion;
				p_explosion->setPosition(it.currentObject()->getPosition());

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
		if (deleted_count >= max_num || saucers.isEmpty()) {
			// Create 5 saucers before destroying self
			for (size_t i = 0; i < 5; i++)
			{
				new Saucer;
			}
			WM.markForDelete(this);
		}
		return 1;
	}
	return 0;
}
