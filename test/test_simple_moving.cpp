#include "elevator.h"
#include <iostream>

//
// Complex check for Elevator state
//
bool check(const Elevator *elevator, CabinState state, int floor) {

    if (elevator == NULL) {
        std::cout << "== Empty pointer" << std::endl;
        return false;
    }
    if (elevator->state() != state || elevator->floor() != floor) {
        std::cout << "== Wrong! State '" << elevator->state() << "'"
                  << " (expected '" << state << "'). Floor '"
                  << elevator->floor() << "'"
                  << " (expected '" << floor << "')" << std::endl;
        return false;
    }

    return true;
}

//
// Main test
//
int main (int, char *[])
{
    std::cout << "1. Create object" << std::endl;
    Elevator elevator(100, 50);
    if (!check(&elevator, StateStopped, 50))
        return -1;

    std::cout << "2. Advance without request" << std::endl;
    for (int i = 0; i < 100; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateStopped, 50)) return -1;

    std::cout << "3. Cabin request: moving sequence (from 50th to 30th)" << std::endl;
    elevator.reqFromCabin(30);
    if (!check(&elevator, StateStopped, 50)) return -1;
    elevator.doAdvance();
    if (!check(&elevator, StateMovingDown, 50)) return -1;
    for (int i = 0; i < 19; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateMovingDown, 31)) return -1;
    elevator.doAdvance();
    if (!check(&elevator, StateStopped, 30)) return -1;

    std::cout << "4. Floor request: moving sequence (from 30th to 40th)" << std::endl;
    elevator.reqFromFloor(40);
    if (!check(&elevator, StateStopped, 30)) return -1;
    elevator.doAdvance();
    if (!check(&elevator, StateMovingUp, 30)) return -1;
    for (int i = 0; i < 9; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateMovingUp, 39)) return -1;
    elevator.doAdvance();
    if (!check(&elevator, StateStopped, 40)) return -1;

    std::cout << "5. Mixed requests (40, C30, F35, C20)" << std::endl;

    elevator.reqFromCabin(30);
    elevator.reqFromFloor(35);
    elevator.reqFromCabin(20);

    if (!check(&elevator, StateStopped, 40)) return -1;

    for (int i = 0; i < 5+1; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateStopped, 35)) return -1;

    for (int i = 0; i < 5+1; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateStopped, 30)) return -1;

    for (int i = 0; i < 10+1; ++i)
        elevator.doAdvance();
    if (!check(&elevator, StateStopped, 20)) return -1;

    // All tests passed
    return 0;
}
