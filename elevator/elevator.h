#include <list>

enum CabinState {
    StateUndefined,
    StateStopped,
    StateMovingUp,
    StateMovingDown
};

class PLC {
public:
    PLC();
    ~PLC();
    
    void reqFloorInt(int floor);
    void reqFloorExt(int floor);
    void doAdvance();
    void doStop();
    
    int getCurrentFloor();
    CabinState getCabinState();
    
private:
    std::list<int> m_requests;
    CabinState m_state;
};