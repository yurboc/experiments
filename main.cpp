#include "elevator.h"

#include <iostream>     // cin, cout
#include <errno.h>      // EINVAL
#include <unistd.h>     // STDIN_FILENO
#include <sys/select.h> // select
#include <stdlib.h>     // srand, rand
#include <time.h>       // time

//
// Type of user request
//   mode : source of request (F - from floor, C - from cabin)
//   floor: requested floor
//
struct UserInput {
    char mode;
    int floor;

    UserInput() { mode = '\0'; floor = 0; }
};

//
// Print current cabin state to stdout
//
void showCabinState(int floor, CabinState state)
{
    // Show current floor
    std::cout << "Cabin on floor " << floor << ". ";

    // Show cabin state
    switch (state) {
        case StateStopped:
            std::cout << "Stopped. Doors opened.";
            break;
        case StateMovingUp:
            std::cout << "Going up.";
            break;
        case StateMovingDown:
            std::cout << "Going down.";
            break;
        default:
            std::cout << "STATE ERROR! " << state;
            break;
    }

    // Show prompt
    std::cout << " <Press any key to enter new command>" << std::endl;
}

//
// Read request from stdin
//
bool readUserInput(UserInput *data)
{
    // Check pointer to data
    if (data == NULL) {
        return false;
    }

    // Wait 1 second for command
    struct timeval tv;
    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (STDIN_FILENO, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int result = select (STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    // Check for timeout
    if (result == 0)
        return false;

    // Parse command
    char mode = 0;
    int floor = 0;
    std::cin >> mode >> floor;

    // Check command type
    if (mode != 'C' && mode != 'c' && mode != 'F' && mode != 'f') {
        std::cout << "Wrong mode '" << mode << "' (allowed: 'C', 'F')" << std::endl;
        return false;
    }

    // Check floor number
    if (floor < 1) {
        std::cout << "Wrong number of floor: " << floor << std::endl;
        return false;
    }

    // Store result
    data->mode = mode;
    data->floor = floor;
    return true;
}

//
// Main
//
int main(int argc, char** argv)
{
    // Request floors count
    int nFloors = 0;
    std::cout << "How many floors?" << std::endl;
    std::cin >> nFloors;

    // Check floors count
    if (nFloors < 1) {
        std::cout << "Wrong data. Exit." << std::endl;
        return -EINVAL;
    }

    // Initialize random seed
    srand(time(NULL));

    // Set random current floor
    int startFloor = rand() % nFloors + 1;

    // Create objects
    Elevator elevator(nFloors, startFloor);

    // Simulation
    bool showStoppedCabin = true;
    while (1) {

        // Move cabin
        elevator.doAdvance();

        // Show cabin state (running --> always, stopped --> once)
        if (elevator.state() == StateStopped) {
            if (showStoppedCabin) {
                showStoppedCabin = false;
                showCabinState(elevator.floor(), elevator.state());
            }
        }
        else {
            showStoppedCabin = true;
            showCabinState(elevator.floor(), elevator.state());
        }

        // Read user input
        UserInput data;
        readUserInput(&data);
        if (data.mode == 'c')
            elevator.reqFromCabin(data.floor);
        else if (data.mode == 'f')
            elevator.reqFromFloor(data.floor);
    }

	return 0;
}
