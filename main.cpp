#include <iostream>
#include <queue>
#include <vector>
#include "headers/Emergency.h"
#include "headers/unit.h"
#include "headers/dispatcher.h"

using namespace std;

int main() {
    // Create dispatcher object
    Dispatcher dispatcher;

    // Add units
    dispatcher.addUnit(Unit("FIRE1", "Fire", 4));
    dispatcher.addUnit(Unit("FIRE2", "Fire", 6));
    dispatcher.addUnit(Unit("AMB1", "Medical", 3));
    dispatcher.addUnit(Unit("AMB2", "Medical", 5));
    dispatcher.addUnit(Unit("POL1", "Crime", 2));
    dispatcher.addUnit(Unit("POL2", "Crime", 6));

    // Create and add emergency requests
    priority_queue<Emergency, vector<Emergency>, EmergencyComparator> emergencyQueue;
    emergencyQueue.push(Emergency("Fire", 9, 1001));
    emergencyQueue.push(Emergency("Medical", 6, 1002));
    emergencyQueue.push(Emergency("Crime", 7, 1003));

    cout << "\n🚨 Dispatching Emergency Requests...\n";

    while (!emergencyQueue.empty()) {
        Emergency e = emergencyQueue.top();
        emergencyQueue.pop();
        dispatcher.dispatch(e);
    }

    // ✅ Phase 3: Print logs
    dispatcher.printLogs();

    // ✅ Phase 4: Undo Last Dispatch
    dispatcher.undoLastDispatch();

    // ✅ Phase 4: Print logs again after undo
    dispatcher.printLogs();

    // ✅ Phase 5: Search log by Emergency ID
    int searchId = 1002;
    cout << "\n🔍 Searching for Emergency ID: " << searchId << "...\n";
    dispatcher.searchLogById(searchId);

    return 0;
}
