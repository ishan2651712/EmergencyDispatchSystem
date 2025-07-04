#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <list>
#include <stack>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "Emergency.h"
#include "unit.h"
#include <sstream>

using namespace std;
using namespace chrono;

class Dispatcher {
private:
    class LogEntry {
    private:
        int emergencyId;
        string emergencyType;
        string unitId;
        int eta;
        system_clock::time_point timeAssigned;

    public:
        LogEntry(int id, string type, string unit, int e, system_clock::time_point time)
            : emergencyId(id), emergencyType(type), unitId(unit), eta(e), timeAssigned(time) {}

        int getEmergencyId() const { return emergencyId; }
        string getEmergencyType() const { return emergencyType; }
        string getUnitId() const { return unitId; }
        int getETA() const { return eta; }
        system_clock::time_point getTimeAssigned() const { return timeAssigned; }
    };

    unordered_map<string, priority_queue<Unit, vector<Unit>, UnitComparator>> unitPool;
    list<LogEntry> dispatchLogs;
    stack<LogEntry> undoStack;
static string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

public:
    Dispatcher() {}

    void addUnit(const Unit& unit) {
        unitPool[toLower(unit.getType())].push(unit); // 👈 fix here

    }

    void dispatch(const Emergency& emergency) {
        string type = toLower(emergency.getType()); // 👈 fix here too


        if (!unitPool[type].empty()) {
            Unit assigned = unitPool[type].top();
            unitPool[type].pop();

            system_clock::time_point now = system_clock::now();

            LogEntry log(emergency.getRequestId(), type, assigned.getId(), assigned.getETA(), now);
            dispatchLogs.push_back(log);
            undoStack.push(log);

            cout << "• Emergency ID: " << emergency.getRequestId()
                 << " | Type: " << type
                 << " | Severity: " << emergency.getSeverity() << "\n";
            cout << "  → Assigned Unit: " << assigned.getId()
                 << " (ETA: " << assigned.getETA() << " mins)"
                 << " at " << timeToString(now) << "\n\n";
        } else {
            cout << "• Emergency ID: " << emergency.getRequestId()
                 << " | Type: " << type
                 << " | Severity: " << emergency.getSeverity() << "\n";
            cout << "  ⚠️ No available units for this type!\n\n";
        }
    }

    void undoLastDispatch() {
        if (dispatchLogs.empty()) {
            cout << "\n⚠️ No dispatches to undo!\n";
            return;
        }

        LogEntry last = undoStack.top();
        undoStack.pop();
        dispatchLogs.pop_back();

        Unit returnedUnit(last.getUnitId(), last.getEmergencyType(), last.getETA());
        unitPool[last.getEmergencyType()].push(returnedUnit);

        cout << "\n⏪ Undo Successful:\n";
        cout << "• Emergency ID: " << last.getEmergencyId()
             << " | Type: " << last.getEmergencyType()
             << " | Returned Unit: " << last.getUnitId()
             << " → Back to unit pool.\n";
    }

    void printLogs() const {
        cout << "\n📜 Dispatch Log History:\n";
        if (dispatchLogs.empty()) {
            cout << "→ No dispatches logged yet.\n";
            return;
        }

        for (const auto& entry : dispatchLogs) {
            cout << "→ [" << timeToString(entry.getTimeAssigned()) << "] "
                 << "Emergency ID: " << entry.getEmergencyId()
                 << " | Type: " << entry.getEmergencyType()
                 << " | Unit: " << entry.getUnitId()
                 << " | ETA: " << entry.getETA() << " mins\n";
        }
    }

    // ✅ Phase 7: Export logs to a file
    void exportLogsToFile(const string& filename) const {
        ofstream outFile(filename, ios::app);

        if (!outFile) {
            cerr << "\n❌ Failed to open file: " << filename << "\n";
            return;
        }

        outFile << "\n📦 Exported Dispatch Logs:\n";
        for (const auto& entry : dispatchLogs) {
            outFile << "→ [" << timeToString(entry.getTimeAssigned()) << "] "
                    << "Emergency ID: " << entry.getEmergencyId()
                    << " | Type: " << entry.getEmergencyType()
                    << " | Unit: " << entry.getUnitId()
                    << " | ETA: " << entry.getETA() << " mins\n";
        }

        outFile << "-------------------------------\n";
        outFile.close();
        cout << "\n✅ Logs exported successfully to: " << filename << "\n";
    }

