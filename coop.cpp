#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <cmath>

// Declare the Chicken class
class Chicken {
  private:
  unsigned int count;
  std::chrono::time_point<std::chrono::steady_clock> spawn_time;
  public:
  Chicken();
  Chicken(unsigned int, std::chrono::time_point<std::chrono::steady_clock>);
  unsigned int count_chickens();
  std::chrono::time_point<std::chrono::steady_clock> get_time();
  void set_chickens(unsigned int);
  void set_time(std::chrono::time_point<std::chrono::steady_clock>);
  void  lay_eggs(class Egg *, std::chrono::time_point<std::chrono::steady_clock>);
  float sell_chickens(unsigned int);
  float upkeep();
};

//Declare the Egg class
class Egg {
  private:
  unsigned int count;
  std::chrono::time_point<std::chrono::steady_clock> spawn_time;
  public:
  Egg();
  Egg(unsigned int, std::chrono::time_point<std::chrono::steady_clock>);
  unsigned int count_eggs();
  std::chrono::time_point<std::chrono::steady_clock> get_time();
  void set_eggs(unsigned int);
  void set_time(std::chrono::time_point<std::chrono::steady_clock>);
  void hatch(class Chicken *, std::chrono::time_point<std::chrono::steady_clock>);
  float sell_eggs(unsigned int);
  float upkeep();
};

// Declare mutex
std::mutex chicken_mutex;
std::mutex egg_mutex;
std::mutex difference_mutex;

// Define Chicken
// From the Merriam-Webster Dictionary:
// 1 a: the common domestic fowl (Gallus gallus) especially when young; also : its flesh used as food
//   b: any of various birds or their young
// 2  : a young woman
// 3 a: coward
//   b: any of various contests in which the paricipants risk personal safety in order to see which one will give up first
// 4  : short for chickenshit
//    : slang : petty details
// 5  : slang : a young male homosexual
Chicken::Chicken() {
  count = 0;
}

Chicken::Chicken(unsigned int c, std::chrono::time_point<std::chrono::steady_clock> t) {
  count = c;
  spawn_time = t;
}

unsigned int Chicken::count_chickens() {
  return count;
}

std::chrono::time_point<std::chrono::steady_clock> Chicken::get_time() {
  return spawn_time;
}

void Chicken::set_chickens(unsigned int c) {
  count = c;
}

void Chicken::set_time(std::chrono::time_point<std::chrono::steady_clock> t) {
  spawn_time = t;
}

void Chicken::lay_eggs(class Egg * e, std::chrono::time_point<std::chrono::steady_clock> t) {
  e->set_eggs(count);
  e->set_time(t);
}

float Chicken::sell_chickens(unsigned int num) {
  count -= num;
  return (float) 10*num;
}

float Chicken::upkeep() {
	return 10*(float)count;
}

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
}

Egg::Egg(unsigned int c, std::chrono::time_point<std::chrono::steady_clock> t) {
  count = c;
  spawn_time = t;
}

unsigned int Egg::count_eggs() {
  return count;
}

std::chrono::time_point<std::chrono::steady_clock> Egg::get_time() {
  return spawn_time;
}

void Egg::set_eggs(unsigned int c) {
  count = c;
}

void Egg::set_time(std::chrono::time_point<std::chrono::steady_clock> t) {
  spawn_time = t;
}

void Egg::hatch(class Chicken * c, std::chrono::time_point<std::chrono::steady_clock> t) {
  c->set_chickens(count);
  c->set_time(t);
}

float Egg::sell_eggs(unsigned int num) {
  count = count - num;
  return 0.25*(float)num;
}

float Egg::upkeep() {
	return 0.05*(float)count;
}

// Declare and define chicken_manager
void chicken_manager(std::vector<class Chicken *> *ckn_list, std::vector<class Egg *> *eg_list) {
  while (ckn_list->size() > 0) {
	class Egg * new_egg = new class Egg();
	unsigned int total_chickens = 0;
  	std::this_thread::sleep_for(std::chrono::milliseconds(10));
  	while (!chicken_mutex.try_lock() && !egg_mutex.try_lock()) {
  	std::this_thread::sleep_for(std::chrono::microseconds(500));
  	}
	std::cout << "From chicken_manager thread: chickens laying eggs." << std::endl;
	for (std::vector<class Chicken *>::const_iterator it = ckn_list->begin(); it != ckn_list->end(); ++it) {
		total_chickens += (*it)->count_chickens();
	}
	new_egg->set_eggs(total_chickens);
	new_egg->set_time(std::chrono::steady_clock::now());
	eg_list->push_back(new_egg);
	chicken_mutex.unlock();
	egg_mutex.unlock();
  }
}

