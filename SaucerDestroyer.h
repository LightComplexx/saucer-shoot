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
    df::ObjectList saucers;
    int destroy_interval;
    int delete_limit;
    int step_count;
    int deleted_count;

public:
    SaucerDestroyer(df::ObjectList filtered_saucers, int interval, int max_delete);
    int eventHandler(const df::Event* p_event) override;
};