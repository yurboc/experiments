#include "elevator.h"   // class Elevator
#include <cmath>        // abs

Elevator::Elevator(int nFloors, int startFloor)
{
    // Save count of floors
    m_maximumFloor = nFloors;
    if (m_maximumFloor < 1)
        m_maximumFloor = 1;

    // Set current floor
    m_currentFloor = startFloor;
    if (m_currentFloor < 1)
        m_currentFloor = 1;
    if (m_currentFloor > m_maximumFloor)
        m_currentFloor = m_maximumFloor;

    // Set initial state
    m_currentState = StateStopped;
    m_globalState = StateStopped;

    // Initialize all floors
    m_floors = std::vector<FloorState>(m_maximumFloor, FloorState());
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
    return m_currentState;
}

int Elevator::findNearFloor(bool reqUp, bool reqDown, bool reqInternal,
                            bool searchUp, bool searchDown) const
{
    int minDist = m_maximumFloor;
    int nearFloor = -1;
    int searchFromFloor = 1;
    int searchDirection = 1; // 1: up, -1: down, 0: check current

    // Check for invalid conditions
    if (!reqUp && !reqDown & !reqInternal)
        return -1;

    // Set search mode
    if (searchUp && searchDown) { // Find in any direction
        searchFromFloor = 1;
        searchDirection = 1;
    }
    if (searchUp && !searchDown) { // Find ascending
        searchFromFloor = m_currentFloor+1;
        searchDirection = 1;
    }
    else if (searchDown && !searchUp) { // Find descending
        searchFromFloor = m_currentFloor-1;
        searchDirection = -1;
    }
    else if (!searchUp && !searchDown) { // Check current floor
        searchFromFloor = m_currentFloor;
        searchDirection = 0;
    }

    // Check current floor
    if (searchDirection == 0) {
        FloorState floor = m_floors.at(searchFromFloor-1);
        if ((reqUp && floor.reqUp)
                || (reqDown && floor.reqDown)
                || (reqInternal && floor.reqFromCabin)) {
            return searchFromFloor;
        }
        else {
            return -1;
        }
    }

    // Find near floor
    for (int i = searchFromFloor;
         i >= 1 && i <= m_maximumFloor;
         i+=searchDirection) {

        // Take floor
        FloorState floor = m_floors[i-1];
        int dist = std::abs(i - m_currentFloor);

        // Update "near floor"
        if (dist < minDist
                && ((reqUp && floor.reqUp)
                    || (reqDown && floor.reqDown)
                    || (reqInternal && floor.reqFromCabin))) {
            minDist = dist;
            nearFloor = i;
        }
    }

    // Return results
    return nearFloor;
}

void Elevator::doStartMoving()
{
    // Select next floor
    int nearFloor = findNearFloor(false, false, true, true, true); // from cabin
    if (nearFloor < 0) {
        nearFloor = findNearFloor(true, true, false, true, true); // from floor
    }

    // No requests
    if (nearFloor < 0)
        return;

    // Go to near floor
    if (nearFloor < m_currentFloor) {
        m_globalState = StateMovingDown;
        m_currentState = StateMovingDown;
        return;
    }
    else if (nearFloor > m_currentFloor) {
        m_globalState = StateMovingUp;
        m_currentState = StateMovingUp;
        return;
    }

    // Do nothing if requested floor is current
    m_floors[nearFloor-1].reqFromCabin = false;
    m_floors[nearFloor-1].reqUp = false;
    m_floors[nearFloor-1].reqDown = false;
    m_globalState = StateStopped;
    m_currentState = StateStopped;
    return;
}

