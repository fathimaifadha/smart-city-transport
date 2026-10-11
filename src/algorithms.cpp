// Member 2: Amna
// Member 2: BFS, shared Dijkstra search, timed passenger journeys.
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
    cout << "\nBFS traversal starting from " << start << ":\n\n";
    bool first = true;
    while (!q.empty())
    {
        string current = q.front();
        q.pop();
        if (!first) cout << ", ";
        cout << current;
        first = false;
        for (const auto& route : network[current])
            if (!visited[route.destination])
            {
                visited[route.destination] = true;
                q.push(route.destination);
            }
    }
    cout << "\n";
}

TransportNetwork::JourneyResult TransportNetwork::calculateRoute(
    const string& from, const string& to, const string& mode) const
{
    JourneyResult result;
    if (network.find(from) == network.end() || network.find(to) == network.end())
        return result;
    map<string, double> times;
    map<string, pair<string, size_t>> previous;
    for (const auto& location : network)
        times[location.first] = numeric_limits<double>::infinity();
    times[from] = 0;
    priority_queue<pair<double, string>, vector<pair<double, string>>,
                   greater<pair<double, string>>> pending;
    pending.push(make_pair(0.0, from));
    while (!pending.empty())
    {
        double time = pending.top().first;
        string current = pending.top().second;
        pending.pop();
        if (time > times[current]) continue;
        const vector<Route>& routes = network.at(current);
        for (size_t i = 0; i < routes.size(); ++i)
        {
            const Route& route = routes[i];
            // An empty filter allows both buses and trains.
            if (!mode.empty() && route.transport != mode) continue;
            double candidate = time + route.travelTime;
            if (candidate < times[route.destination])
            {
                times[route.destination] = candidate;
                previous[route.destination] = make_pair(current, i);
                pending.push(make_pair(candidate, route.destination));
            }
        }
    }
    if (!isfinite(times[to])) return result;
    result.found = true;
    result.minutes = times[to];
    string current = to;
    result.locations.push_back(current);
    while (current != from)
    {
        const pair<string, size_t>& p = previous.at(current);
        result.legs.push_back(network.at(p.first).at(p.second));
        current = p.first;
        result.locations.push_back(current);
    }
    reverse(result.locations.begin(), result.locations.end());
    reverse(result.legs.begin(), result.legs.end());
    return result;
}

void TransportNetwork::findFastestRoute(string start, string destination, int passengers)
{
    if (network.find(start) == network.end() || network.find(destination) == network.end())
    {
        cout << "\nLocation not found.\n";
        return;
    }
    JourneyResult result = calculateRoute(start, destination);
    if (!result.found)
    {
        cout << "\nNo route exists.\n";
        return;
    }
    cout << "\nFASTEST ROUTE\n";
    for (size_t i = 0; i < result.locations.size(); ++i)
        cout << (i ? " -> " : "") << result.locations[i];
    cout << "\nTotal travel time: " << result.minutes << " minutes\n";
    if (result.legs.empty()) cout << "Already at destination. Vehicles required: 0\n";
    for (size_t i = 0; i < result.legs.size(); ++i)
    {
        const Route& leg = result.legs[i];
        cout << result.locations[i] << " -> " << leg.destination
             << " : " << leg.transport << " | " << leg.travelTime << " min\n";
        if (passengers > 0 && leg.capacity > 0)
            cout << "   Vehicles required for this leg: "
                 << passengers / leg.capacity + (passengers % leg.capacity != 0) << "\n";
    }
}

bool TransportNetwork::parseDeparture(const string& time, int& minutes)
{
    if (time.size() != 5 || time[2] != ':') return false;
    for (size_t i = 0; i < time.size(); ++i)
        if (i != 2 && (time[i] < '0' || time[i] > '9')) return false;
    int hours = (time[0] - '0') * 10 + time[1] - '0';
    int mins = (time[3] - '0') * 10 + time[4] - '0';
    if (hours > 23 || mins > 59) return false;
    minutes = hours * 60 + mins;
    return true;
}

string TransportNetwork::clockTime(double minutesAfterMidnight)
{
    // Preserve fractional route minutes as seconds, and handle midnight crossings.
    long long seconds = llround(minutesAfterMidnight * 60);
    long long day = seconds / 86400;
    seconds %= 86400;
    ostringstream text;
    text << setfill('0') << setw(2) << seconds / 3600 << ':'
         << setw(2) << seconds / 60 % 60;
    if (seconds % 60) text << ':' << setw(2) << seconds % 60;
    if (day) text << " (+" << day << (day == 1 ? " day)" : " days)");
    return text.str();
}

void TransportNetwork::simulatePassengerJourney(
    string from, string to, int passengerCount, string departure)
{
    int departureMinutes;
    if (passengerCount <= 0)
    {
        cout << "\nPassenger count must be a positive whole number.\n";
        return;
    }
    if (!parseDeparture(departure, departureMinutes))
    {
        cout << "\nInvalid departure time. Use HH:MM (00:00 to 23:59).\n";
        return;
    }
    if (network.find(from) == network.end() || network.find(to) == network.end())
    {
        cout << "\nLocation not found.\n";
        return;
    }
    JourneyResult result = calculateRoute(from, to);
    if (!result.found)
    {
        cout << "\nNo route exists.\n";
        return;
    }
    for (const Route& leg : result.legs)
        if (leg.capacity <= 0)
        {
            cout << "\nCannot plan journey: vehicle capacity is unavailable.\n";
            return;
        }
    lastFrom = from;
    lastTo = to;
    lastDeparture = departure;
    lastPassengerCount = passengerCount;
    hasLastJourney = true;
    cout << "\nPASSENGER JOURNEY\n" << from << " -> " << to
         << "\nDeparture: " << departure << "\nPassengers: " << passengerCount
         << "\n\n";
    double elapsed = 0;
    for (size_t i = 0; i < result.legs.size(); ++i)
    {
        const Route& leg = result.legs[i];
        int vehicles = passengerCount / leg.capacity + (passengerCount % leg.capacity != 0);
        cout << "Leg " << i + 1 << ": " << clockTime(departureMinutes + elapsed)
             << " - " << clockTime(departureMinutes + elapsed + leg.travelTime)
             << " | " << result.locations[i] << " -> " << leg.destination
             << " | " << leg.transport << " | " << leg.travelTime << " min\n"
             << "   " << (leg.transport == "Bus" ? "Buses" : "Trains")
             << " required: " << vehicles << " | Capacity per vehicle: " << leg.capacity << "\n";
        elapsed += leg.travelTime;
    }
    if (result.legs.empty()) cout << "Already at destination. Vehicles required: 0\n";
    cout << "\nArrival: " << clockTime(departureMinutes + result.minutes)
         << "\nTotal travel time: " << result.minutes << " minutes\n";
}

void TransportNetwork::simulateStoredJourney(int recordNumber)
{
    if (recordNumber < 1 || static_cast<size_t>(recordNumber) > demands.size())
    {
        cout << "\nInvalid demand record number.\n";
        return;
    }
    const PassengerDemand& demand = demands[recordNumber - 1];

    simulatePassengerJourney(demand.from, demand.to, demand.passengers, demand.time);
}
