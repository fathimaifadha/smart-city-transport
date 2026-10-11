// Member 3: Nafrin
// Member 3: stored passenger demand, network totals and journey efficiency.
#include "TransportNetwork.h"

void TransportNetwork::displayDemand()
{
    cout << "\nPASSENGER DEMAND\n";
    if (demands.empty()) cout << "No demand records available.\n";
    for (size_t i = 0; i < demands.size(); ++i)
    {
        const PassengerDemand& demand = demands[i];
        cout << "\nRecord " << i + 1 << "\nTime       : " << demand.time
             << "\nFrom       : " << demand.from << "\nTo         : " << demand.to
             << "\nPassengers : " << demand.passengers << "\n";
    }

}

void TransportNetwork::profiling()
{
    // Formatting here must not affect later menu options.
    ios::fmtflags originalFlags = cout.flags();
    streamsize originalPrecision = cout.precision();
    cout << fixed << setprecision(2);
    int totalRoutes = 0, busRoutes = 0, trainRoutes = 0;
    double totalDistance = 0, totalTravelTime = 0;
    for (const auto& location : network)
        for (const Route& route : location.second)
        {
            ++totalRoutes;
            totalDistance += route.distance;
            totalTravelTime += route.travelTime;
            if (route.transport == "Bus") ++busRoutes;
            if (route.transport == "Train") ++trainRoutes;
        }
    totalRoutes /= 2;
    busRoutes /= 2;
    trainRoutes /= 2;
    totalDistance /= 2;
    totalTravelTime /= 2;
    long long totalPassengers = 0;
    for (const PassengerDemand& demand : demands) totalPassengers += demand.passengers;
    cout << "\nSYSTEM PROFILING\n----------------------------------------------------\nNETWORK SUMMARY\n"
         << "Total locations: " << network.size()
         << "\nTotal routes: " << totalRoutes << "\nBus routes: " << busRoutes
         << "\nTrain routes: " << trainRoutes << "\nTotal route distance: " << totalDistance << " km"
         << "\nAverage route time: " << (totalRoutes ? totalTravelTime / totalRoutes : 0.0) << " minutes"
         << "\nStored demand records: " << demands.size()
         << "\nTotal stored passenger demand: " << totalPassengers << "\n";
    if (!hasLastJourney)
        cout << "\nPlan a journey with option 6 first to see its efficiency comparison.\n";
    else
    {
        JourneyResult combined = calculateRoute(lastFrom, lastTo);
        cout << "\nLAST SUCCESSFULLY PLANNED JOURNEY\n" << lastFrom << " -> " << lastTo
             << "\nDeparture: " << lastDeparture << " | Passengers: " << lastPassengerCount
             << "\n\nTRAVEL-TIME COMPARISON\n";
        if (!combined.found) cout << "Journey is no longer reachable.\n";
        else
        {
            cout << "Combined network: " << combined.minutes << " minutes\n"
                 << "  Path: ";
            for (size_t i = 0; i < combined.locations.size(); ++i)
                cout << (i ? " -> " : "") << combined.locations[i];
            cout << "\n";
            for (const string& mode : vector<string>{"Bus", "Train"})
            {
                JourneyResult alternative = calculateRoute(lastFrom, lastTo, mode);
                cout << "\n" << mode << " only: ";
                if (!alternative.found)
                {
                    cout << "No route available; time saving not applicable.\n";
                    continue;
                }
                cout << alternative.minutes << " minutes\n  Path: ";
                for (size_t i = 0; i < alternative.locations.size(); ++i)
                    cout << (i ? " -> " : "") << alternative.locations[i];
                cout << "\n  Time saved versus " << mode << " only: "
                     << alternative.minutes - combined.minutes << " minutes\n";
            }
            cout << "\nCAPACITY UTILISATION\n";
            if (combined.legs.empty()) cout << "No travel needed; capacity utilisation is not applicable.\n";
            for (size_t i = 0; i < combined.legs.size(); ++i)
            {
                const Route& leg = combined.legs[i];
                cout << "\nLeg " << i + 1 << ": " << combined.locations[i] << " -> " << leg.destination
                     << " | " << leg.transport << "\n";
                if (leg.capacity <= 0)
                {
                    cout << "Capacity unavailable.\n";
                    continue;
                }
                long long vehicles = lastPassengerCount / leg.capacity + (lastPassengerCount % leg.capacity != 0);
                long long places = vehicles * leg.capacity;
                double utilisation = round(1000.0 * lastPassengerCount / places) / 10.0;
                cout << "  Vehicles: " << vehicles << " | Available capacity: " << places
                     << "\n  Passengers: " << lastPassengerCount
                     << " | Unused capacity: " << places - lastPassengerCount
                     << "\n  Utilisation: " << setprecision(1) << utilisation << "%\n" << setprecision(2);
            }

        }
    }
    cout.flags(originalFlags);
    cout.precision(originalPrecision);
}
