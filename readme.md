# 🚨 Emergency Response Dispatch System (C++ DSA Project)

A real-time emergency dispatch simulation system written in C++. This project is designed to simulate how emergency units (Fire, Medical, Crime) are dispatched in response to incoming emergency requests. Built using Object-Oriented Programming (OOP) and key Data Structures like Priority Queues, Stacks, Queues, Lists, and Maps.

## 📌 Features

- ✅ Add and manage emergency units (Fire, Medical, Crime)
- ✅ Accept manual or randomly generated emergency requests
- ✅ Prioritized dispatch based on severity
- ✅ Undo last dispatch (stack-based rollback)
- ✅ View real-time unit availability
- ✅ Persistent save/load using files (`units.txt`, `logs.txt`)
- ✅ Export dispatch logs to file
- ✅ Search dispatch history by emergency ID
- ✅ **New:** 📊 Emergency Dispatch Analytics Dashboard
  - Total dispatches
  - Units used by type
  - Average ETA
  - Most common emergency type

## 🧠 Data Structures Used

| Feature                        | Data Structure   |
|-------------------------------|------------------|
| Unit Pool                     | Priority Queue   |
| Emergency Queue               | Priority Queue   |
| Undo Dispatch                 | Stack            |
| Dispatch Logs                 | List             |
| Emergency Analytics           | Unordered Map    |
| ID Tracking                   | Set              |

## 📝 Sample Usage

```bash
# Add units
1 → FIRE1, Fire, ETA: 4
1 → AMB1, Medical, ETA: 3

# Add emergencies
3 → Fire, Severity: 8, ID: 1234

# Dispatch
4

# View analytics
11
```

## 🛠️ How to Compile & Run
```bash
# Compile
g++ main.cpp -Iheaders -std=c++17 -o dispatch

# Run
./dispatch
```

## 💾 Persistence
- units.txt stores current available units
- logs.txt records all dispatch events with timestamps

## 📊 Emergency Dispatch Analytics:
```bash
• Total Dispatches: 6
• Units Used by Type:
   → fire: 3
   → medical: 2
   → crime: 1
• Average ETA: 3.17 mins
• Most Common Emergency Type: fire
```