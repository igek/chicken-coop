//Declare the Egg class
class Egg {
        private:
                unsigned int count;
                std::chrono::time_point<std::chrono::steady_clock> spawn_time;
        public:
                Egg();
                Egg(unsigned int, std::chrono::time_point<std::chrono::steady_clock>);
                unsigned int getCount();
                std::chrono::time_point<std::chrono::steady_clock> getTime();
                void setCount(unsigned int);
                void setTime(std::chrono::time_point<std::chrono::steady_clock>);
                class Chicken * hatch();
                float sell();
                float sell(unsigned int);
};
