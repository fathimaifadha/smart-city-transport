#include "../src/TransportNetwork.h"
#include <cassert>

int main()
{
    ostringstream captured;
    streambuf* original = cout.rdbuf(captured.rdbuf());
    TransportNetwork empty;
    empty.profiling();
    empty.displayDemand();
    empty.simulateStoredJourney(1);
    assert(captured.str().find("Average route time: 0.00") != string::npos);
    assert(captured.str().find("No demand records available.") != string::npos);

    TransportNetwork disconnected;
    disconnected.addLocation("A");
    disconnected.addLocation("B");
    disconnected.simulatePassengerJourney("A", "B", 10, "07:00");
    assert(captured.str().find("No route exists.") != string::npos);

    TransportNetwork fractional;
    fractional.addRoute("A", "B", 1, 1.5, "Bus", 60);
    fractional.simulatePassengerJourney("A", "B", 61, "23:59");
    assert(captured.str().find("Arrival: 00:00:30 (+1 day)") != string::npos);
    assert(captured.str().find("Buses required: 2") != string::npos);

    TransportNetwork invalidCapacity;
    invalidCapacity.addRoute("A", "B", 1, 1, "Bus", 0);
    invalidCapacity.simulatePassengerJourney("A", "B", 10, "07:00");
    assert(captured.str().find("capacity is unavailable") != string::npos);
    cout.rdbuf(original);
    cout << "Empty, disconnected, fractional-time and invalid-capacity checks passed.\n";
}
