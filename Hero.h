//
// Hero.h
//

// Engine includes
#include "Object.h"
#include "EventKeyboard.h"
#include "EventMouse.h"

//Game includes
#include "Reticle.h"

class Hero : public df::Object {

private:
    void kbd(const df::EventKeyboard* p_keyboard_event);
    void mouse(const df::EventMouse* p_mouse_event);
    void fire(df::Vector target);
    void mark(df::Vector target);
    void move(int dy);
    void step();
    void nuke();
    void slash();
    int move_slowdown;
    int move_countdown;
    int fire_slowdown;
    int fire_countdown;
    int nuke_count;
    bool can_slash;
    bool slash_state;
    Reticle* p_reticle;

public:
    Hero();
    ~Hero();
    int eventHandler(const df::Event* p_e) override;
};