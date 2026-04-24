/*
Hospital Patient Flow and Queue Analysis System
Author: Piyush

Description:
This program simulates patient flow in a hospital using queues.
It tracks waiting time across multiple stages:
- Pre-registration
- Registration
- Doctor
- Billing
- Pharmacy

It identifies:
- Total waiting time
- Bottleneck stage
- Category-wise load distribution

Concepts Used:
- Queue (Linked List Implementation)
- OOP (Classes & Objects)
- Simulation Logic
*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include<algorithm>
using namespace std;

// -------------------- PATIENT CLASS --------------------
// Represents each patient and stores all related data
class Patient {
public:
    int id;
    string name;
    string category;

    // Time tracking (in minutes)
    int hospitalArrival;   // time when patient enters hospital
    int regQueueEntry;     // time when patient enters registration queue

    // Service times at each stage
    int regService, docService, billService, pharmService;

    // Waiting times at each stage
    int preRegWait;
    int regWait, docWait, billWait, pharmWait;

    // Total time calculations
    int totalWaitingTime;
    int totalTimeInHospital;

    // End times of each stage
    int regEnd, docEnd, billEnd, pharmEnd;

    // Constructor initializes patient data
    Patient(int id, string name, string category, int hospitalArrival, int regQueueEntry,
            int regService, int docService, int billService, int pharmService) {

        this->id = id;
        this->name = name;
        this->category = category;
        this->hospitalArrival = hospitalArrival;
        this->regQueueEntry = regQueueEntry;
        this->regService = regService;
        this->docService = docService;
        this->billService = billService;
        this->pharmService = pharmService;

        // Pre-registration waiting time (before entering system)
        preRegWait = regQueueEntry - hospitalArrival;

        // Initialize all waiting times to 0
        regWait = docWait = billWait = pharmWait = 0;

        // Initialize totals
        totalWaitingTime = totalTimeInHospital = 0;

        // Initialize stage end times
        regEnd = docEnd = billEnd = pharmEnd = 0;
    }
};

// -------------------- NODE CLASS --------------------
// Used for linked list implementation of queue
class Node {
public:
    Patient* data;
    Node* next;

    Node(Patient* p) {
        data = p;
        next = nullptr;
    }
};

// -------------------- QUEUE CLASS --------------------
// Custom queue using linked list (FIFO)
class PatientQueue {
private:
    Node* frontNode;  // front of queue
    Node* rearNode;   // rear of queue

public:
    PatientQueue() {
        frontNode = rearNode = nullptr;
    }

    // Check if queue is empty
    bool isEmpty() {
        return frontNode == nullptr;
    }

    // Insert patient into queue (enqueue)
    void enqueue(Patient* p) {
        Node* newNode = new Node(p);

        if (rearNode == nullptr) {
            frontNode = rearNode = newNode;
            return;
        }

        rearNode->next = newNode;
        rearNode = newNode;
    }

    // Remove patient from queue (dequeue)
    Patient* dequeue() {
        if (isEmpty()) return nullptr;

        Node* temp = frontNode;
        Patient* p = temp->data;

        frontNode = frontNode->next;

        if (frontNode == nullptr) {
            rearNode = nullptr;
        }

        delete temp;
        return p;
    }
};

// -------------------- TIME UTILITIES --------------------

// Convert time from "HH:MM" to total minutes
int timeToMinutes(string time) {
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    return hour * 60 + minute;
}

// Convert total minutes back to "HH:MM" format
string minutesToTime(int totalMinutes) {
    int hour = totalMinutes / 60;
    int minute = totalMinutes % 60;

    string h = (hour < 10 ? "0" : "") + to_string(hour);
    string m = (minute < 10 ? "0" : "") + to_string(minute);

    return h + ":" + m;
}

// -------------------- CORE SIMULATION FUNCTION --------------------
void processHospitalFlow(vector<Patient*>& patients) {

    // Create queues for each hospital stage
    PatientQueue registrationQueue, doctorQueue, billingQueue, pharmacyQueue;

    // Step 1: Add all patients to registration queue
    for (Patient* p : patients) {
        registrationQueue.enqueue(p);
    }

    // Track when each service becomes available
    int regAvailable = 0;
    int docAvailable = 0;
    int billAvailable = 0;
    int pharmAvailable = 0;

    // -------- REGISTRATION STAGE --------
    while (!registrationQueue.isEmpty()) {
        Patient* p = registrationQueue.dequeue();

        // Service starts when both patient and server are available
        int startTime = max(regAvailable, p->regQueueEntry);

        // Waiting time = delay before service starts
        p->regWait = startTime - p->regQueueEntry;

        // End time after service
        p->regEnd = startTime + p->regService;

        // Update server availability
        regAvailable = p->regEnd;

        // Move patient to next stage
        doctorQueue.enqueue(p);
    }

    // -------- DOCTOR STAGE --------
    while (!doctorQueue.isEmpty()) {
        Patient* p = doctorQueue.dequeue();

        // Patient enters doctor queue after registration
        int docQueueEntry = p->regEnd;

        int startTime = max(docAvailable, docQueueEntry);

        p->docWait = startTime - docQueueEntry;
        p->docEnd = startTime + p->docService;

        docAvailable = p->docEnd;

        billingQueue.enqueue(p);
    }

    // -------- BILLING STAGE --------
    while (!billingQueue.isEmpty()) {
        Patient* p = billingQueue.dequeue();

        int billQueueEntry = p->docEnd;

        int startTime = max(billAvailable, billQueueEntry);

        p->billWait = startTime - billQueueEntry;
        p->billEnd = startTime + p->billService;

        billAvailable = p->billEnd;

        pharmacyQueue.enqueue(p);
    }

    // -------- PHARMACY STAGE --------
    while (!pharmacyQueue.isEmpty()) {
        Patient* p = pharmacyQueue.dequeue();

        int pharmQueueEntry = p->billEnd;

        int startTime = max(pharmAvailable, pharmQueueEntry);

        p->pharmWait = startTime - pharmQueueEntry;
        p->pharmEnd = startTime + p->pharmService;

        pharmAvailable = p->pharmEnd;

        // Final calculations for patient
        p->totalWaitingTime = p->preRegWait + p->regWait + p->docWait + p->billWait + p->pharmWait;
        p->totalTimeInHospital = p->pharmEnd - p->hospitalArrival;
    }
}

// -------------------- PATIENT REPORT --------------------
void showPatientReport(vector<Patient*>& patients) {
    cout << "\n================ PATIENT-WISE REPORT ================\n";

    // Table headers
    cout << left << setw(5) << "ID"
         << setw(12) << "Name"
         << setw(12) << "Category"
         << setw(10) << "Arrive"
         << setw(10) << "Exit"
         << setw(10) << "PreReg"
         << setw(8) << "Reg"
         << setw(8) << "Doc"
         << setw(8) << "Bill"
         << setw(8) << "Pharm"
         << setw(12) << "TotalWait"
         << setw(12) << "TotalTime"
         << endl;

    // Display data for each patient
    for (Patient* p : patients) {
        cout << left << setw(5) << p->id
             << setw(12) << p->name
             << setw(12) << p->category
             << setw(10) << minutesToTime(p->hospitalArrival)
             << setw(10) << minutesToTime(p->pharmEnd)
             << setw(10) << p->preRegWait
             << setw(8) << p->regWait
             << setw(8) << p->docWait
             << setw(8) << p->billWait
             << setw(8) << p->pharmWait
             << setw(12) << p->totalWaitingTime
             << setw(12) << p->totalTimeInHospital
             << endl;
    }
}

// -------------------- BOTTLENECK ANALYSIS --------------------
void showBottleneckAnalysis(vector<Patient*>& patients) {
    int totalPreReg = 0, totalReg = 0, totalDoc = 0, totalBill = 0, totalPharm = 0;
    int n = patients.size();

    // Sum waiting times across all patients
    for (Patient* p : patients) {
        totalPreReg += p->preRegWait;
        totalReg += p->regWait;
        totalDoc += p->docWait;
        totalBill += p->billWait;
        totalPharm += p->pharmWait;
    }

    // Calculate average waiting time
    double avgPreReg = (double)totalPreReg / n;
    double avgReg = (double)totalReg / n;
    double avgDoc = (double)totalDoc / n;
    double avgBill = (double)totalBill / n;
    double avgPharm = (double)totalPharm / n;

    cout << "\n================ BOTTLENECK ANALYSIS ================\n";
    cout << "Average Pre-Registration Wait: " << avgPreReg << " minutes\n";
    cout << "Average Registration Wait:     " << avgReg << " minutes\n";
    cout << "Average Doctor Wait:           " << avgDoc << " minutes\n";
    cout << "Average Billing Wait:          " << avgBill << " minutes\n";
    cout << "Average Pharmacy Wait:         " << avgPharm << " minutes\n";

    // Identify bottleneck stage (maximum delay)
    double maxWait = avgPreReg;
    string bottleneck = "Pre-Registration";

    if (avgReg > maxWait) {
        maxWait = avgReg;
        bottleneck = "Registration";
    }

    if (avgDoc > maxWait) {
        maxWait = avgDoc;
        bottleneck = "Doctor Consultation";
    }

    if (avgBill > maxWait) {
        maxWait = avgBill;
        bottleneck = "Billing";
    }

    if (avgPharm > maxWait) {
        maxWait = avgPharm;
        bottleneck = "Pharmacy";
    }

    cout << "\nMain Bottleneck Stage: " << bottleneck << endl;
    cout << "Highest Average Waiting Time: " << maxWait << " minutes\n";
}

// -------------------- CATEGORY ANALYSIS --------------------
void showCategoryAnalysis(vector<Patient*>& patients) {
    vector<string> categories;
    vector<int> counts;

    // Count patients in each category
    for (Patient* p : patients) {
        bool found = false;

        for (int i = 0; i < categories.size(); i++) {
            if (categories[i] == p->category) {
                counts[i]++;
                found = true;
                break;
            }
        }

        if (!found) {
            categories.push_back(p->category);
            counts.push_back(1);
        }
    }

    cout << "\n================ CATEGORY LOAD ANALYSIS ================\n";

    int maxCount = 0;
    string maxCategory = "";

    for (int i = 0; i < categories.size(); i++) {
        cout << categories[i] << ": " << counts[i] << " patients\n";

        if (counts[i] > maxCount) {
            maxCount = counts[i];
            maxCategory = categories[i];
        }
    }

    cout << "\nMost Loaded Category: " << maxCategory << endl;
}

// -------------------- SAMPLE DATA --------------------
void addSampleData(vector<Patient*>& patients) {
    patients.push_back(new Patient(1, "Aman", "General", timeToMinutes("09:30"), timeToMinutes("09:50"), 4, 10, 3, 5));
    patients.push_back(new Patient(2, "Riya", "Skin", timeToMinutes("09:35"), timeToMinutes("09:55"), 3, 8, 2, 4));
    patients.push_back(new Patient(3, "Karan", "Eye", timeToMinutes("09:40"), timeToMinutes("10:00"), 5, 12, 3, 6));
    patients.push_back(new Patient(4, "Neha", "General", timeToMinutes("09:45"), timeToMinutes("10:05"), 4, 9, 2, 5));
    patients.push_back(new Patient(5, "Rohit", "Emergency", timeToMinutes("09:50"), timeToMinutes("10:10"), 2, 15, 4, 6));
}

// -------------------- MANUAL INPUT --------------------
void addManualData(vector<Patient*>& patients) {
    int n;
    cout << "Enter number of patients: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int id;
        string name, category;
        string hospitalArrival, regEntry;
        int regService, docService, billService, pharmService;

        cout << "\nEnter details for patient " << i + 1 << ":\n";

        cout << "Patient ID: ";
        cin >> id;

        cout << "Name: ";
        cin >> name;

        cout << "Category: ";
        cin >> category;

        cout << "Hospital Arrival Time (HH:MM): ";
        cin >> hospitalArrival;

        cout << "Registration Queue Entry Time (HH:MM): ";
        cin >> regEntry;

        cout << "Registration Service Time (minutes): ";
        cin >> regService;

        cout << "Doctor Service Time (minutes): ";
        cin >> docService;

        cout << "Billing Service Time (minutes): ";
        cin >> billService;

        cout << "Pharmacy Service Time (minutes): ";
        cin >> pharmService;

        // Add patient to list
        patients.push_back(new Patient(
            id, name, category,
            timeToMinutes(hospitalArrival),
            timeToMinutes(regEntry),
            regService, docService, billService, pharmService
        ));
    }
}

// -------------------- MAIN FUNCTION --------------------
int main() {
    vector<Patient*> patients;

    int choice;

    cout << "===============================================\n";
    cout << " HOSPITAL PATIENT FLOW AND QUEUE ANALYSIS SYSTEM\n";
    cout << "===============================================\n";

    cout << "\n1. Use Sample Data";
    cout << "\n2. Enter Patient Data Manually";
    cout << "\nEnter choice: ";
    cin >> choice;

    // Choose input method
    if (choice == 1) {
        addSampleData(patients);
    } else if (choice == 2) {
        addManualData(patients);
    } else {
        cout << "Invalid choice.\n";
        return 0;
    }

    // Process patient flow
    processHospitalFlow(patients);

    // Display outputs
    showPatientReport(patients);
    showBottleneckAnalysis(patients);
    showCategoryAnalysis(patients);

    // Free allocated memory
    for (Patient* p : patients) {
        delete p;
    }

    return 0;
}
