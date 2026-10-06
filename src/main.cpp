// main.cpp
// Member 4: console interface and city data.
// clearScreen, displayMenu, city data (locations, routes, demand), menu loop,
// input validation. Also owns README, testing and the report.
#include "TransportNetwork.h"
#ifdef _WIN32
#include <io.h>
#endif

// Keep each menu and result on a fresh screen during a console demo.
void clearScreen()
{
#ifdef _WIN32
    if (_isatty(_fileno(stdout)))
        system("cls");
#else
    cout << "\033[2J\033[H" << flush;
#endif
}

void displayMenu()
{
    cout << "\n\n";
    cout << "====================================================\n";
    cout << "          SMART CITY TRANSPORT SYSTEM\n";
    cout << "====================================================\n";

    cout << "\nFictional demo data - not real routes or timetables.\n";
    cout << "Assumed : Bus = 60 passengers; Train = 800 passengers.\n";

    cout << "\n1. Display complete transport network";

    cout << "\n2. Display bus network";

    cout << "\n3. Display train network";

    cout << "\n4. Display passenger demand";

    cout << "\n5. Find fastest route";

    cout << "\n6. Plan passenger journey and vehicle counts";

    cout << "\n7. BFS network traversal";

    cout << "\n8. System profiling";

    cout << "\n9. Display locations";

    cout << "\n10. Exit";


    cout << "\n\n----------------------------------------------------\n";
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    TransportNetwork city;


    // ========================================================
    // CREATE CITY LOCATIONS
    // ========================================================

    city.addLocation("Colombo Fort");
    city.addLocation("Maradana");
    city.addLocation("Borella");
    city.addLocation("Nugegoda");
    city.addLocation("Dehiwala");
    city.addLocation("Wellawatte");
    city.addLocation("Rajagiriya");
    city.addLocation("Kottawa");


    // FICTIONAL DEMO DATA: these are not real routes or timetables.
    // Each addRoute stores both directions. Final value = capacity.
    // Train routes: from, to, kilometres, minutes, transport, capacity.
    city.addRoute("Colombo Fort", "Maradana",   2.0,  5, "Train", 800);
    city.addRoute("Maradana", "Wellawatte",     7.0, 12, "Train", 800);
    city.addRoute("Wellawatte", "Dehiwala",     3.0,  6, "Train", 800);
    city.addRoute("Dehiwala", "Nugegoda",       6.0, 18, "Train", 800);
    city.addRoute("Nugegoda", "Kottawa",       10.0, 15, "Train", 800);

    // Bus routes. Maradana -> Borella provides a useful train/bus transfer.
    city.addRoute("Colombo Fort", "Borella",    5.0, 18, "Bus", 60);
    city.addRoute("Borella", "Rajagiriya",      2.0,  6, "Bus", 60);
    city.addRoute("Rajagiriya", "Nugegoda",     3.0,  7, "Bus", 60);
    city.addRoute("Nugegoda", "Dehiwala",       5.0, 14, "Bus", 60);
    city.addRoute("Borella", "Nugegoda",        4.0,  9, "Bus", 60);
    city.addRoute("Wellawatte", "Dehiwala",     3.0, 10, "Bus", 60);
    city.addRoute("Maradana", "Borella",        1.5,  4, "Bus", 60);

    // Fictional demand records, separate from option 6's manual input.
    city.addDemand("Colombo Fort", "Nugegoda", "07:00", 130);
    city.addDemand("Colombo Fort", "Nugegoda", "13:00", 70);
    city.addDemand("Colombo Fort", "Nugegoda", "17:00", 240);
    city.addDemand("Kottawa", "Colombo Fort", "08:00", 180);
    city.addDemand("Dehiwala", "Colombo Fort", "08:00", 95);
    city.addDemand("Borella", "Nugegoda", "17:00", 125);

    // ========================================================
    // MENU
    // ========================================================

    int choice = 0;


    const char* optionNames[] = {
        "", "Display complete transport network", "Display bus network",
        "Display train network", "Display passenger demand", "Find fastest route",
        "Plan passenger journey and vehicle counts", "BFS network traversal",
        "System profiling", "Display locations", "Exit"
    };

    clearScreen();
    displayMenu();
    bool firstChoice = true;
    while (true)
    {
        cout << (firstChoice ? "Enter your choice (1-10): "
                             : "\nEnter next option (1-10): ") << flush;
        string selection;
        if (!getline(cin, selection))
            break;
        istringstream input(selection);
        char extra;
        bool validChoice = (input >> choice) && !(input >> extra)
                           && choice >= 1 && choice <= 10;

        // Redraw the menu above only the newly selected option's output.
        clearScreen();
        displayMenu();
        firstChoice = false;
        if (!validChoice)
        {
            cout << "Please enter a whole number from 1 to 10.\n";
            continue;
        }
        cout << "Selected option: " << choice << " - " << optionNames[choice]
             << "\n----------------------------------------------------\n";

        switch (choice)
        {
            // ------------------------------------------------
            // COMPLETE NETWORK
            // ------------------------------------------------

            case 1:
            {
                city.displayNetwork();

                break;
            }


            // ------------------------------------------------
            // BUS
            // ------------------------------------------------

            case 2:
            {
                city.displayBusRoutes();

                break;
            }


            // ------------------------------------------------
            // TRAIN
            // ------------------------------------------------

            case 3:
            {
                city.displayTrainRoutes();

                break;
            }


            // ------------------------------------------------
            // DEMAND
            // ------------------------------------------------

            case 4:
            {
                city.displayDemand();

                break;
            }


            // ------------------------------------------------
            // FASTEST ROUTE
            // ------------------------------------------------

            case 5:
            {
                string from;
                string to;





                cout << "\nEnter starting location: ";

                getline(cin, from);


                cout << "Enter destination: ";

                getline(cin, to);


                city.findFastestRoute(
                    from,
                    to
                );


                break;
            }


            // ------------------------------------------------
            // PASSENGER JOURNEY
            // ------------------------------------------------

            case 6:
            {
                string from, to, passengerInput;
                int passengers;
                char extra;
                cout << "\nEnter starting location: ";
                if (!getline(cin, from)) return 0;
                cout << "Enter destination: ";
                if (!getline(cin, to)) return 0;
                cout << "Enter number of passengers: ";
                if (!getline(cin, passengerInput)) return 0;
                istringstream input(passengerInput);
                if (!(input >> passengers) || (input >> extra) || passengers <= 0)
                {
                    cout << "\nPassenger count must be a positive whole number.\n";
                    break;
                }
                city.simulatePassengerJourney(from, to, passengers);
                break;
            }
            // ------------------------------------------------
            // BFS
            // ------------------------------------------------

            case 7:
            {
                string start;





                cout << "\nEnter starting location: ";

                getline(cin, start);


                city.BFS(start);


                break;
            }


            // ------------------------------------------------
            // PROFILING
            // ------------------------------------------------

            case 8:
            {
                city.profiling();

                break;
            }


            // ------------------------------------------------
            // LOCATIONS
            // ------------------------------------------------

            case 9:
            {
                city.displayLocations();

                break;
            }


            // ------------------------------------------------
            // EXIT
            // ------------------------------------------------

            case 10:
            {
                cout << "\nProgram terminated.\n";

                break;
            }


            default:
            {
                cout << "\nInvalid choice.\n";
            }
        }


        if (choice == 10)
            break;
    }


    return 0;
}