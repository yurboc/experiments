#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>

enum CabinState {
    StateStopped,
    StateMovingUp,
    StateMovingDown
};

struct FloorState {
    bool reqFromCabin;
    bool reqUp;
    bool reqDown;
    FloorState() : reqFromCabin(false), reqUp(false), reqDown(false) {}
};

class Elevator {
public:
    Elevator(int nFloors);
    ~Elevator();

    void reqFromCabin(int floor);
    void reqFromFloor(int floor);
    void doAdvance();

    int floor() const;
    CabinState state() const;

private:
    int findNearFloor(bool reqUp, bool reqDown, bool reqInternal,
                      bool searchUp, bool searchDown) const;
    void doStartMoving();
    void doContinueMoving();
    void doResumeMoving();

    std::vector<FloorState> m_floors;
    CabinState m_currentState;
    CabinState m_globalState;
    int m_maximumFloor;
    int m_currentFloor;
};

#endif // ELEVATOR_H
