#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

struct Participant {
    string name;
    int age;
    Participant* next;
};

struct Event {
    string name, category;
    int id, priority;
    Participant* participants;
    Event* left;
    Event* right;
};

enum OperationType { ADD, DELETE, UPDATE };

struct Operation {
    OperationType type;
    Event* event;
    Event* previousState; // For update operations
};

// Comparison for priority queue
struct ComparePriority {
    bool operator()(Event* a, Event* b) {
        return a->priority < b->priority;
    }
};

// Globals
priority_queue<Event*, vector<Event*>, ComparePriority> schedule;
queue<Participant*> checkInQueue;
stack<Operation> undoStack, redoStack;

// Create Event
Event* createEvent(Event* root, string name, string category, int id, int priority) {
    Event* e = new Event{name, category, id, priority, nullptr, nullptr, nullptr};
    if (!root) return e;
    if (id < root->id) root->left = createEvent(root->left, name, category, id, priority);
    else root->right = createEvent(root->right, name, category, id, priority);
    return root;
}

// Search Event
Event* searchEvent(Event* root, int id) {
    if (!root || root->id == id) return root;
    return id < root->id ? searchEvent(root->left, id) : searchEvent(root->right);
}

// Delete Event
Event* deleteEvent(Event* root, int id) {
    if (!root) return root;
    if (id < root->id) root->left = deleteEvent(root->left, id);
    else if (id > root->id) root->right = deleteEvent(root->right, id);
    else {
        if (!root->left) {
            Event* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Event* temp = root->left;
            delete root;
            return temp;
        }
        Event* temp = root->right;
        while (temp->left) temp = temp->left;
        root->id = temp->id;
        root->name = temp->name;
        root->category = temp->category;
        root->priority = temp->priority;
        root->participants = temp->participants;
        root->right = deleteEvent(root->right, temp->id);
    }
    return root;
}

// Add Participant
Participant* addParticipant(Participant* head, string name, int age) {
    Participant* p = new Participant{name, age, head};
    return p;
}

// Remove Participant
Participant* removeParticipant(Participant* head, string name) {
    if (!head) return head;
    if (head->name == name) {
        Participant* temp = head->next;
        delete head;
        return temp;
    }
    head->next = removeParticipant(head->next, name);
    return head;
}

// Check-In Participant
void checkInParticipant(Event* event, string name) {
    Participant* p = event->participants;
    while (p && p->name != name) p = p->next;
    if (p) checkInQueue.push(p);
}

// Process Check-In
void processCheckIn() {
    if (!checkInQueue.empty()) {
        checkInQueue.pop();
        cout << "Processed one check-in.\n";
    } else {
        cout << "No participants in the check-in queue.\n";
    }
}

// Undo Operation
void undo(Event*& root) {
    if (undoStack.empty()) {
        cout << "No operations to undo.\n";
        return;
    }

    Operation lastOperation = undoStack.top();
    undoStack.pop();

    switch (lastOperation.type) {
        case ADD:
            root = deleteEvent(root, lastOperation.event->id);
            cout << "Undo: Event with ID " << lastOperation.event->id << " has been deleted.\n";
            break;
        case DELETE:
            root = createEvent(root, lastOperation.event->name, lastOperation.event->category,
                               lastOperation.event->id, lastOperation.event->priority);
            cout << "Undo: Event with ID " << lastOperation.event->id << " has been restored.\n";
            break;
        case UPDATE:
            root = deleteEvent(root, lastOperation.event->id);
            root = createEvent(root, lastOperation.previousState->name, lastOperation.previousState->category,
                               lastOperation.previousState->id, lastOperation.previousState->priority);
            cout << "Undo: Event with ID " << lastOperation.previousState->id << " has been reverted to the previous state.\n";
            break;
    }

    redoStack.push(lastOperation);
}

