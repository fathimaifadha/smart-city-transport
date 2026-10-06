// TransportNetwork.h
// Shared declarations: Route, PassengerDemand and the TransportNetwork class.
// Owner: Member 1 (graph core). Everybody includes this file.
#ifndef TRANSPORT_NETWORK_H
#define TRANSPORT_NETWORK_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstdlib>

using namespace std;

// ============================================================
// STRUCTURE FOR A TRANSPORT ROUTE
// ============================================================

struct Route
{
    string destination;

    // Distance between two locations in kilometres
    double distance;

    // Travel time in minutes
    double travelTime;

    // "Bus" or "Train"
    string transport;

    // Number of passengers this route can carry
    int capacity;

    Route(string dest,
          double dist,
          double time,
          string type,
          int cap)
    {
        destination = dest;
        distance = dist;
        travelTime = time;
        transport = type;
        capacity = cap;
    }
};


// ============================================================
// STRUCTURE FOR PASSENGER DEMAND
// ============================================================

struct PassengerDemand
{
    string from;
    string to;
    string time;

    int passengers;

    PassengerDemand(string f,
                    string t,
                    string tm,
                    int p)
    {
        from = f;
        to = t;
        time = tm;
        passengers = p;
    }
};


// ============================================================
// TRANSPORT NETWORK CLASS
// ============================================================

class TransportNetwork
{
private:

    // Adjacency list representation of the graph
    map<string, vector<Route>> network;

    // Passenger demand records
    vector<PassengerDemand> demands;


public:

    // ---- Member 1: graph core (network.cpp) ----
    void addLocation(string location);
    void addRoute(string from, string to, double distance,
                  double travelTime, string transport, int capacity);
    void addDemand(string from, string to, string time, int passengers);
    void displayNetwork();
    void displayBusRoutes();
    void displayTrainRoutes();
    void displayLocations();

    // ---- Member 2: algorithms (algorithms.cpp) ----
    void BFS(string start);
    void findFastestRoute(string start, string destination, int passengers = 0);
    void simulatePassengerJourney(string from, string to, int passengerCount);

    // ---- Member 3: demand and profiling (demand_profiling.cpp) ----
    void displayDemand();
    void profiling();
};

#endif
