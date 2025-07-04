// headers/Unit.h
#pragma once
#include <string>

using namespace std;

class Unit
{
private:
    string id;
    string type; // Ambulance / FireTruck / Police
    int eta;     // Estimated time to reach in minutes
    bool available;

public:
    Unit(string i, string t, int e)
    {
        id = i;
        type = t;
        eta = e;
        available = true;
    }

    string getId() const { return id; }
    string getType() const { return type; }
    int getETA() const { return eta; }
    bool isAvailable() const { return available; }

    void setAvailable(bool a) { available = a; }
};

// Comparator for Unit Min-Heap: lower ETA = higher priority
class UnitComparator
{
public:
    bool operator()(const Unit &a, const Unit &b)
    {
        return a.getETA() > b.getETA();
    }
};
