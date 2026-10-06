// algorithms.cpp
// Member 2: graph algorithms.
// BFS traversal, Dijkstra fastest route, vehicle counts for a journey.
#include "TransportNetwork.h"

void TransportNetwork::BFS(string start)
{
    if (network.find(start) == network.end())
    {
        cout << "\nLocation not found.\n";
        return;
    }

    map<string, bool> visited;

    queue<string> q;

    visited[start] = true;

    q.push(start);

    cout << "\nBFS traversal starting from "
         << start << ":\n\n";

    bool firstLocation = true;
    while (!q.empty())
    {
        string current = q.front();

        q.pop();

        if (!firstLocation)
            cout << ", ";
        cout << current;
        firstLocation = false;

        for (auto &route : network[current])
        {
            if (!visited[route.destination])
            {
                visited[route.destination] = true;

                q.push(route.destination);
            }
        }
    }

    cout << "\n";
}


void TransportNetwork::findFastestRoute(
    string start,
    string destination,
    int passengers)
{
    if (network.find(start) == network.end() ||
        network.find(destination) == network.end())
    {
        cout << "\nLocation not found.\n";
        return;
    }


    const double INF =
        numeric_limits<double>::infinity();


    // Shortest travel time to each location
    map<string, double> distance;

    // Previous location used to reconstruct path
    map<string, string> previous;

    // Transport used to reach each location
    map<string, string> previousTransport;
    map<string, int> previousCapacity;


    for (auto &location : network)
    {
        distance[location.first] = INF;
    }


    if (start == destination)
    {
        cout << "\nYou are already at the destination. Travel time: 0 minutes.\n";
        if (passengers > 0) cout << "Vehicles required: 0\n";
        return;
    }
    distance[start] = 0;


    // Priority queue:
    // <travel time, location>
    priority_queue<
        pair<double, string>,
        vector<pair<double, string>>,
        greater<pair<double, string>>
    > pq;


    pq.push({0, start});


    while (!pq.empty())
    {
        double currentTime =
            pq.top().first;

        string current =
            pq.top().second;

        pq.pop();


        if (currentTime >
            distance[current])
        {
            continue;
        }


        for (auto &route : network[current])
        {
            double newTime =
                currentTime +
                route.travelTime;


            if (newTime <
                distance[route.destination])
            {
                distance[route.destination] =
                    newTime;

                previous[route.destination] =
                    current;

                previousTransport[
                    route.destination
                ] = route.transport;
                previousCapacity[route.destination] = route.capacity;


                pq.push(
                    {
                        newTime,
                        route.destination
                    }
                );
            }
        }
    }


    // No route exists
    if (distance[destination] == INF)
    {
        cout << "\nNo route exists.\n";
        return;
    }


    // ====================================================
    // RECONSTRUCT PATH
    // ====================================================

    vector<string> path;

    string current = destination;


    while (current != start)
    {
        path.push_back(current);

        current = previous[current];
    }


    path.push_back(start);


    reverse(
        path.begin(),
        path.end()
    );


    // ====================================================
    // DISPLAY RESULT
    // ====================================================

    cout << "\n";
    cout << "====================================================\n";
    cout << "                 FASTEST ROUTE\n";
    cout << "====================================================\n";


    cout << "\nJourney:\n\n";


    for (size_t i = 0;
         i < path.size();
         i++)
    {
        cout << path[i];

        if (i < path.size() - 1)
        {
            cout << " -> ";
        }
    }


    cout << "\n\n";


    cout << "Total travel time: "
         << distance[destination]
         << " minutes\n";


    // ====================================================
    // DISPLAY TRANSPORTS
    // ====================================================

    cout << "\nTransport modes:\n";


    for (size_t i = 1;
         i < path.size();
         i++)
    {
        cout << path[i - 1]
             << " -> "
             << path[i]
             << " : "
             << previousTransport[path[i]]
             << "\n";
        if (passengers > 0)
        {
            int capacity = previousCapacity[path[i]];
            if (capacity > 0)
            {
                int vehicles = passengers / capacity + (passengers % capacity != 0);
                cout << "   " << (previousTransport[path[i]] == "Bus" ? "Buses" : "Trains") << " required for this leg: " << vehicles << "\n";
            }
            else
            {
                cout << "   Vehicle capacity is unavailable.\n";
            }
        }
    }
}


void TransportNetwork::simulatePassengerJourney(string from, string to, int passengerCount)
{
    if (passengerCount <= 0)
    {
        cout << "\nPassenger count must be a positive whole number.\n";
        return;
    }
    cout << "\nPassengers: " << passengerCount << "\n";
    findFastestRoute(from, to, passengerCount);
}

