#include "elevator.h"   // class Elevator
#include <stdlib.h>     // srand, rand
#include <time.h>       // time

Elevator::Elevator(int nFloors)
{
    // Initialize random seed
    srand(time(NULL));

    // Save count of floors
    m_maximumFloor = nFloors;

    // Set random current floor
    m_currentFloor = rand() % m_maximumFloor + 1;

    // Set initial state
    m_state = StateStopped;
}

Elevator::~Elevator()
{
}

int Elevator::floor() const
{
    return m_currentFloor;
}

CabinState Elevator::state() const
{
    return m_state;
}

void Elevator::doAdvance()
{
}

void Elevator::reqFromCabin(int floor)
{
}

void Elevator::reqFromFloor(int floor)
{
}
