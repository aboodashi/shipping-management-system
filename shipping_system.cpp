#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>

using namespace std;

///////////////////////////// Shipment Structure /////////////////////////////
struct Shipment {
    int    id;
    string sender;
    string receiver;
    string destination;
    double weight;
    int    priority;   
    string status;     
};

int nextShipmentID = 1001;

//  Linked List Node
struct Node {
    Shipment data;
    Node* next;
    Node(Shipment s) : data(s), next(nullptr) {}
};


class ShipmentList {
    Node* head;
public:
    ShipmentList() : head(nullptr) {}

    void insert(Shipment s) {
        Node* newNode = new Node(s);
        if (!head) { head = newNode; return; }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }

    bool remove(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            Node* tmp = head; head = head->next;
            delete tmp; return true;
        }
        Node* cur = head;
        while (cur->next && cur->next->data.id != id)
            cur = cur->next;
        if (!cur->next) return false;
        Node* tmp = cur->next;
        cur->next = tmp->next;
        delete tmp; return true;
    }

    Shipment* find(int id) {
        Node* cur = head;
        while (cur) {
            if (cur->data.id == id) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    vector<Shipment> toVector() {
        vector<Shipment> v;
        Node* cur = head;
        while (cur) { v.push_back(cur->data); cur = cur->next; }
        return v;
    }

    bool updateStatus(int id, const string& status) {
        Shipment* s = find(id);
        if (!s) return false;
        s->status = status; return true;
    }

    ~ShipmentList() {
        while (head) { Node* tmp = head; head = head->next; delete tmp; }
    }
};

//  Print helpers =============================================================
void printHeader() {
    cout << "\n" << string(85, '-') << "\n";
    cout << left
        << setw(6) << "ID"
        << setw(14) << "Sender"
        << setw(14) << "Receiver"
        << setw(14) << "Destination"
        << setw(8) << "Weight"
        << setw(10) << "Priority"
        << setw(14) << "Status" << "\n";
    cout << string(85, '-') << "\n";
}

void printShipment(const Shipment& s) {
    string pri = (s.priority == 1) ? "High" : "Normal";
    cout << left
        << setw(6) << s.id
        << setw(14) << s.sender
        << setw(14) << s.receiver
        << setw(14) << s.destination
        << setw(8) << s.weight
        << setw(10) << pri
        << setw(14) << s.status << "\n";
}

//  Main System Class =============================================================
class ShippingSystem {

    ShipmentList list;

    struct ComparePriority {
        bool operator()(const Shipment& a, const Shipment& b) {
            if (a.priority != b.priority) return a.priority > b.priority;
            return a.id > b.id;
        }
    };
    priority_queue<Shipment, vector<Shipment>, ComparePriority> priorityQueue;
    queue<Shipment> normalQueue;
    stack<Shipment> undoStack;

    const string ADMIN_PASS = "admin123";

    //  Shipment operations (Admin)=============================================================
    void addShipment() {
        Shipment s;
        s.id = nextShipmentID++;
        s.status = "Pending";
        cout << "\n--- Add New Shipment ---\n";
        cout << "Sender name   : "; cin.ignore(); getline(cin, s.sender);
        cout << "Receiver name : "; getline(cin, s.receiver);
        cout << "Destination   : "; getline(cin, s.destination);
        cout << "Weight (kg)   : "; cin >> s.weight;
        cout << "Priority (1=High / 2=Normal): "; cin >> s.priority;
        while (s.priority != 1 && s.priority != 2) {
            cout << "Invalid! Enter 1 or 2: "; cin >> s.priority;
        }
        list.insert(s);
        if (s.priority == 1) priorityQueue.push(s);
        else                 normalQueue.push(s);
        cout << "[OK] Shipment added. ID = " << s.id << "\n";
    }

    void deleteShipment() {
        cout << "\n--- Delete Shipment ---\n";
        cout << "Enter Shipment ID: ";
        int id; cin >> id;
        Shipment* s = list.find(id);
        if (!s) { cout << "[!] Not found.\n"; return; }
        undoStack.push(*s);
        list.remove(id);
        cout << "[OK] Shipment " << id << " deleted. (Can be undone)\n";
    }

    void undoDelete() {
        if (undoStack.empty()) { cout << "\n[!] Nothing to undo.\n"; return; }
        Shipment s = undoStack.top(); undoStack.pop();
        list.insert(s);
        cout << "[OK] Shipment " << s.id << " restored.\n";
    }

    void displayAll() {
        vector<Shipment> v = list.toVector();
        if (v.empty()) { cout << "\n[!] No shipments.\n"; return; }
        printHeader();
        for (auto& s : v) printShipment(s);
        cout << string(85, '-') << "\n";
    }

    void processNext() {
        cout << "\n--- Process Next Shipment ---\n";
        if (!priorityQueue.empty()) {
            Shipment s = priorityQueue.top(); priorityQueue.pop();
            list.updateStatus(s.id, "Processing");
            s.status = "Processing";
            cout << "[>>] Processing HIGH-priority shipment:\n";
            printHeader(); printShipment(s);
            cout << string(85, '-') << "\n";
            return;
        }
        if (!normalQueue.empty()) {
            Shipment s = normalQueue.front(); normalQueue.pop();
            list.updateStatus(s.id, "Processing");
            s.status = "Processing";
            cout << "[>>] Processing NORMAL shipment:\n";
            printHeader(); printShipment(s);
            cout << string(85, '-') << "\n";
            return;
        }
        cout << "[!] All queues are empty.\n";
    }

    void markDelivered() {
        cout << "\n--- Mark as Delivered ---\n";
        cout << "Enter Shipment ID: ";
        int id; cin >> id;
        if (list.updateStatus(id, "Delivered"))
            cout << "[OK] Shipment " << id << " marked as Delivered.\n";
        else
            cout << "[!] Not found.\n";
    }

    void searchByID() {
        cout << "\n--- Search Shipment ---\n";
        cout << "Enter Shipment ID: ";
        int id; cin >> id;
        Shipment* s = list.find(id);
        if (!s) { cout << "[!] Not found.\n"; return; }
        printHeader(); printShipment(*s);
        cout << string(85, '-') << "\n";
    }

    void sortShipments() {
        cout << "\n--- Sort Shipments ---\n";
        cout << "Sort by: 1.Weight  2.Priority  3.Destination\nChoice: ";
        int ch; cin >> ch;
        vector<Shipment> v = list.toVector();
        if (v.empty()) { cout << "[!] No shipments.\n"; return; }
        if (ch == 1) sort(v.begin(), v.end(), [](const Shipment& a, const Shipment& b) { return a.weight < b.weight; });
        else if (ch == 2) sort(v.begin(), v.end(), [](const Shipment& a, const Shipment& b) { return a.priority < b.priority; });
        else if (ch == 3) sort(v.begin(), v.end(), [](const Shipment& a, const Shipment& b) { return a.destination < b.destination; });
        else { cout << "[!] Invalid.\n"; return; }
        printHeader();
        for (auto& s : v) printShipment(s);
        cout << string(85, '-') << "\n";
    }

    //  Admin menu=============================================================
    void adminMenu() {
        int ch;
        do {
            cout << "\n|==============================|\n";
            cout << "|        ADMIN MENU            |\n";
            cout << "|==============================|\n";
            cout << "|  1.  Add Shipment            |\n";
            cout << "|  2.  Delete Shipment         |\n";
            cout << "|  3.  Display All Shipments   |\n";
            cout << "|  4.  Process Next Shipment   |\n";
            cout << "|  5.  Mark as Delivered       |\n";
            cout << "|  6.  Sort Shipments          |\n";
            cout << "|  7.  Undo Last Delete        |\n";
            cout << "|  0.  Logout                  |\n";
            cout << "|==============================|\n";
            cout << "Choice: ";
            cin >> ch;
            switch (ch) {
            case 1: addShipment();   break;
            case 2: deleteShipment();break;
            case 3: displayAll();    break;
            case 4: processNext();   break;
            case 5: markDelivered(); break;
            case 6: sortShipments(); break;
            case 7: undoDelete();    break;
            case 0: cout << "[Logged out]\n"; break;
            default: cout << "[!] Invalid choice.\n";
            }
        } while (ch != 0);
    }

    //  Public tracking =============================================================
    void trackShipment() {
        cout << "\n--- Track Shipment ---\n";
        cout << "Enter Shipment ID: ";
        int id; cin >> id;
        Shipment* s = list.find(id);
        if (!s) { cout << "[!] Shipment not found.\n"; return; }
        cout << "\n+----------------------------------+\n";
        cout << "  Shipment ID  : " << s->id << "\n";
        cout << "  From         : " << s->sender << "\n";
        cout << "  To           : " << s->receiver << "\n";
        cout << "  Destination  : " << s->destination << "\n";
        cout << "  Weight       : " << s->weight << " kg\n";
        cout << "  Priority     : " << (s->priority == 1 ? "High" : "Normal") << "\n";
        cout << "  Status       : [" << s->status << "]\n";
        cout << "+----------------------------------+\n";
    }

    ///////////////////////////////////////Admin login ///////////////////////////////////////
    void adminLogin() {
        string pass;
        cout << "Admin Password: ";
        cin >> pass;
        if (pass == ADMIN_PASS) {
            cout << "[OK] Welcome, Admin!\n";
            adminMenu();
        }
        else {
            cout << "[!] Wrong password.\n";
        }
    }

public:
    void run() {
        cout << "\n==========================================\n";
        cout << "  International Shipping Management System\n";
        cout << "==========================================\n";

        int choice;
        do {
            cout << "\n|==========================|\n";
            cout << "|        MAIN MENU         |\n";
            cout << "|==========================|\n";
            cout << "|  1. Admin Login          |\n";
            cout << "|  2. Track a Shipment     |\n";
            cout << "|  0. Exit                 |\n";
            cout << "|==========================|\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice) {
            case 1: adminLogin();    break;
            case 2: trackShipment(); break;
            case 0: cout << "\nGoodbye!\n"; break;
            default: cout << "[!] Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

//  Entry point=============================================================
int main() {
    ShippingSystem app;
    app.run();
    return 0;
}