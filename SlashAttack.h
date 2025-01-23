//
// SlashAttack.h
//

// Engine includes
#include "ViewObject.h"

class SlashAttack : public df::ViewObject {
public:
	SlashAttack(df::Vector location);
	~SlashAttack();
	int draw() override;
	int eventHandler(const df::Event* p_e);
};
