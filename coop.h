// Declare Coop class
class Coop {
        private:
                std::list<class Chicken *> * broods;
                unsigned int count;
                float upkeep_estimate;
                float revenue_estimate;
                void updateUpkeepEstimate(unsigned int);
        public:
                Coop();
                Coop(std::list<class Chicken *> *);
                unsigned int getCount();
                std::list<class Chicken *> * getList();
                float getUpkeepEstimate();
                float getRevenueEstimate();
                void push(class Chicken *);
                float sell();
                float sell(unsigned int);
};
