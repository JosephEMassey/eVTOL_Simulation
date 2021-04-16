cmake_minimum_required(VERSION 3.1.0)
project(eVTOL_Unit_Tests)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin/)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../include)
 
# source files
file(GLOB_RECURSE SOURCES "../src/Simulation.cpp" "../src/Charger.cpp" "../src/SimulationThread.cpp" "../src/Vehicle.cpp" "*.cpp")

# Link runTests with what we want to test and the GTest and pthread library
add_executable(${PROJECT_NAME} ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} Threads::Threads)