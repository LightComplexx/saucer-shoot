//
// Hero.h
//

// Engine includes
#include "Object.h"
#include "ObjectList.h"
#include "TextBox.h"
#include "EventKeyboard.h"
#include "EventMouse.h"

// Game includes
#include "Reticle.h"

enum class Direction {
    Left = 0,
    Right,
    Up,
    Down
};

class Hero : public df::Object {

private:
    void kbd(const df::EventKeyboard* p_keyboard_event);
    void mouse(const df::EventMouse* p_mouse_event);
    void fire(df::Vector target);
    void slash();
    void move(int dy);
    void step();
    void nuke();
    void startSlash();
    void modifyDisplay(df::Keyboard::Key input);
    bool checkDirectionInput(df::Keyboard::Key input, int spot_in_string);
    std::string updateDirectionList(std::string type);
    std::string mapDirectionToString(Direction direc);
    std::string direction_list;
    int spot_in_direc_list;
    int move_slowdown;
    int move_countdown;
    int fire_slowdown;
    int fire_countdown;
    int nuke_count;
    bool can_slash;
    bool slash_state;
    Reticle* p_reticle;
    df::TextBox* comb_display;

public:
    Hero();
    ~Hero();
    int eventHandler(const df::Event* p_e) override;
};