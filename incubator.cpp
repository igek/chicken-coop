#include <list>
#include <chrono>

#include "egg.h"
#include "incubator.h"

// Define Incubator
// From the Merriam-Webster Dictionary
// 1  : to grasp or hold with or as it with the hand or claws usually strongly, tightly, or suddenly
// 2  : [obsolete] clench
// 3  : to seek to grasp and hold
// 4  : to operate and automobile clutch
// 5 a: the claws or a hand in the act of grasping or seizing firmly
//   b: an often cruel or unrelenting control, power, or possession
//   c: the act of grasping, holding, or restraining
// 6 a: a coupling used to connect and disconnect a driving and a driven part (such as an engine and a transmission of a mechanism
//   b: a level (such as a pedal) operating such a clutch
// 7  : a tight or critical situation
// 8  : clutch bag
// 9  : made or done in a crucial situation
// 10 : successful in a crucial situation
// 11 : a nest of eggs or a brood of chicks
// 12 : group, bunch

Incubator::Incubator() {
        clutches = new std::list<class Egg *>;
        count = 0;
        upkeep_estimate = 0;
        revenue_estimate = 0;
}

Incubator::Incubator(std::list<class Egg *> * v) {
        clutches = v;
        count = 0;
        
        for (std::list<class Egg *>::const_iterator it = clutches->begin(); it != clutches->end(); ++it) {
                unsigned int temp = (*it)->getCount();
                count += temp;
        }

        revenue_estimate = 0.25 * (float)count;
        upkeep_estimate = 0.05 * (float)count;
}

unsigned int Incubator::getCount() {
        return count;
}

float Incubator::getUpkeepEstimate() {
        return upkeep_estimate;
}

float Incubator::getRevenueEstimate() {
        return revenue_estimate;
}

void Incubator::push(class Egg * e) {
        unsigned int temp_count = e->getCount();
        clutches->push_back(e);
        count += temp_count;
        upkeep_estimate += 0.05 * temp_count;
        revenue_estimate += 0.25 * temp_count;
}

std::list<class Chicken *> * Incubator::hatchAll() {
        std::list<class Chicken *> * c = new std::list<class Chicken *>();
        class Chicken * temp = NULL;

        for (std::list<class Egg *>::const_iterator it = clutches->begin(); it != clutches->end(); ) {
            temp = (*it)->hatch();
            if (temp != NULL) {
                    c->push_back(temp);
                    it = clutches->erase(it);
            } else { ++it; }
        }
        return c;
}

float Incubator::sell(unsigned int n) {
        unsigned int to_sell = n;
        std::list<class Egg *>::const_iterator it = clutches->begin();
        unsigned int current_count;
        float revenue = 0;

        while (to_sell > 0 && it != clutches->end()) {
                current_count = (*it)->getCount();
                if (to_sell > current_count) {
                        to_sell -= current_count;
                        count -= current_count;
                        revenue += (*it)->sell();
                        it = clutches->erase(it);
                } else if (to_sell < current_count) {
                        count -= to_sell;
                        revenue += (*it)->sell(to_sell);
                        to_sell = 0;
                } else if (to_sell == current_count) {
                        count -= to_sell;
                        revenue += (*it)->sell(to_sell);
                        to_sell = 0;
                        it = clutches->erase(it);
                }
        }

        return revenue;
}
