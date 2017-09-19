#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <list>    // std::list

enum CabinState {
    StateUndefined,
    StateStopped,
    StateMovingUp,
    StateMovingDown
};

class Elevator {
public:
    Elevator(int nFloors);
    ~Elevator();

    void reqFromCabin(int floor);
    void reqFromFloor(int floor);
    void doAdvance();
    void doStop();

    int floor() const;
    CabinState state() const;

private:
    std::list<int> m_reqInt;
    std::list<int> m_reqExt;

    CabinState m_state;
    int m_maximumFloor;
    int m_currentFloor;
};

#endif // ELEVATOR_H
