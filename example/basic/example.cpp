#include "Valve.hpp"

#include <iostream>
#include <chrono>
#include <ostream>
#include <thread>

namespace microhsm_examples
{

    static ValveContext valve = ValveContext();
    static ValveHSM hsm = ValveHSM();

    const char* getStateName(eValveState s)
    {
        switch(s) {
            case eSTATE_IDLE: return "IDLE";
            case eSTATE_RUNNING: return "RUNNING";
            case eSTATE_OPEN: return "OPEN";
            case eSTATE_CLOSED: return "CLOSED";
            default: return "UNKNOWN";
        }
    }

    const char* getEventName(eValveEvent e)
    {
        switch(e) {
            case eEVENT_ANONYMOUS: return "ANONYMOUS";
            case eEVENT_START: return "START";
            case eEVENT_TICK: return "TICK";
            case eEVENT_PAUSE: return "PAUSE";
            default: return "UNKNOWN";
        }
    }

    void sleepMs(unsigned int ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    void printEvent(unsigned int event)
    {
        eValveEvent e = static_cast<eValveEvent>(event);
        std::cout << "ValveHSM Event: " << getEventName(e) << std::endl;
    }

    void printState()
    {
        eValveState s = static_cast<eValveState>(hsm.getCurrentState()->ID);
        std::cout << "ValveHSM State: " << getStateName(s) << std::endl;
    }

    void dispatchEvent(unsigned int event)
    {
        std::cout << std::endl;
        printEvent(event);
        hsm.dispatch(event, static_cast<void*>(&valve));
        printState();
    }

    int runExample()
    {
        std::cout << "Running Example: Basic" << std::endl;
        printState();

        const unsigned int sleepTimeMs = 750;

        std::cout << std::endl << "--- Example: starting valve" << std::endl;
        // Start valve
        dispatchEvent(eEVENT_START);
        sleepMs(sleepTimeMs);

        for (int i = 0; i < 3; i++) {
            dispatchEvent(eEVENT_TICK);
            sleepMs(sleepTimeMs);
        }

        // Locking valve
        std::cout << std::endl << "--- Example: locking valve" << std::endl;
        valve.lock();
        sleepMs(sleepTimeMs);

        for (int i = 0; i < 3; i++) {
            dispatchEvent(eEVENT_TICK);
            sleepMs(sleepTimeMs);
        }

        // Locking valve
        std::cout << std::endl << "--- Example: unlocking valve" << std::endl;
        valve.unlock();
        sleepMs(sleepTimeMs);

        for (int i = 0; i < 3; i++) {
            dispatchEvent(eEVENT_TICK);
            sleepMs(sleepTimeMs);
        }

        std::cout << std::endl << "--- Example: pause" << std::endl;
        dispatchEvent(eEVENT_PAUSE);
        sleepMs(sleepTimeMs);

        for (int i = 0; i < 3; i++) {
            dispatchEvent(eEVENT_TICK);
            sleepMs(sleepTimeMs);
        }
        return 0;
    }

}

int main()
{
    return microhsm_examples::runExample();
}
