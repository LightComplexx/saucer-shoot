//
// Timebar.h
//

// Engine includes
#include "ViewObject.h"

class Timebar : public df::ViewObject {
private:
	int time_to_live;

public:
	Timebar();
	~Timebar();
	int draw() override;
	int eventHandler(const df::Event* p_e);
};