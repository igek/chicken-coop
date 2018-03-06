// Declare Incubator class
class Incubator {
        private:
                std::list<class Egg *> * clutches;
                unsigned int count;
                float upkeep_estimate;
                float revenue_estimate;
        public:
                Incubator();
                Incubator(std::list<class Egg *> *);
                unsigned int getCount();
                float getUpkeepEstimate();
                float getRevenueEstimate();
                std::list<class Chicken *> * hatchAll();
                void push(class Egg *);
                float sell(unsigned int);
};
