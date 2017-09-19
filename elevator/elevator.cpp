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
    m_last = StateStopped;
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

void Elevator::reqFromCabin(int floor)
{
    if (floor < 1 || floor > m_maximumFloor)
        return;

    m_reqInt.push_back(floor);
    m_reqInt.unique();
    m_reqInt.sort();
}

void Elevator::reqFromFloor(int floor)
{
    if (floor < 1 || floor > m_maximumFloor)
        return;

    m_reqExt.push_back(floor);
    m_reqExt.unique();
    m_reqExt.sort();
}

void Elevator::doStop()
{
    m_reqInt.clear();
}

void Elevator::doAdvance()
{
    // Cabin stopped
    // - process NEXT requests from CABIN in SAME direction
    // - process NEXT requests from CABIN in ANY direction
    // - process TOP request from FLOOR
    // - process NEXT request from FLOOR in TOP, then BOTTOM direction
    //
    // Cabin moving UP
    // - stop on CABIN requests (assume that floor buttons are "down")
    // - stop on TOP request from floor
    //
    // Cabin moving DOWN
    // - stop on ALL requests
    //
    // Do advance (read state --> move cabin)

    // 1. Cabin is stopped (doors are opened)
    if (m_state == StateStopped) {

        // Remove current floor
        //m_reqInt.remove(m_currentFloor);
        //m_reqExt.remove(m_currentFloor);

        // Check requests from CABIN
        if (!m_reqInt.empty()) {

            // DEBUG ===============
            m_state = StateMovingUp;
            // =====================

            // Find requests in SAME direction


            // Find requests in ANOTHER direction

        }

        // Check requests from FLOOR
        // ...
    }

    // Do advance
    if (m_state == StateMovingUp) {
        m_currentFloor += 1;
    }
    else if (m_state == StateMovingDown) {
        m_currentFloor -= 1;
    }

    // Update last state
    m_last = m_state;
}
