//
// Slash.h
//

// Engine includes
#include "ViewObject.h"

class Slash : public df::ViewObject {
private:
	int time_to_live;

public:
	Slash();
	~Slash();
	int draw() override;
	int eventHandler(const df::Event* p_e) override;
};