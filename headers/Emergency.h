// headers/Emergency.h
#pragma once
#include <string>

using namespace std;

class Emergency
{
private:
    string type;
    int severity;
    int requestId;

public:
    Emergency(string t, int s, int id)
    {
        type = t;
        severity = s;
        requestId = id;
    }

    string getType() const { return type; }
    int getSeverity() const { return severity; }
    int getRequestId() const { return requestId; }
};

// Max-heap comparator using class
class EmergencyComparator
{
public:
    bool operator()(const Emergency &a, const Emergency &b)
    {
        return a.getSeverity() < b.getSeverity(); // higher severity = higher priority
    }
};
