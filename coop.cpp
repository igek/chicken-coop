#include <list>
#include <chrono>
#include <cmath>

#include "chicken.h"
#include "coop.h"

// Forward declare the Chicken class
class Chicken;

// Define Coop
// From the Merriam-Webster Dictionary:
// 1  : a cage or small enclosure (as for poultry); also: a small building for housing poultry
// 2 a: a confined area
//   b: jail
// 3  : to confine in a restricted and often crowded area - usually used with up
// 4  : to place or keep in a coop - often used with up

Coop::Coop() {
        broods = new std::list<class Chicken *>;
        count = 0;
        upkeep_estimate = 0;
        revenue_estimate = 0;
}

Coop::Coop(std::list<class Chicken *> * v) {
        broods = v;
        count = 0;
        upkeep_estimate = 0;

        for (std::list<class Chicken *>::const_iterator it = broods->begin(); it != broods->end(); ++it) {
            unsigned int temp = (*it)->getCount();
            count += temp;
            updateUpkeepEstimate(temp);
        }

        revenue_estimate = 10 * (float)count;
}

void Coop::updateUpkeepEstimate(unsigned int c) {
        for (unsigned int i = c; i > 0; i--) {
                if (count <= 5 && i <= 5) {
                        upkeep_estimate += 2;
                } else {
                        upkeep_estimate += 5*(floor((count + i)/5));
                }
        }
}

unsigned int Coop::getCount() {
        return count;
}

std::list<class Chicken *> * Coop::getList() {
        return broods;
}

float Coop::getUpkeepEstimate() {
        return upkeep_estimate;
}

float Coop::getRevenueEstimate() {
        return revenue_estimate;
}

void Coop::push(class Chicken * c) {
        unsigned int temp_count = c->getCount();
        broods->push_back(c);
        count += temp_count;
        updateUpkeepEstimate(temp_count);
        revenue_estimate += 10 * temp_count;

}

float Coop::sell() {
        return sell(count - 25);
}

float Coop::sell(unsigned int n) {
        unsigned int to_sell = n;
        std::list<class Chicken *>::const_iterator it = broods->begin();
        unsigned int current_count;
        float revenue = 0;

        while (to_sell > 0 && it != broods->end()) {
                current_count = (*it)->getCount();
                if (to_sell > current_count) {
                        to_sell -= current_count;
                        count -= current_count;
                        revenue += (*it)->sell();
                        it = broods->erase(it);
                } else if (to_sell < current_count) {
                        count -= to_sell;
                        revenue += (*it)->sell(to_sell);
                        to_sell = 0;
                } else if (to_sell == current_count) {
                        count -= to_sell;
                        revenue += (*it)->sell(to_sell);
                        to_sell = 0;
                        it = broods->erase(it);
                }
        }

        return revenue;
}
