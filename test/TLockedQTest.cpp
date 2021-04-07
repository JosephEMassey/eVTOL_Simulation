#include <gtest/gtest.h>

#include "TLockedQueue.h"

class TLockedQTest: public ::testing::Test 
{ 
    public: 
        TLockedQTest( ) { 
            // initialization code here"
        } 

        void SetUp( ) { 
            // code here will execute just before the test ensues 
        }

        void TearDown( ) { 
            // code here will be called just after the test completes
            // ok to through exceptions from here if need be
        }

        ~TLockedQTest( )  { 
            // cleanup any pending stuff, but no exceptions allowed
        }

        TLockedQueue<int> _q;
};

TEST_F (TLockedQTest, enqueue) 
{ 
    EXPECT_EQ(0, _q.size());

    for(unsigned int i = 0; i < 10; ++i)
        _q.enqueue(i);

    EXPECT_EQ(10, _q.size());
}

TEST_F (TLockedQTest, dequeue) 
{ 
    for(unsigned int i = 0; i < 10; ++i)
        _q.enqueue(i);

    while(_q.size())
        _q.dequeue();

    EXPECT_EQ(0, _q.size());
}