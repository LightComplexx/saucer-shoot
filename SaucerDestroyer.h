//
// SaucerDestroyer.h
//

// Engine includes
#include "WorldManager.h"
#include "EventStep.h"
#include "GameManager.h"
#include "ObjectList.h"
#include "ObjectListIterator.h"

class SaucerDestroyer : public df::Object {
private:
    df::ObjectList saucers; // List of saucer objects
    int destroy_interval;  // Steps between destruction
    int max_num;           // max number to delete
    int step_count;        // Tracks game steps

public:
    SaucerDestroyer(df::ObjectList filtered_saucers, int interval, int max_delete);
    int eventHandler(const df::Event* p_event) override;
};