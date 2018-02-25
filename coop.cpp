#include <iostream>
#include <chrono>
#include <ctime>
#include <list>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

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
	std::this_thread::sleep_for(std::chrono::microseconds(500));
	}
	for (std::vector<class Chicken *>::const_iterator it = ckn_list->begin(); it != ckn_list->end(); ++it) {
		total_chickens += (*it)->count_chickens();
	}
	while ( total_chickens > 25 ) {
		if (ckn_list->front()->count_chickens() > 25) { 
			total_chickens -= ckn_list->front()->count_chickens() - 25;
			*cash_difference += ckn_list->front()->sell_chickens(ckn_list->front()->count_chickens() - 25);
		} else {
			total_chickens -= ckn_list->front()->count_chickens();
			*cash_difference += ckn_list->front()->sell_chickens(ckn_list->front()->count_chickens());
			ckn_list->erase(ckn_list->begin());
		}
  	}
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
  float farm_cash = 0.0;
  float cash_difference = (float) profit_goal - farm_cash;

  // Setup the Accounts
  std::chrono::time_point<std::chrono::steady_clock> start_time, now, end_time;
  std::chrono::duration<double> diff;

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

  // Setup coop and carton
  std::list<class Chicken *> *coop = new std::list<class Chicken *>();
  std::list<class Egg *> *carton = new std::list<class Egg *>();

  coop->push_back(chicken_ptr);
  carton->push_back(egg_ptr);

  std::cout << "Farm setup!" << std::endl << "Chickens: " << chicken_ptr->count_chickens() << std::endl << "Eggs: " << egg_ptr->count_eggs() << std::endl;

  // Setup the threads
  std::thread chicken_thread, egg_thread, upkeep_thread;
  chicken_thread = std::thread(chicken_manager, &chicken_vector, &egg_vector);
  egg_thread = std::thread(egg_manager, &chicken_vector, &egg_vector);
  upkeep_thread = std::thread(upkeep, &chicken_vector, &egg_vector, &cash_difference);

  // Start the timer
  start_time = std::chrono::steady_clock::now();

  // Do the stuff now outsourced to the threads

  // Join the threads
  chicken_thread.join();
  egg_thread.join();
  upkeep_thread.join();

  // Write out results
  write_file << chicken_ptr->count_chickens() << "," << egg_ptr->count_eggs() << "," << (unsigned int)  farm_cash;
  // Clean up
  end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "Run time: " << diff.count() << std::endl;
  write_file.close();

  return 0;
}
