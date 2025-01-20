//
// Reticle.h
//

// Engine Includes
#include "Object.h"
#include "EventMouse.h"

#define RETICLE_CHAR '+'

class Reticle : public df::Object {
private:
	df::Color color;
	void mouse(const df::EventMouse* p_mouse_event);

public:
	Reticle(df::Color c);
	int draw(void) override;
	int eventHandler(const df::Event* p_e) override;
};