#include <iostream>
#include <chrono>

#include "egg.h"
#include "chicken.h"


// Define Egg
// transitive verb
//  : to incite to action - usually used with on
// noun, often attributive
// 1 a: the hard-shelled reproductive body produced by a bird and especially by the common domestic chicken; also : its contents used as food
//   b: an animal reproductive body consisting of an ovum together with its nutritive and protective envelopes and having the capacity to develop into a new individual capable of independent existence
//   c: ovum
// 2  : something resembling an egg
// 3  : person, sort; a good egg

Egg::Egg() {
  count = 0;
  spawn_time = std::chrono::steady_clock::now();
}

Egg::Egg(unsigned int n, std::chrono::time_point<std::chrono::steady_clock> t) {
  count = n;
  spawn_time = t;
}

unsigned int Egg::getCount() {
  return count;
}

std::chrono::time_point<std::chrono::steady_clock> Egg::getTime() {
  return spawn_time;
}

void Egg::setCount(unsigned int n) {
  count = n;
}

void Egg::setTime(std::chrono::time_point<std::chrono::steady_clock> t) {
  spawn_time = t;
}

class Chicken * Egg::hatch() {
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> diff = now - spawn_time;
        // Check to make sure that 100ms - 109ms have passed
        // Time should not exceed this window before eggs hatch
        if (diff.count() >= 100 && diff.count() <= 109) {
                spawn_time = now;
                class Chicken * e = new class Chicken(count, now);
                return e;
        } else if (diff.count() > 109) {
                std::cerr << "Egg did not hatch on time! (>109ms have passed)" << std::endl;
                return NULL;
        } else if (diff.count() < 100) {
                std::cerr << "Eggs are not ready to hatch! (<100ms have passed)" << std::endl;
                return NULL;
        }
}

float Egg::sell() {
        return sell(count);
}

float Egg::sell(unsigned int n) {
        // Can't sell more eggs than count
        // Return a negative number that represents the actual number of chickens available to sell
        // However, this should never get called because external checks happen before calling Egg::sell()
        if (n > count) {
                std::cerr << "Tried to sell more eggs than you have! Egg promises are worthless!" << std::endl;
                return -1*count;
        } else {
              count -= n;
              return 0.25 * (float)n;
        }
}
