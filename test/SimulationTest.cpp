#include <chrono>
#include <memory>

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

/**
 * @brief Test Simulation::Create
 * 
 */
TEST_F (SimulationTest, Create) 
{ 
    Simulation* simulation = new Simulation(10, 5, 10);
    
    // Verify number of simulation objects created
    EXPECT_EQ(20, simulation->Create());

    delete simulation;
}

/**
 * @brief Test Simulation::Run
 * 
 */
TEST_F (SimulationTest, Run) 
{ 
    Simulation* simulation = new Simulation(10, 5, 10);
    simulation->Create();

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    // Verify simulation runs for 10s (10 minutes of simulation time)
    simulation->Run(10);

    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start);

    EXPECT_NEAR(10, time_span.count(), 1.00);

    delete simulation;
}