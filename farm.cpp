#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <fstream>
#include <list>
#include <cmath>

// Include Chicken and Egg class files
#include "chicken.h"
#include "egg.h"
#include "coop.h"
#include "incubator.h"

int main (int argc, char const * argv[]) {
  // Setup Timers
  std::chrono::time_point<std::chrono::steady_clock> start_time, now, end_time;
  std::chrono::time_point<std::chrono::steady_clock> prev_chicken, prev_egg, prev_sell, prev_upkeep;
  std::chrono::duration<double> diff, diff_chicken, diff_egg, diff_sell, diff_upkeep;
  start_time = std::chrono::steady_clock::now();

  // Setup File I/O
  std::string const read_filename = "StarterFarm.txt";
  std::string const write_filename = "FinalFarm.txt";
  std::fstream read_file(read_filename, read_file.in);
  std::fstream write_file(write_filename, write_file.out);

  // Variables to initialize the farm
  std::string string_chicken, string_eggs, string_debt, string_goal;
  unsigned int starter_chickens, starter_eggs, amount_of_allowable_debt, profit_goal;
  unsigned int days = 0;
  float farm_cash = 0.0;
  float cash_difference = 0.0;

  // Read in given variables and start time
  if (!read_file.is_open()) {
  std::cerr << "Failed to open " << read_filename << std::endl;
  } else {
  // Read the file into variables
  read_file >> starter_chickens;
          read_file.ignore(256, ',');
          read_file >> starter_eggs;
  read_file.ignore(256, ',');
  read_file >> amount_of_allowable_debt;
  read_file.ignore(256, ',');
  read_file >> profit_goal;
  std::cout << starter_chickens << ", " << starter_eggs << ", " << amount_of_allowable_debt << ", " << profit_goal << std::endl;
  }

  read_file.close();

  cash_difference = (float)profit_goal - farm_cash;
  
  // Setup the Farm
  class Chicken * brood = new class Chicken(starter_chickens, std::chrono::steady_clock::now());
  class Egg * clutch = new class Egg(starter_eggs, std::chrono::steady_clock::now());
  class Coop * broods = new class Coop();
  class Incubator * clutches = new class Incubator();

  broods->push(brood);
  clutches->push(clutch);

  now = std::chrono::steady_clock::now();
  prev_chicken = now;
  prev_egg = now;
  prev_sell = now;
  prev_upkeep = now;

  while (cash_difference > 0) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(5));

    now = std::chrono::steady_clock::now();
    diff = now - start_time;
    diff_chicken = now - prev_chicken;
    diff_egg = now - prev_egg;
    diff_sell = now - prev_sell;
    diff_upkeep = now - prev_upkeep;

    // Lay eggs
	if (diff_chicken >= std::chrono::milliseconds(10)) {
		std::cout << diff.count() << " : Chickens laying eggs..." << std::endl;
		class Egg * new_egg = new class Egg();

		new_egg->setCount(broods->getCount());
		new_egg->setTime(now);
		clutches->push(new_egg);
		prev_chicken = now;
	}

    // Hatch eggs
	if (diff_egg >= std::chrono::milliseconds(100)) {
		std::cout << diff.count() << " : Eggs hatching..." << std::endl;
        std::list<class Chicken *> * temp = clutches->hatchAll();
        if (!temp->empty()) {
    		broods->getList()->insert(broods->getList()->end(), temp->begin(), temp->end());
        }
		prev_egg = now;
		days += 1;
	}

    // If profit_goal can be reached, sell
	if (diff_sell >= std::chrono::milliseconds(25)) {
		float revenue_estimate = broods->getRevenueEstimate() + clutches->getRevenueEstimate();
		float cash = 0;

		if (revenue_estimate > cash_difference) {
			std::cout << diff.count() << " : Selling..." << std::endl;
			cash = broods->sell((unsigned int)((cash_difference - cash) / 10));
            cash += clutches->sell((unsigned int)((cash_difference - cash) / 0.25));
			cash_difference -= cash;
			farm_cash += cash;
	    }

        prev_sell = now;
	}

    // Pay Upkeep
	if (diff_upkeep >= std::chrono::milliseconds(700)) {
		std::cout << diff.count() << " : Upkeep..." << std::endl;
		float cash = 0;
        
        // Sell off excess chickens ( > 25 )
        if (broods->getCount() > 25) {
                cash += broods->sell();
        }

        // Make sure debt limit is not exceeded
        if ( (farm_cash - broods->getUpkeepEstimate() - clutches->getUpkeepEstimate()) < -1 * amount_of_allowable_debt ) {
                std::cout << diff.count() << " : Welp, you're going to exceed the debt limit." << std::endl;
                float debt_difference = (broods->getUpkeepEstimate() + clutches->getUpkeepEstimate()) - (farm_cash + amount_of_allowable_debt);
                float temp_cash = broods->sell((unsigned int)(debt_difference / 10));
                temp_cash += clutches->sell((unsigned int)((debt_difference - temp_cash) / 0.25));
                cash += temp_cash;

                std::cout << diff.count() << " : Disaster avoided! You came this close to the debt limit: " << farm_cash << std::endl;
        } 

//        cash -= (broods->getUpkeepEstimate() + clutches->getUpkeepEstimate());
        cash_difference -= cash;
        farm_cash += cash;

        std::cout << diff.count() << " : Cash Difference : $" << cash_difference << std::endl;
        std::cout << diff.count() << " : Farm Cash : $" << farm_cash << std::endl;
        prev_upkeep = now;
    }
  }

  // Write out results
  write_file << broods->getCount() << "," << clutches->getCount() << "," << (unsigned int)  farm_cash;
  write_file.close();
 
  // Clean up
  end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "Days to reach goal: " << days << std::endl;
  std::cout << "Final chickens: " << broods->getCount() << std::endl;
  std::cout << "Final eggs: " << clutches->getCount() << std::endl;
  std::cout << "Final cash: " << (unsigned int)farm_cash << std::endl;
  std::cout << "Run time: " << diff.count() << std::endl;

  return days;
}
