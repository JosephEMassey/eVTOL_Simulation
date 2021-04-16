#include <gtest/gtest.h>

#include "Simulation.h"

class SimulationTest: public ::testing::Test 
{ 
    public: 
        SimulationTest( ) { 
            // initialization code here"
        } 

        void SetUp( ) { 
            // code here will execute just before the test ensues 
        }

        void TearDown( ) { 
            // code here will be called just after the test completes
            // ok to through exceptions from here if need be
        }

        ~SimulationTest( )  { 
            // cleanup any pending stuff, but no exceptions allowed
        }
};


TEST_F (SimulationTest, Create) 
{ 
    // Simulation* simulation = new Simulation(1, 5, 1);
    
    // EXPECT_EQ(0, _q.size());

    // for(unsigned int i = 0; i < 10; ++i)
    //     _q.enqueue(i);

    // EXPECT_EQ(10, _q.size());
}

TEST_F (SimulationTest, Run) 
{ 
    // for(unsigned int i = 0; i < 10; ++i)
    //     _q.enqueue(i);

    // while(_q.size())
    //     _q.dequeue();

    // EXPECT_EQ(0, _q.size());
}