#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <limits>

using namespace std;

struct Participant {
    string name;
    Participant* next;
};

struct Event {
    int id;
    string name;
    string category;
    int priority;
    Participant* participants;
    queue<string> checkInQueue;
    Event* left;
    Event* right;
};

struct UndoRedo {
    string operation;
    Event* eventState;
    Participant* participantState;
};

stack<UndoRedo> undoStack;
stack<UndoRedo> redoStack;

Event* root = nullptr;
priority_queue<pair<int, Event*>> priorityQueue;

Event* createEvent(int id, string name, string category, int priority) {
    Event* newEvent = new Event{id, name, category, priority, nullptr, {}, nullptr, nullptr};
    return newEvent;
}

Event* insertEvent(Event* root, Event* newEvent) {
    if (!root) return newEvent;
    if (newEvent->id < root->id) root->left = insertEvent(root->left, newEvent);
    else root->right = insertEvent(root->right, newEvent);
    return root;
}

Event* searchEvent(Event* root, int id) {
    if (!root || root->id == id) return root;
    return id < root->id ? searchEvent(root->left, id) : searchEvent(root->right, id);
}

void addParticipant(Event* event, string name) {
    Participant* newParticipant = new Participant{name, event->participants};
    event->participants = newParticipant;
    UndoRedo undo = {"AddParticipant", event, newParticipant};
    undoStack.push(undo);
    while (!redoStack.empty()) redoStack.pop();
}

void enqueueParticipant(Event* event, string name) {
    event->checkInQueue.push(name);
    cout << "Participant " << name << " added to check-in queue." << endl;
}

void dequeueParticipant(Event* event) {
    if (event->checkInQueue.empty()) {
        cout << "No participants in queue." << endl;
        return;
    }
    cout << "Processing participant: " << event->checkInQueue.front() << endl;
    event->checkInQueue.pop();
}

void viewNextInQueue(Event* event) {
    if (event->checkInQueue.empty()) {
        cout << "No participants in queue." << endl;
        return;
    }
    cout << "Next in queue: " << event->checkInQueue.front() << endl;
}

void undo() {
    if (undoStack.empty()) {
        cout << "No actions to undo." << endl;
        return;
    }
    UndoRedo lastAction = undoStack.top();
    undoStack.pop();
    redoStack.push(lastAction);

    if (lastAction.operation == "AddParticipant") {
        Participant* toDelete = lastAction.participantState;
        Event* event = lastAction.eventState;
        event->participants = event->participants->next;
        delete toDelete;
        cout << "Undo: Removed participant." << endl;
    }
    // Additional undo operations can be handled here
}

void redo() {
    if (redoStack.empty()) {
        cout << "No actions to redo." << endl;
        return;
    }
    UndoRedo lastAction = redoStack.top();
    redoStack.pop();
    undoStack.push(lastAction);

    if (lastAction.operation == "AddParticipant") {
        Participant* newParticipant = lastAction.participantState;
        Event* event = lastAction.eventState;
        newParticipant->next = event->participants;
        event->participants = newParticipant;
        cout << "Redo: Added participant." << endl;
    }
    // Additional redo operations can be handled here
}

void generateReport() {
    cout << "Event Reports:\n";
    for (priority_queue<pair<int, Event*>> pq = priorityQueue; !pq.empty(); pq.pop()) {
        Event* event = pq.top().second;
        cout << "Event ID: " << event->id << " Name: " << event->name << " Priority: " << event->priority << endl;
        cout << "Participants:\n";
        Participant* p = event->participants;
        while (p) {
            cout << " - " << p->name << endl;
            p = p->next;
        }
        cout << "Queue Status: " << event->checkInQueue.size() << " participants in queue." << endl;
    }
}

void displayEventDetails(Event* event) {
    if (!event) {
        cout << "Event not found." << endl;
        return;
    }
    cout << "Event Details:\n";
    cout << "ID: " << event->id << "\nName: " << event->name << "\nCategory: " << event->category << "\nPriority: " << event->priority << endl;
    cout << "Participants:\n";
    Participant* p = event->participants;
    while (p) {
        cout << " - " << p->name << endl;
        p = p->next;
    }
    cout << "Queue: " << event->checkInQueue.size() << " participants waiting." << endl;
}

int main() {
    int choice;
    do {
        cout << "1. Create Event\n2. Add Participant\n3. Enqueue Participant\n4. Dequeue Participant\n5. View Next in Queue\n6. Undo\n7. Redo\n8. Generate Report\n9. Display Event\n0. Exit\nChoice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        if (choice == 1) {
            int id, priority;
            string name, category;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Category: ";
            getline(cin, category);
            cout << "Enter Priority (1-5): ";
            cin >> priority;

            Event* newEvent = createEvent(id, name, category, priority);
            root = insertEvent(root, newEvent);
            priorityQueue.push({priority, newEvent});

        } else if (choice == 2) {
            int id;
            string name;
            cout << "Enter Event ID: ";
            cin >> id;
            Event* event = searchEvent(root, id);
            if (!event) {
                cout << "Event not found." << endl;
                continue;
            }
            cout << "Enter Participant Name: ";
            cin.ignore();
            getline(cin, name);
            addParticipant(event, name);
        } else if (choice == 3) {
            int id;
            string name;
            cout << "Enter Event ID: ";
            cin >> id;
            Event* event = searchEvent(root, id);
            if (!event) {
                cout << "Event not found." << endl;
                continue;
            }
            cout << "Enter Participant Name: ";
            cin.ignore();
            getline(cin, name);
            enqueueParticipant(event, name);
        } else if (choice == 4) {
            int id;
            cout << "Enter Event ID: ";
            cin >> id;
            Event* event = searchEvent(root, id);
            if (!event) {
                cout << "Event not found." << endl;
                continue;
            }
            dequeueParticipant(event);
        } else if (choice == 5) {
            int id;
            cout << "Enter Event ID: ";
            cin >> id;
            Event* event = searchEvent(root, id);
            if (!event) {
                cout << "Event not found." << endl;
                continue;
            }
            viewNextInQueue(event);
        } else if (choice == 6) {
            undo();
        } else if (choice == 7) {
            redo();
        } else if (choice == 8) {
            generateReport();
        } else if (choice == 9) {
            int id;
            cout << "Enter Event ID: ";
            cin >> id;
            Event* event = searchEvent(root, id);
            if (!event) {
                cout << "Event not found." << endl;
                continue;
            }
            displayEventDetails(event);
        } else if (choice != 0) {
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    } while (choice != 0);

    cout << "Exiting..." << endl;
    return 0;
}