    // ✅ Phase 5: Search by Emergency ID
    void searchLogById(int id) const {
        bool found = false;
        for (const auto& entry : dispatchLogs) {
            if (entry.getEmergencyId() == id) {
                cout << "\n🔎 Emergency Found:\n";
                cout << "→ Emergency ID: " << entry.getEmergencyId()
                     << " | Type: " << entry.getEmergencyType()
                     << " | Unit: " << entry.getUnitId()
                     << " | ETA: " << entry.getETA() << " mins"
                     << " | Assigned at: " << timeToString(entry.getTimeAssigned()) << "\n";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "\n❌ No dispatch log found for Emergency ID: " << id << "\n";
        }
    }

    // ✅ Phase 6: Real-time unit availability status
    void printUnitStatus() const {
        cout << "\n📊 Current Unit Availability:\n";
        if (unitPool.empty()) {
            cout << "→ No units in the pool.\n";
            return;
        }

        for (const auto& pair : unitPool) {
            string type = pair.first;
            auto pqCopy = pair.second;

            cout << "• " << type << " Units (" << pqCopy.size() << " available):\n";
            while (!pqCopy.empty()) {
                Unit u = pqCopy.top();
                pqCopy.pop();
                cout << "   → ID: " << u.getId() << " | ETA: " << u.getETA() << " mins\n";
            }
        }
    }

    // ✅ Phase 9: Save dispatcher state to files
    void saveStateToFiles() const {
        ofstream unitsFile("units.txt");
        ofstream logsFile("logs.txt");

        if (!unitsFile || !logsFile) {
            cerr << "❌ Error opening files to save state.\n";
            return;
        }

        // Save units
        for (const auto& pair : unitPool) {
            string type = pair.first;
            auto pqCopy = pair.second;

            while (!pqCopy.empty()) {
                Unit u = pqCopy.top(); pqCopy.pop();
                unitsFile << u.getId() << "," << type << "," << u.getETA() << "\n";
            }
        }

        // Save logs
        for (const auto& entry : dispatchLogs) {
            time_t raw = system_clock::to_time_t(entry.getTimeAssigned());
            logsFile << entry.getEmergencyId() << "," << entry.getEmergencyType() << ","
                     << entry.getUnitId() << "," << entry.getETA() << "," << raw << "\n";
        }

        unitsFile.close();
        logsFile.close();
        cout << "✅ State saved to units.txt and logs.txt\n";
    }

    // ✅ Phase 9: Load dispatcher state from files
    void loadStateFromFiles() {
        ifstream unitsFile("units.txt");
        ifstream logsFile("logs.txt");

        if (!unitsFile || !logsFile) {
            cerr << "⚠️ No saved state found. Starting fresh.\n";
            return;
        }

        unitPool.clear();
        dispatchLogs.clear();

        string line;
        // Load units
        while (getline(unitsFile, line)) {
            stringstream ss(line);
            string id, type, etaStr;
            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, etaStr, ',');

            unitPool[toLower(type)].push(Unit(id, toLower(type), stoi(etaStr))); // 👈 fix

        }

        // Load logs
        while (getline(logsFile, line)) {
            stringstream ss(line);
            string idStr, type, unit, etaStr, timeStr;
            getline(ss, idStr, ',');
            getline(ss, type, ',');
            getline(ss, unit, ',');
            getline(ss, etaStr, ',');
            getline(ss, timeStr, ',');

            int eid = stoi(idStr);
            int eta = stoi(etaStr);
            time_t rawTime = stol(timeStr);
            system_clock::time_point logTime = system_clock::from_time_t(rawTime);

            dispatchLogs.push_back(LogEntry(eid, type, unit, eta, logTime));
        }

        cout << "✅ State loaded from files.\n";
    }

    static string timeToString(system_clock::time_point time) {
        time_t timeRaw = system_clock::to_time_t(time);
        char buffer[26];
        ctime_r(&timeRaw, buffer);
        buffer[24] = '\0';
        return string(buffer);
    }
    //✅ Phase 12: Emergency Dispatch Analytics
    void viewStatistics() const {
        if (dispatchLogs.empty()) {
            cout << "\n📉 No dispatches to analyze.\n";
            return;
        }

        int total = dispatchLogs.size();
        unordered_map<string, int> unitTypeCount;
        unordered_map<string, int> emergencyTypeCount;
        int totalETA = 0;

        for (const auto& log : dispatchLogs) {
            string type = log.getEmergencyType();
            unitTypeCount[type]++;
            emergencyTypeCount[type]++;
            totalETA += log.getETA();
        }

        // Determine most common emergency type
        string mostCommonType = "N/A";
        int maxCount = 0;
        for (const auto& pair : emergencyTypeCount) {
            if (pair.second > maxCount) {
                maxCount = pair.second;
                mostCommonType = pair.first;
            }
        }

        double avgETA = static_cast<double>(totalETA) / total;

        cout << "\n📊 Emergency Dispatch Analytics:\n";
        cout << "• Total Dispatches: " << total << "\n";
        cout << "• Units Used by Type:\n";
        for (const auto& pair : unitTypeCount) {
            cout << "   → " << pair.first << ": " << pair.second << "\n";
        }
        cout << "• Average ETA: " << fixed << setprecision(2) << avgETA << " mins\n";
        cout << "• Most Common Emergency Type: " << mostCommonType << "\n";
    }
};