// Redo Operation
void redo(Event*& root) {
    if (redoStack.empty()) {
        cout << "No operations to redo.\n";
        return;
    }

    Operation lastRedo = redoStack.top();
    redoStack.pop();

    switch (lastRedo.type) {
        case ADD:
            root = createEvent(root, lastRedo.event->name, lastRedo.event->category,
                               lastRedo.event->id, lastRedo.event->priority);
            cout << "Redo: Event with ID " << lastRedo.event->id << " has been added back.\n";
            break;
        case DELETE:
            root = deleteEvent(root, lastRedo.event->id);
            cout << "Redo: Event with ID " << lastRedo.event->id << " has been deleted again.\n";
            break;
        case UPDATE:
            root = deleteEvent(root, lastRedo.previousState->id);
            root = createEvent(root, lastRedo.event->name, lastRedo.event->category,
                               lastRedo.event->id, lastRedo.event->priority);
            cout << "Redo: Event with ID " << lastRedo.event->id << " has been updated again.\n";
            break;
    }

    undoStack.push(lastRedo);
}

// Generate Reports
void generateReports(Event* root) {
    if (!root) return;
    generateReports(root->left);
    cout << "Event: " << root->name << ", Category: " << root->category << ", Priority: " << root->priority << "\n";
    Participant* p = root->participants;
    while (p) {
        cout << "  Participant: " << p->name << ", Age: " << p->age << "\n";
        p = p->next;
    }
    generateReports(root->right);
}

// Main Function
int main() {
    Event* root = nullptr;
    int choice, id, priority, age;
    string name, category, participantName;

    while (true) {
        cout << "1. Create Event\n2. View Event\n3. Update Event\n4. Delete Event\n5. Add Participant\n6. Remove Participant\n7. Check In\n8. Process Check In\n9. Generate Reports\n10. Undo\n11. Redo\n12. Exit\nEnter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Event Name: "; cin >> name;
                cout << "Category: "; cin >> category;
                cout << "ID: "; cin >> id;
                cout << "Priority: "; cin >> priority;
                root = createEvent(root, name, category, id, priority);
                undoStack.push({ADD, new Event{name, category, id, priority, nullptr, nullptr, nullptr}, nullptr});
                while (!redoStack.empty()) redoStack.pop(); // Clear redo stack
                break;
            }
            case 2: {
                cout << "ID: "; cin >> id;
                if (Event* e = searchEvent(root, id)) {
                    cout << "Event: " << e->name << ", Category: " << e->category << ", Priority: " << e->priority << "\n";
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 3: {
                cout << "ID: "; cin >> id;
                Event* toUpdate = searchEvent(root, id);
                if (toUpdate) {
                    Event* previousState = new Event{toUpdate->name, toUpdate->category, toUpdate->id, toUpdate->priority, nullptr, nullptr, nullptr};
                    cout << "New Event Name: "; cin >> name;
                    cout << "New Category: "; cin >> category;
                    cout << "New Priority: "; cin >> priority;
                    root = deleteEvent(root, id);
                    root = createEvent(root, name, category, id, priority);
                    undoStack.push({UPDATE, new Event{name, category, id, priority, nullptr, nullptr, nullptr}, previousState});
                    while (!redoStack.empty()) redoStack.pop(); // Clear redo stack
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 4: {
                cout << "ID: "; cin >> id;
                Event* toDelete = searchEvent(root, id);
                if (toDelete) {
                    undoStack.push({DELETE, toDelete, nullptr});
                    root = deleteEvent(root, id);
                    while (!redoStack.empty()) redoStack.pop(); // Clear redo stack
                    cout << "Event deleted.\n";
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 5: {
                cout << "Event ID: "; cin >> id;
                Event* e = searchEvent(root, id);
                if (e) {
                    cout << "Participant Name: "; cin >> participantName;
                    cout << "Participant Age: "; cin >> age;
                    e->participants = addParticipant(e->participants, participantName, age);
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 6: {
                cout << "Event ID: "; cin >> id;
                Event* e = searchEvent(root, id);
                if (e) {
                    cout << "Participant Name: "; cin >> participantName;
                    e->participants = removeParticipant(e->participants, participantName);
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 7: {
                cout << "Event ID: "; cin >> id;
                Event* e = searchEvent(root, id);
                if (e) {
                    cout << "Participant Name: "; cin >> participantName;
                    checkInParticipant(e, participantName);
                } else {
                    cout << "Event not found.\n";
                }
                break;
            }
            case 8: {
                processCheckIn();
                break;
            }
            case 9: {
                generateReports(root);
                break;
            }
            case 10: {
                undo(root);
                break;
            }
            case 11: {
                redo(root);
                break;
            }
            case 12: {
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    return 0;
}
