//
// SlashAttack.h
//

// Engine includes
#include "ViewObject.h"

class SlashAttack : public df::ViewObject {
private:
	int time_to_live;

public:
	SlashAttack(df::Vector location);
	int draw();
	int eventHandler(const df::Event* p_e);
};
