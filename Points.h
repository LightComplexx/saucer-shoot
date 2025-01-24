//
// Points.h
//

// Engine includes
#include "Event.h"
#include "ViewObject.h"

#define POINTS_STRING "Points"

class Points : public df::ViewObject {
private:
    bool pause_step;

public:
    Points();
    int eventHandler(const df::Event* p_e) override;
};