void Elevator::doContinueMoving()
{
    // Examine current floor
    FloorState &floor = m_floors[m_currentFloor-1];

    // Current floor reqested from cabin
    if (floor.reqFromCabin) {
        floor.reqFromCabin = false;
        m_currentState = StateStopped;
        return;
    }

    // Current floor requested DOWN (cabin going DOWN)
    if (floor.reqDown && m_currentState == StateMovingDown) {
        floor.reqDown = false;
        m_currentState = StateStopped;
        return;
    }

    // Current floor requested UP (cabin going UP)
    if (floor.reqUp && m_currentState == StateMovingUp) {
        floor.reqUp = false;
        m_currentState = StateStopped;
        return;
    }

    // Current floor requested DOWN (cabin going UP)
    if (floor.reqDown && m_currentState == StateMovingUp) {

        // Stop if this is highest floor with "go down" command
        int nextUpFloor = findNearFloor(true, true, true, true, false);
        if (nextUpFloor < 0) {
            floor.reqDown = false;
            m_currentState = StateStopped;
            return;
        }
    }

    // Current floor requested UP (cabin going DOWN)
    if (floor.reqUp && m_currentState == StateMovingDown) {

        // Stop if this is highest floor with "go down" command
        int nextDownFloor = findNearFloor(true, true, true, false, true);
        if (nextDownFloor < 0) {
            floor.reqUp = false;
            m_currentState = StateStopped;
            return;
        }
    }
}

void Elevator::doResumeMoving()
{
    // Check if cabin stopped
    if (m_currentState != StateStopped)
        return;

    // Find ANY requests for going UP
    if (m_globalState == StateMovingUp) {

        // Find next floor (req. from CABIN)
        int nearUp = findNearFloor(false, false, true, true, false);

        // Find next floor (req. from FLOOR, req. UP)
        if (nearUp < 0)
            nearUp = findNearFloor(true, false, false, true, false);

        // Request found, continue moving
        if (nearUp >= 1) {
            m_currentState = m_globalState;
            return;
        }
    }

    // Find ANY requests for going DOWN
    if (m_globalState == StateMovingDown) {

        // Find next floor (req. from CABIN)
        int nearDown = findNearFloor(false, false, true, false, true);

        // Find next floor (req. from FLOOR, req. DOWN)
        if (nearDown < 0)
            nearDown = findNearFloor(false, true, false, false, true);

        // Request found, continue moving
        if (nearDown >= 1) {
            m_currentState = m_globalState;
            return;
        }
    }

    // Find CABIN requests for going ANY direction
    int nearAny = findNearFloor(false, false, true, true, true);
    if (nearAny >= 1) {
        m_globalState = (nearAny > m_currentFloor) ? StateMovingUp : StateMovingDown;
        m_currentState = m_globalState;
        return;
    }

    // Find FLOOR requests for going ANY direction
    nearAny = findNearFloor(true, true, false, true, true);
    if (nearAny >= 1) {
        m_globalState = (nearAny > m_currentFloor) ? StateMovingUp : StateMovingDown;
        m_currentState = m_globalState;
        return;
    }

    // No requests. Stop
    m_globalState = StateStopped;
}

void Elevator::reqFromCabin(int floor)
{
    // Check constraints
    if (floor < 1 || floor > m_maximumFloor)
        return;

    // Request stop on floor
    m_floors[floor-1].reqFromCabin = true;
}

void Elevator::reqFromFloor(int floor)
{
    // Check constraints
    if (floor < 1 || floor > m_maximumFloor)
        return;

    if (floor == 1) // from first floor - only UP
        m_floors[floor-1].reqUp = true;
    else // from all other floors - only DOWN
        m_floors[floor-1].reqDown = true;
}

void Elevator::doAdvance()
{
    // Start moving
    if (m_currentState == StateStopped && m_globalState == StateStopped) {
        doStartMoving();
        return;
    }

    // Continue moving
    if (m_currentState == StateMovingDown) {
        if (m_currentFloor > 1) {
            --m_currentFloor;
        }
        doContinueMoving();
        return;
    }
    else if (m_currentState == StateMovingUp) {
        if (m_currentFloor < m_maximumFloor) {
            ++m_currentFloor;
        }
        doContinueMoving();
        return;
    }
    else {
        // Cabin stopped on requested floor
        doResumeMoving();
        return;
    }
}
