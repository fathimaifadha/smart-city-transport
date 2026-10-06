// demand_profiling.cpp
// Member 3: passenger demand and system profiling.
#include "TransportNetwork.h"

void TransportNetwork::displayDemand()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "                PASSENGER DEMAND\n";
    cout << "====================================================\n";


    for (auto &demand : demands)
    {
        cout << "\nTime       : "
             << demand.time;

        cout << "\nFrom       : "
             << demand.from;

        cout << "\nTo         : "
             << demand.to;

        cout << "\nPassengers : "
             << demand.passengers;

        cout << "\n";
    }
}


void TransportNetwork::profiling()
{
    cout << "\n";
    cout << "====================================================\n";
    cout << "              SYSTEM PROFILING\n";
    cout << "====================================================\n";


    int totalLocations =
        network.size();


    int totalRoutes = 0;

    int busRoutes = 0;

    int trainRoutes = 0;


    double totalDistance = 0;

    double totalTravelTime = 0;


    // Since each route is stored in both directions,
    // divide totals by 2 later.

    for (auto &location : network)
    {
        for (auto &route : location.second)
        {
            totalRoutes++;

            totalDistance +=
                route.distance;

            totalTravelTime +=
                route.travelTime;


            if (route.transport == "Bus")
            {
                busRoutes++;
            }
            else if (route.transport == "Train")
            {
                trainRoutes++;
            }
        }
    }


    totalRoutes /= 2;

    busRoutes /= 2;

    trainRoutes /= 2;


    totalDistance /= 2;

    totalTravelTime /= 2;


    // ====================================================
    // TOTAL PASSENGER DEMAND
    // ====================================================

    int totalPassengers = 0;


    for (auto &demand : demands)
    {
        totalPassengers +=
            demand.passengers;
    }


    // ====================================================
    // DISPLAY PROFILE
    // ====================================================

    cout << fixed << setprecision(2);


    cout << "\nTotal locations       : "
         << totalLocations;

    cout << "\nTotal routes          : "
         << totalRoutes;

    cout << "\nBus routes            : "
         << busRoutes;

    cout << "\nTrain routes          : "
         << trainRoutes;

    cout << "\nTotal route distance  : "
         << totalDistance
         << " km";

    cout << "\nAverage route time    : "
         << (totalRoutes > 0 ? totalTravelTime / totalRoutes : 0.0)
         << " minutes";

    cout << "\nTotal passenger demand: "
         << totalPassengers;


    cout << "\n\n";


    // ====================================================
    // EFFICIENCY MEASUREMENT
    // ====================================================

    double averageDemandPerRoute =
        (totalRoutes > 0 ? (double)totalPassengers / totalRoutes : 0.0);


    cout << "Average passengers per route: "
         << averageDemandPerRoute;


    cout << "\n\n";


    if (averageDemandPerRoute < 50)
    {
        cout << "Network status: Low demand\n";
    }
    else if (averageDemandPerRoute < 150)
    {
        cout << "Network status: Moderate demand\n";
    }
    else
    {
        cout << "Network status: High demand\n";
    }
}

