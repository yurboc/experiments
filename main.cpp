#include "elevator.h"

#include <cstdio>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

struct UserInput {
    char mode; // F - from floor, C - from cabin
    int floor; // requested floor

    UserInput() { mode = '\0'; floor = 0; }
};

void showCabinState(int floor, CabinState state)
{
    // Current floor
    std::cout << "Cabin on floor " << floor << ". ";

    // Cabin state
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
    std::cout << " <Press Enter for command>" << std::endl;
}

bool readUserInput(UserInput *data)
{
    // Check pointer
    if (data == NULL) {
        return false;
    }

    // Read next char
    int c = getchar();
    if (c < 0) {
        std::cout << "Got EOF" << std::endl;
        return false;
    }
    else {
        std::cout << "Got char " << c << std::endl;
        return true;
    }
}

int main(int argc, char** argv)
{
    // Request floors count
    int nFloors = 0;
    std::cout << "How many floors?" << std::endl;
    std::cin >> nFloors;
    if (nFloors < 1) {
        std::cout << "Wrong data. Exit." << std::endl;
        return -EINVAL;
    }

    // Turn off blocking flag in stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);

    // Create objects
	Elevator elevator(nFloors);

    // Simulation
    while (1) {

        // Show cabin state
        showCabinState(elevator.floor(), elevator.state());

        // Wait for user input
        sleep(1);

        // Read user input
        UserInput data;
        readUserInput(&data);
        if (data.mode == 'c')
            elevator.reqFromCabin(data.floor);
        else if (data.mode == 'f')
            elevator.reqFromFloor(data.floor);

        // Move cabin
        elevator.doAdvance();
    }

	return 0;
}
