//
// Star.h
//

// Engine includes
#include "Object.h"

#define STAR_CHAR '.'

class Star : public df::Object {

private:
    df::Vector rememberVel;
    void out();

public:
    Star();
    int eventHandler(const df::Event* p_e) override;
};