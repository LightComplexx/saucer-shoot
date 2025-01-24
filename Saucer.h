//
// Saucer.h
//

// Engine includes
#include "Object.h"
#include "EventCollision.h"

#define SAUCER_SPEED "Saucer Speed"

class Saucer : public df::Object {
private:
	void moveToStart();
	void out();
	void hit(const df::EventCollision* p_c);
	bool slash_state;
	bool collisions_active;
	float m_speed;

public:
	Saucer();
	~Saucer();
	int eventHandler(const df::Event* p_e) override;
	void disableCollisions();
};