// Declare and define egg_manager
void egg_manager(std::vector<class Chicken *> *ckn_list, std::vector<class Egg *> *eg_list) {
  while (eg_list->size() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (!chicken_mutex.try_lock() && !egg_mutex.try_lock()) {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    std::cout << "From egg_manager thread: eggs hatching." << std::endl;
    class Chicken * new_chicken = new class Chicken();
    eg_list->front()->hatch(new_chicken, std::chrono::steady_clock::now());
    ckn_list->push_back(new_chicken);
    eg_list->erase(eg_list->begin());
    chicken_mutex.unlock();
    egg_mutex.unlock();
  }
}

// Declare and define upkeep
void upkeep(std::vector<class Chicken *> *ckn_list, std::vector<class Egg *> *eg_list, float * cash_difference) {
  std::chrono::time_point<std::chrono::steady_clock> now;
  while ( *cash_difference > 0 ) {
	unsigned int total_chickens = 0;
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	while (!chicken_mutex.try_lock() && !egg_mutex.try_lock()) {
	std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
	for (std::vector<class Chicken *>::const_iterator it = ckn_list->begin(); it != ckn_list->end(); ++it) {
		total_chickens += (*it)->count_chickens();
	}
	while ( total_chickens > 25 ) {
		if (ckn_list->front()->count_chickens() > 25) { 
			total_chickens -= ckn_list->front()->count_chickens() - 25;
			*cash_difference -= ckn_list->front()->sell_chickens(ckn_list->front()->count_chickens() - 25);
		} else {
			total_chickens -= ckn_list->front()->count_chickens();
			*cash_difference -= ckn_list->front()->sell_chickens(ckn_list->front()->count_chickens());
			ckn_list->erase(ckn_list->begin());
		}
  	}
	std::cout << "From upkeep thread: current cash $" << *cash_difference << std::endl;
	chicken_mutex.unlock();
	egg_mutex.unlock();
	}
}

int main(int argc, char const * argv[]) {
  // Setup File I/O
  std::string const read_filename = "StarterFarm.txt";
  std::string const write_filename = "FinalFarm.txt";
  std::fstream read_file(read_filename, read_file.in);
  std::fstream write_file(write_filename, write_file.out);

  // Setup the Chicken Coop
  std::string string_chicken, string_eggs, string_debt, string_goal;
  unsigned int starter_chickens, starter_eggs, amount_of_allowable_debt, profit_goal;
  unsigned int days = 0;
  float farm_cash = 0.0;
  float cash_difference = 0.0;

  // Setup the Accounts
  std::chrono::time_point<std::chrono::steady_clock> start_time, now, end_time;
  std::chrono::time_point<std::chrono::steady_clock> prev_chicken, prev_egg, prev_sell, prev_upkeep;
  std::chrono::duration<double> diff, diff_chicken, diff_egg, diff_sell, diff_upkeep;

  // Read in given variables and start time
  if (!read_file.is_open()) {
  std::cout << "Failed to open " << read_filename << std::endl;
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

  // Setup the chicken farm
  std::vector<class Chicken *> chicken_vector = std::vector<class Chicken *>();
  std::vector<class Egg *> egg_vector = std::vector<class Egg *>();
  class Chicken * chicken_ptr = new class Chicken(starter_chickens, start_time);
  class Egg * egg_ptr = new class Egg(starter_eggs, start_time);

  chicken_vector.push_back(chicken_ptr);
  egg_vector.push_back(egg_ptr);
  cash_difference = (float)profit_goal - farm_cash;

  std::cout << "Farm setup!" << std::endl << "Chickens: " << chicken_ptr->count_chickens() << std::endl << "Eggs: " << egg_ptr->count_eggs() << std::endl << "Profit Goal: " << profit_goal << std::endl << "Allowable debt: " << amount_of_allowable_debt << std::endl;

  // Setup the threads
  //  std::thread chicken_thread, egg_thread, upkeep_thread;
  //  chicken_thread = std::thread(chicken_manager, &chicken_vector, &egg_vector);
  //  egg_thread = std::thread(egg_manager, &chicken_vector, &egg_vector);
  //  upkeep_thread = std::thread(upkeep, &chicken_vector, &egg_vector, &cash_difference);

  // Start the timer
  start_time = std::chrono::steady_clock::now();
  prev_chicken = start_time;
  prev_egg = start_time;
  prev_sell = start_time;
  prev_upkeep = start_time;

  // Do the stuff now outsourced to the threads
  while (cash_difference > 0) {
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	
	now = std::chrono::steady_clock::now();
	diff = now -start_time;
	diff_chicken = now - prev_chicken;
	diff_egg = now - prev_egg;
	diff_sell = now - prev_sell;
	diff_upkeep = now - prev_upkeep;

	if (diff_chicken >= std::chrono::milliseconds(10)) {
		std::cout << diff.count() << " : Chickens laying eggs..." << std::endl;
		unsigned int total_chickens = 0;
		class Egg * new_egg = new class Egg();

		for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ++it) {
			total_chickens += (*it)->count_chickens();
		}
		new_egg->set_eggs(total_chickens);
		new_egg->set_time(now);
		egg_vector.push_back(new_egg);
		prev_chicken = now;
	}
	if (diff_egg >= std::chrono::milliseconds(100)) {
		std::cout << diff.count() << " : Eggs hatching..." << std::endl;
		class Chicken * new_chicken = new class Chicken();
		egg_vector.front()->hatch(new_chicken, now);
		chicken_vector.push_back(new_chicken);
		egg_vector.erase(egg_vector.begin());
		prev_egg = now;
		days += 1;
	}
	if (diff_sell >= std::chrono::milliseconds(25)) {
		unsigned int total_chickens = 0, total_eggs = 0;
		float revenue_estimate = 0;
		float cash = 0;

		for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ++it) {
			total_chickens += (*it)->count_chickens();
		}
		for (std::vector<class Egg *>::const_iterator it = egg_vector.begin(); it != egg_vector.end(); ++it) {
			total_eggs += (*it)->count_eggs();
		}
		
		revenue_estimate += 10*total_chickens;
		revenue_estimate += 0.25*total_eggs;

		if (revenue_estimate > cash_difference) {
			if ((*chicken_vector.begin())->count_chickens() >= (unsigned int)(cash_difference / 10)) {
				cash = (*chicken_vector.begin())->sell_chickens((unsigned int)(cash_difference / 10));
				cash_difference -= cash;
				farm_cash += cash;
			} else {
				unsigned int to_sell = (unsigned int)(cash_difference / 10);

				for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ) {
					if (to_sell > 0) {
						if (to_sell > (*it)->count_chickens()) {
							to_sell -= (*it)->count_chickens();
							cash += (*it)->sell_chickens((*it)->count_chickens());
							it = chicken_vector.erase(it);
						} else {
							cash += (*it)->sell_chickens(to_sell);
							to_sell = 0;
							++it;
						}
					} else {
						++it;
					}
				}
				cash_difference -= cash;
				farm_cash += cash;
			}
			if ((*egg_vector.begin())->count_eggs() >= (unsigned int)(cash_difference / 0.25)) {
				cash = (*egg_vector.begin())->sell_eggs((unsigned int)(cash_difference / 0.25));
				cash_difference -= cash;
				farm_cash += cash;
			} else {	
				unsigned int to_sell = (unsigned int)(cash_difference / 0.25);

				for (std::vector<class Egg *>::const_iterator it = egg_vector.begin(); it != egg_vector.end(); ) {
					if (to_sell > 0) {
						if (to_sell > (*it)->count_eggs()) {
							to_sell -= (*it)->count_eggs();
							cash += (*it)->sell_eggs((*it)->count_eggs());
							it = egg_vector.erase(it);
						} else {
							cash += (*it)->sell_eggs(to_sell);
							to_sell = 0;
							++it;
						}
					} else {
						++it;
					}
				}
			}
			std::cout << diff.count() << " : Selling..." << std::endl;
		}
	}
	if (diff_upkeep >= std::chrono::milliseconds(700)) {
		std::cout << diff.count() << " : Upkeep..." << std::endl;
		unsigned int total_chickens = 0;
		unsigned int total_eggs = 0;
		float cash = 0;
		float chicken_upkeep_est = 0;

		for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ++it) {
			total_chickens += (*it)->count_chickens();
		}
		for (std::vector<class Egg *>::const_iterator it = egg_vector.begin(); it != egg_vector.end(); ++it) {
			total_eggs += (*it)->count_eggs();
		}

		// Sell off excess chickens ( > 25)
		// total_chickens = 25 after this function
		if (total_chickens > 25) {
			unsigned int to_sell = total_chickens - 25;

			for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ) {
				if (to_sell > 0) {
					if (to_sell > (*it)->count_chickens()) {
						to_sell -= (*it)->count_chickens();
						total_chickens -= (*it)->count_chickens();
						cash += (*it)->sell_chickens((*it)->count_chickens());
						it = chicken_vector.erase(it);
					} else {
						total_chickens -= to_sell;
						cash += (*it)->sell_chickens(to_sell);
						to_sell = 0;
						++it;
					}
				} else {
					++it;
				}
			}

			cash_difference -= cash;
			farm_cash+= cash;
		}

		for (unsigned int i = 0; i < total_chickens; i++) {
			if (i < 5) {
				chicken_upkeep_est += 2;
			} else {
				chicken_upkeep_est += 5*(floor(i/5));
			}
		}	
		// Make sure not to exceed allowable debt
		// Need to check this, it results in an infinite loop
		if ( (farm_cash - chicken_upkeep_est - 0.25*total_eggs) < -1*amount_of_allowable_debt) {
			std::cout << "Welp, you are going to exceeded your debt limit." << std::endl;
			float debt_diff = (chicken_upkeep_est + 0.25*total_eggs) - (farm_cash + amount_of_allowable_debt);
			unsigned int chicken_to_sell = (unsigned int)(debt_diff / 10);
			float cash = 0;

			for (unsigned int i = total_chickens; i > 0; i--) {
				std::vector<class Chicken *>::const_iterator it = chicken_vector.begin();
				float this_time_cash = 0;

				if (debt_diff > 1) {
					if ( i < 5) {
						this_time_cash += (*it)->sell_chickens(1);
						cash += this_time_cash;
						debt_diff -= 2;
						total_chickens -= 1;
					} else {
						this_time_cash += (*it)->sell_chickens(1);
						cash += this_time_cash;
						debt_diff -= 5*(floor(i/5));
						total_chickens -=1;
					}
					if ((*it)->count_chickens() < 1) {
						it = chicken_vector.erase(it);
					}
					debt_diff -= this_time_cash;
				}
			}
			
			for (unsigned int i = total_eggs; i > 0; i--) {
				std::vector<class Egg *>::const_iterator it = egg_vector.begin();
				float this_time_cash = 0;

				if (debt_diff > 0) {
					this_time_cash += (*it)->sell_eggs(1);
					cash += this_time_cash;
					debt_diff -= 0.25;
					total_eggs -= 1;
				}
				if ((*it)->count_eggs() < 1) {
					it = egg_vector.erase(it);
				}
				debt_diff -= this_time_cash;
			}
			cash_difference -= cash;
			farm_cash += cash;

			std::cout << "Disaster avoided!" << std::endl << "You were this close to exceeding the debt limit: " << farm_cash << std::endl;
		}

		// Pay for chickens and eggs
		for (std::vector<class Egg *>::const_iterator it = egg_vector.begin(); it != egg_vector.end(); ++it) {
			cash_difference += (*it)->upkeep();
			farm_cash -= (*it)->upkeep();
		}
		cash_difference += chicken_upkeep_est;
		farm_cash -= chicken_upkeep_est;
		std::cout << diff.count() << " : Cash Difference: $" << cash_difference << std::endl;
		std::cout << diff.count() << " : Farm Cash: $" << farm_cash << std::endl;
		prev_upkeep = now;
	}
  }

  // Join the threads
//  chicken_thread.join();
//  egg_thread.join();
//  upkeep_thread.join();

  // Write out results
  unsigned int final_chickens = 0;
  unsigned int final_eggs = 0;
  for (std::vector<class Chicken *>::const_iterator it = chicken_vector.begin(); it != chicken_vector.end(); ++it) {
	final_chickens += (*it)->count_chickens();
  }
  for (std::vector<class Egg *>::const_iterator it = egg_vector.begin(); it != egg_vector.end(); ++it) {
	final_eggs += (*it)->count_eggs();
  }
  write_file << final_chickens << "," << final_eggs << "," << (unsigned int)  farm_cash;
  // Clean up
  end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "Days to reach goal: " << days << std::endl;
  std::cout << "Final chickens: " << final_chickens << std::endl;
  std::cout << "Final eggs: " << final_eggs << std::endl;
  std::cout << "Final cash: " << (unsigned int)farm_cash << std::endl;
  std::cout << "Run time: " << diff.count() << std::endl;
  write_file.close();

  return days;
}
