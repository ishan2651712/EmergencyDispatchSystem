#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include "headers/Emergency.h"
#include "headers/unit.h"
#include "headers/dispatcher.h"
#include <random>
#include <ctime>
#include <set>
#include <algorithm>

using namespace std;
string toLower(const string& str) {
    string lower;
    for (char c : str)
        lower += tolower(c);
    return lower;
}

set<int> usedIds; // to avoid duplicate emergency IDs

// ✅ Phase 10: Random Emergency Generator (lowercase types)
Emergency generateRandomEmergency() {
    static const vector<string> types = {"fire", "medical", "crime"};
    static mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> typeDist(0, 2);
    uniform_int_distribution<int> severityDist(1, 10);
    uniform_int_distribution<int> idDist(1000, 9999);

    string type = types[typeDist(rng)];
    int severity = severityDist(rng);

    int id;
    do {
        id = idDist(rng);
    } while (usedIds.find(id) != usedIds.end());

    usedIds.insert(id);
    return Emergency(type, severity, id);
}

int main() {
    Dispatcher dispatcher;

    // ✅ Phase 9: Load saved state
    dispatcher.loadStateFromFiles();

    priority_queue<Emergency, vector<Emergency>, EmergencyComparator> emergencyQueue;

    int choice;
    do {
        cout << "\n🚨 EMERGENCY RESPONSE SYSTEM MENU 🚨\n";
        cout << "1. Add Emergency Unit\n";
        cout << "2. View Available Units\n";
        cout << "3. Add Emergency Request\n";
        cout << "4. Dispatch Emergencies\n";
        cout << "5. Undo Last Dispatch\n";
        cout << "6. Search Dispatch Log by Emergency ID\n";
        cout << "7. View Dispatch Logs\n";
        cout << "8. Export Logs to File\n";
        cout << "9. Exit\n";
        cout << "10. Generate Random Emergencies\n";
        cout << "11. View System Statistics\n";
        cout << "Enter choice (1-10): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string id, type;
                int eta;
                cout << "Enter Unit ID: ";
                cin >> id;
            
 while (true) {
        cout << "Enter Unit Type (Fire/Medical/Crime): ";
        cin >> type;

        // Convert to lowercase
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type == "fire" || type == "medical" || type == "crime") {
            break;
        } else {
            cout << "❌ Invalid type. Please enter Fire, Medical, or Crime.\n";
        }
    }
                cout << "Enter ETA (in minutes): ";
                cin >> eta;
                transform(type.begin(), type.end(), type.begin(), ::tolower); // normalize
                dispatcher.addUnit(Unit(id, type, eta));
                cout << "✅ Unit added.\n";
                break;
            }

            case 2:
                dispatcher.printUnitStatus();
                break;

            case 3: {
                string type;
                int severity, id;
                cout << "Enter Emergency Type (Fire/Medical/Crime): ";
                cin >> type;
                transform(type.begin(), type.end(), type.begin(), ::tolower); // normalize
                cout << "Enter Severity (1-10): ";
                cin >> severity;
                cout << "Enter Emergency ID: ";
                cin >> id;
                emergencyQueue.push(Emergency(type, severity, id));
                cout << "✅ Emergency added to queue.\n";
                break;
            }

            case 4: {
                if (emergencyQueue.empty()) {
                    cout << "⚠️ No emergencies in queue.\n";
                    break;
                }
                cout << "\n🚑 Dispatching...\n";
                while (!emergencyQueue.empty()) {
                    Emergency e = emergencyQueue.top();
                    emergencyQueue.pop();
                    dispatcher.dispatch(e);
                }
                break;
            }

            case 5:
                dispatcher.undoLastDispatch();
                break;

            case 6: {
                int searchId;
                cout << "Enter Emergency ID to search: ";
                cin >> searchId;
                dispatcher.searchLogById(searchId);
                break;
            }

            case 7:
                dispatcher.printLogs();
                break;

            case 8:
                dispatcher.exportLogsToFile("logs.txt");
                break;

            case 10: {
                int count;
                cout << "Enter how many random emergencies to generate: ";
                cin >> count;
                for (int i = 0; i < count; ++i) {
                    Emergency e = generateRandomEmergency();
                    emergencyQueue.push(e);
                    cout << "✅ Random Emergency Added — Type: " << e.getType()
                         << " | Severity: " << e.getSeverity()
                         << " | ID: " << e.getRequestId() << "\n";
                }
                break;
            }
        case 11:
        dispatcher.viewStatistics();
        break;

            case 9:
                dispatcher.saveStateToFiles();
                cout << "👋 Exiting system. Stay safe!\n";
                break;

            default:
                cout << "❌ Invalid choice. Try again.\n";
        }

    } while (choice != 9);

    return 0;
}
