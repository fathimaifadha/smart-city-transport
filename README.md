# Smart City Public Transport System (ADSA Problem C)

A C++ console program that models an imaginary city with only public transport
(buses and trains) as a weighted graph. It shows the bus and train networks,
passenger demand at different times of the day, the fastest route between two
locations (Dijkstra), BFS traversal and some profiling of the system.

All route and demand data is fictional.

## Files

| File | What it contains | Owner |
|------|------------------|-------|
| `src/TransportNetwork.h` | `Route`, `PassengerDemand` and the `TransportNetwork` class declaration | Member 1 |
| `src/network.cpp` | building the graph, displaying routes and locations | Member 1 |
| `src/algorithms.cpp` | BFS, Dijkstra fastest route, vehicle counts | Member 2 |
| `src/demand_profiling.cpp` | passenger demand display and system profiling | Member 3 |
| `src/main.cpp` | menu, input checking, city data | Member 4 |

## Build and run

Needs a C++ compiler (g++ with C++17).

    g++ -std=c++17 -o smartcity src/main.cpp src/network.cpp src/algorithms.cpp src/demand_profiling.cpp
    ./smartcity            (Windows: smartcity.exe)

or `make` then `./smartcity`.

Location names are case sensitive (for example `Colombo Fort`). Use menu
option 9 to see all names.

## Group

See `CONTRIBUTIONS.md`.
