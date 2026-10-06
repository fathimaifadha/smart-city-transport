// Member 1: Nahla
// network.cpp
// Member 1: graph core.
// Builds the graph (adjacency list) and displays locations and routes.
#include "TransportNetwork.h"

void TransportNetwork::addLocation(string location)
{
    if (network.find(location) == network.end())
    {
        network[location] = {};
    }
}


void TransportNetwork::addRoute(
    string from,
    string to,
    double distance,
    double travelTime,
    string transport,
    int capacity)
{
    addLocation(from);
    addLocation(to);

    // Because transport routes are assumed to work
    // in both directions, we add two edges.

    network[from].push_back(
        Route(
            to,
            distance,
            travelTime,
            transport,
            capacity
        )
    );

    network[to].push_back(
        Route(
            from,
            distance,
            travelTime,
            transport,
            capacity
        )
    );
}


void TransportNetwork::addDemand(
    string from,
    string to,
    string time,
    int passengers)
{
    demands.push_back(
        PassengerDemand(
            from,
            to,
            time,
            passengers
        )
    );
}


void TransportNetwork::displayNetwork()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "              SMART CITY TRANSPORT NETWORK\n";
    cout << "====================================================\n";

    for (auto &location : network)
    {
        cout << "\n[" << location.first << "]";

        for (auto &route : location.second)
        {
            cout << "\n   -> "
                 << route.destination
                 << " | "
                 << route.transport
                 << " | Distance: "
                 << route.distance
                 << " km"
                 << " | Time: "
                 << route.travelTime
                 << " min"
                 << " | Capacity: "
                 << route.capacity;
        }

        cout << "\n";
    }
}


void TransportNetwork::displayBusRoutes()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "                    BUS NETWORK\n";
    cout << "====================================================\n";

    for (auto &location : network)
    {
        for (auto &route : location.second)
        {
            if (route.transport == "Bus")
            {
                cout << location.first
                     << " -> "
                     << route.destination
                     << " | "
                     << route.distance
                     << " km | "
                     << route.travelTime
                     << " min | Capacity: "
                     << route.capacity
                     << "\n";
            }
        }
    }
}


void TransportNetwork::displayTrainRoutes()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "                   TRAIN NETWORK\n";
    cout << "====================================================\n";

    for (auto &location : network)
    {
        for (auto &route : location.second)
        {
            if (route.transport == "Train")
            {
                cout << location.first
                     << " -> "
                     << route.destination
                     << " | "
                     << route.distance
                     << " km | "
                     << route.travelTime
                     << " min | Capacity: "
                     << route.capacity
                     << "\n";
            }
        }
    }
}


void TransportNetwork::displayLocations()
{
    cout << "\nAvailable locations:\n\n";

    for (auto &location : network)
    {
        cout << "- "
             << location.first
             << "\n";
    }
}

