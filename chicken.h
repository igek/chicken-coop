// Declare Chicken class
class Chicken {
	private:
		unsigned int count;
		std::chrono::time_point<std::chrono::steady_clock> spawn_time;
	public:
		Chicken();
		Chicken(unsigned int, std::chrono::time_point<std::chrono::steady_clock>);
		unsigned int getCount();
		std::chrono::time_point<std::chrono::steady_clock> getTime();
		void setCount(unsigned int);
		void setTime(std::chrono::time_point<std::chrono::steady_clock>);
		class Egg * layEggs();
        float sell();
		float sell(unsigned int);
};
