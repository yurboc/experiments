#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>

//
// State of Elevator Cabin
//
enum CabinState {
    StateStopped,
    StateMovingUp,
    StateMovingDown
};

//
// Record for Floor requests
//   reqFromCabin : button on the Cabin control panel
//   reqUp        : button on the floor (want to go up)
//   reqDown      : button on the floor (want to go down)
//
struct FloorState {
    bool reqFromCabin;
    bool reqUp;
    bool reqDown;
    FloorState() : reqFromCabin(false), reqUp(false), reqDown(false) {}
};

//
// Elevator control class
//
class Elevator {
public:
    Elevator(int nFloors, int startFloor=1);
    ~Elevator();

    void reqFromCabin(int floor); // Add request from cabin
    void reqFromFloor(int floor); // Add request from floor
    void doAdvance();             // Move cabin if need

    int floor() const;            // Get current Cabin floor
    CabinState state() const;     // Get current Cabin state

private:

    // Find floor near the Cabin
    //   reqUp       : find requests from floors "want to go up"
    //   reqDown     : find requests from floors "want to go down"
    //   reqInternal : find requests from the Cabin
    //   searchUp    : find requests above Cabin
    //   searchDown  : find requests below Cabin
    int findNearFloor(bool reqUp, bool reqDown, bool reqInternal,
                      bool searchUp, bool searchDown) const;

    // Start Cabin moving (from condition "no requests")
    void doStartMoving();

    // Continue moving (from conditions "going up" or "going down")
    void doContinueMoving();

    // Resume moving (from condition "stopped on floor" and has requests)
    void doResumeMoving();

    std::vector<FloorState> m_floors; // State of all floors
    CabinState m_currentState;        // Current Cabin state
    CabinState m_globalState;         // Cabin state (persist when Cabin stops)
    int m_maximumFloor;               // Last floor number
    int m_currentFloor;               // Current Cabin floor
};

#endif // ELEVATOR_H
