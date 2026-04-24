# Hospital Patient Flow and Queue Analysis System

A C++ based simulation system that models multi-stage patient flow in hospitals and analyzes waiting time across different service queues to identify bottlenecks and improve operational efficiency.

---

## Problem Statement

In real-world hospital environments, patients do not wait at a single point.  
They go through multiple stages such as:

- Registration  
- Doctor Consultation  
- Billing  
- Pharmacy  

Even if individual queues seem manageable, the **total waiting time increases due to accumulation across stages**.

Currently, most systems:
- track only individual queues  
- do not analyze full patient flow  
- cannot clearly identify bottlenecks  

---

## Solution

This project simulates the **complete patient journey** using queue-based modeling and calculates:

- Waiting time at each stage  
- Total waiting time  
- Total time spent in hospital  
- Bottleneck stage (maximum delay)  
- Category-wise patient load  

---

## Features

- Multi-stage queue simulation  
- Pre-registration delay included  
- Waiting time calculation at each stage  
- Bottleneck detection  
- Category-wise load analysis  
- Linked list based queue implementation  

---

## Concepts Used

- Data Structures (Queue using Linked List)  
- Object-Oriented Programming (OOP)  
- Simulation Logic  
- Time-based flow modeling  

---

## System Flow

Hospital Arrival
↓
Pre-Registration Wait
↓
Registration Queue
↓
Doctor Queue
↓
Billing Queue
↓
Pharmacy Queue
↓
Exit + Analysis

## Sample Output
================ PATIENT-WISE REPORT ================
ID   Name        Category     Arrive   Exit     PreReg  Reg  Doc  Bill  Pharm  TotalWait  TotalTime
1    Aman        General      09:30    10:12    20      0    4    2     1      27         42
...

================ BOTTLENECK ANALYSIS ================
Average Pre-Registration Wait: 18 minutes
Average Registration Wait: 3 minutes
Average Doctor Wait: 10 minutes
...

Main Bottleneck Stage: Doctor Consultation

## Key Insights

This system helps to:

* Identify which stage causes maximum delay
* Understand how waiting time accumulates
* Analyze patient load distribution
* Suggest better staff allocation

⸻

## Assumptions

* Patients follow queue continuously (no personal delays)
* Service times are predefined
* Simulation-based model (not real-time data)

⸻

## Future Improvements

* Add priority queue for emergency patients
* Real-time data integration
* Graphical visualization of queues
* Multi-server support (multiple doctors/counters)

⸻
 
 ## Author
   Piyush Thakur
  
⸻

## Note

This is not a hospital management system.
It is a queue analysis and simulation system focused on identifying delays and improving efficiency.












