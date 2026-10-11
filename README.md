# Smart City Transport

A C++ console project for modelling a bus and train network, finding the fastest
route, planning passenger journeys and comparing transport efficiency.

## Team responsibilities

| Member | Name | Responsibility | Files |
| --- | --- | --- | --- |
| 1 | Nahla | Graph core: build the graph, display routes and locations | `src/TransportNetwork.h`, `src/network.cpp` |
| 2 | Amna | BFS, Dijkstra, timed journeys and vehicle counts | `src/algorithms.cpp` |
| 3 | Nafrin | Passenger demand and system profiling | `src/demand_profiling.cpp` |
| 4 | Ifadha | Menu, input checking, city data and documentation | `src/main.cpp`, `README.md` |

## Folder structure

```text
smart-city-transport/
|-- README.md
|-- .gitignore
|-- SmartCityTransport.dev
`-- src/
    |-- TransportNetwork.h
    |-- network.cpp
    |-- algorithms.cpp
    |-- demand_profiling.cpp
    `-- main.cpp
```

## Build and run

Requires a C++11 compiler or later.

### Dev-C++ on Windows

1. Open **SmartCityTransport.dev** in Dev-C++.
2. Press **F11** to compile and run the full project.
3. Select a menu number and follow the prompts.

Open the project file rather than a single `.cpp` file: all four source files
must be compiled together. The project already enables C++11.

### Command line

From this folder, compile with:

```sh
g++ -std=c++11 -Wall -Wextra src/main.cpp src/network.cpp src/algorithms.cpp src/demand_profiling.cpp -o SmartCityTransport.exe
```

Run `./SmartCityTransport.exe` in PowerShell or a Unix shell, or
`SmartCityTransport.exe` in Windows Command Prompt.

## Menu

| Option | What it does |
| --- | --- |
| 1 | Display the complete transport network |
| 2 | Display bus routes |
| 3 | Display train routes |
| 4 | Display stored passenger-demand records |
| 5 | Find the fastest route between two locations using Dijkstra |
| 6 | Plan a stored or custom journey, showing times and vehicle counts |
| 7 | Traverse the network using BFS; this does not minimise travel time |
| 8 | Show network totals and compare the last successful option-6 journey |
| 9 | List valid location names |
| 10 | Exit |

Each selection clears the previous result and displays the full menu above the
new output. Location names are case sensitive; use option 9 for exact spelling.

## Example: plan a journey

Choose **6**, then **2** for a custom journey. Enter:

```text
Starting location: Nugegoda
Destination: Colombo Fort
Departure time: 07:00
Passengers: 1780
```

The fastest route takes **18 minutes**:

| Leg | Time | Route | Transport | Vehicles |
| --- | --- | --- | --- | --- |
| 1 | 07:00–07:09 | Nugegoda → Borella | Bus | 30 |
| 2 | 07:09–07:13 | Borella → Maradana | Bus | 30 |
| 3 | 07:13–07:18 | Maradana → Colombo Fort | Train | 3 |

Bus capacity is **60 passengers**; train capacity is **800 passengers**.
Vehicle counts are rounded up: 1,780 / 60 needs 30 buses, and 1,780 / 800
needs 3 trains. A leg is one connection in the journey.

Choose **8** afterwards to compare the combined route (18 minutes), bus-only
route (27 minutes) and train-only route (41 minutes). Profiling also shows
available capacity, unused seats and utilisation for each leg.

For stored demand, choose **6 → 1**, then a record number. Option 4 shows all
six records, including different passenger counts at 07:00, 13:00 and 17:00.

## How it works

- An undirected weighted adjacency-list graph stores eight locations and twelve
  connections: seven bus routes and five train routes.
- Dijkstra minimises total travel time. A route can use buses, trains or both.
- BFS visits neighbouring locations using a queue and prints comma-separated names.
- Option 6 uses the fastest path and calculates each leg's timetable and vehicle needs.
- Option 8 compares the last successful option-6 journey with routes restricted
  to buses or trains. An unavailable alternative is reported as unavailable.
- Capacity utilisation = passengers / total capacity of required vehicles × 100.

## Model assumptions

All routes, travel times and demand records are fictional demonstration data.
Travel times are fixed: departure time changes the clock times, not traffic or
the selected path. Waiting and transfer delays are zero, with enough vehicles
assumed available at each leg. Vehicle counts describe each leg; vehicle reuse
and the number of distinct vehicles in a fleet are not modelled.

Custom journeys do not change the stored demand total of 840 passengers.
Option 5 does not replace the journey used by profiling. Invalid journey input
preserves the last successful journey. Midnight arrivals show a day offset.

## Upload to GitHub

Create an empty GitHub repository, open a terminal in this folder and run:

```sh
git init
git add .
git commit -m "Add smart city transport project"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/smart-city-transport.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username and use your repository URL.
The `.gitignore` keeps executables, object files and generated editor files out
of commits. Source files, the Dev-C++ project and documentation are included.
