//
// GameOver.h
//

// Engine includes
#include "ViewObject.h"

class GameOver : public df::ViewObject {
private:
	void step();
	int time_to_live;

public:
	GameOver();
	~GameOver();
	int draw() override;
	int eventHandler(const df::Event* p_e);
};
