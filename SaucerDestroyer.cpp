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

SaucerDestroyer::SaucerDestroyer(df::ObjectList filtered_saucers, int interval, int max_delete) { // 30 steps ~ 1 second (assuming 30 FPS)
	setType("SaucerDestroyer");
	saucers = filtered_saucers;
	destroy_interval = interval;
	max_num = max_delete;
	step_count = 0;

	// Register for step events.
	registerInterest(df::STEP_EVENT);
}

int SaucerDestroyer::eventHandler(const df::Event* p_event) {
	int deleted_count = 0;

	if (p_event->getType() == df::STEP_EVENT) {
		step_count++;

		if (step_count >= destroy_interval && !saucers.isEmpty() && deleted_count < max_num) {
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
