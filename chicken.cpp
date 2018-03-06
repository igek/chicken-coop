#include <iostream>
#include <chrono>
#include "chicken.h"
#include "egg.h"

// Define Chicken
// From the Merriam-Webster Dictionary:
// 1 a: the common domestic fowl (Gallus gallus) especially when young; also : its flesh used as food
//   b: any of the various birds or their young
// 2  : a young women
// 3 a: coward
//   b: any of various contests in which the participants risk personal safety in order to see which one will give up first
// 4  : short for chickenshit
//    : slang : petty details
// 5  : slang : a young male homosexual

Chicken::Chicken() {
	count = 0;
	spawn_time = std::chrono::steady_clock::now();
}

Chicken::Chicken(unsigned int n, std::chrono::time_point<std::chrono::steady_clock> t) {
	count = n;
	spawn_time = t;
}

unsigned int Chicken::getCount() {
	return count;
}

std::chrono::time_point<std::chrono::steady_clock> Chicken::getTime() {
	return spawn_time;
}

void Chicken::setCount(unsigned int n) {
	count = n;
}

void Chicken::setTime(std::chrono::time_point<std::chrono::steady_clock> t) {
	spawn_time = t;
}

class Egg * Chicken::layEggs() {
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> diff = now - spawn_time;
	// Check to make sure that 10ms - 14ms have passed
	// Time should not exceed this window before chickens lay eggs
	if (diff.count() >= 10 && diff.count() <= 14) {
		// Reset the spawn_time
		spawn_time = now;
		class Egg * e = new class Egg(count, now);
		return e;
	} else if (diff.count() > 14) {
		std::cerr << "Chicken did not lay eggs on time! (>14ms have passed)" << std::endl;
		return NULL;
	} else {
		std::cerr << "Chickens are not ready to lay eggs! (<10ms have passed)" << std::endl;
		return NULL;
	}
}

float Chicken::sell() {
       return sell(count);
}

float Chicken::sell(unsigned int n) {
	// Can't sell more chickens than count
	// Return a negative number that represents the actuall number of chickens available to sell
	// However, this should never get called because external checks happen before calling Chicken::sell()
	if (n > count) {
		std::cerr << "Tried to sell more chickens than you have! Chicken promises are worthless!" << std::endl;
		return -1*count;
	} else {
		count -= n;
		return 10 * (float)n;
	}
}
