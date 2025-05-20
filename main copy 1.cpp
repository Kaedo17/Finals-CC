#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <limits>
#include <ctime>
#include <iomanip>
#include "database.h"

using namespace std;

class ToDoApp {
private:
    ToDoDataBase datBase;
     string currentInput;

    // ANSI color codes
    const  string RED = "\033[31m";
    const  string GREEN = "\033[32m";
    const  string YELLOW = "\033[33m";
    const  string BLUE = "\033[34m";
    const  string MAGENTA = "\033[35m";
    const  string RESET = "\033[0m";
    const  string STRIKE = "\033[9m";
    const  string BOLD = "\033[1m";

    void clearScreen() {
         cout << "\033[2J\033[1;1H";
    }

    void displayMainMenu() {
         cout << "\n" << BOLD << BLUE << "+-----------------------MAIN MENU-----------------------+" << RESET << "\n";
         cout << "[1] Add task      [2] Toggle complete  [3] Set priority\n";
         cout << "[4] Delete task  [5] Sort tasks      [6] Exit\n";
         cout << BOLD << BLUE << "+----------------------------------------------------+" << RESET << "\n";
         cout << "Choose an option: ";
    }

    void displaySortMenu() {
         cout << "\n" << BOLD << MAGENTA << "+-----------------------SORT BY-----------------------+" << RESET << "\n";
         cout << "[1] Priority (High to Low)  [2] Alphabetical (A-Z)\n";
         cout << "[3] Creation Date (Newest)  [4] Back to Main Menu\n";
         cout << BOLD << MAGENTA << "+----------------------------------------------------+" << RESET << "\n";
         cout << "Choose sort option: ";
    }

    void viewTasks(bool showPriorityHeaders = true) {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
            cout << "\nNo tasks found!\n";
            return;
        }
    
        vector<size_t> taskIndices; // Store the actual indices of tasks
        vector<tuple<string, bool, int, time_t>> highPriority;
        vector<tuple<string, bool, int, time_t>> normalPriority;
    
        // Separate tasks by priority
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) == 1) {
                highPriority.push_back(tasks[i]);
                taskIndices.push_back(i);
            } else {
                normalPriority.push_back(tasks[i]);
                taskIndices.push_back(i);
            }
        }
    
        cout << "\n" << BOLD << GREEN << "+-----------------------TASKS-----------------------+" << RESET << "\n";
        
        size_t displayNumber = 1;
        
        if (!highPriority.empty() && showPriorityHeaders) {
            cout << YELLOW << "            >>> HIGH PRIORITY TASKS <<<\n" << RESET;
            displayNumber = displayTaskList(highPriority, displayNumber);
        }
    
        if (!normalPriority.empty() && showPriorityHeaders) {
            cout << BLUE << "\n              >>> NORMAL TASKS <<<\n" << RESET;
        }
        displayTaskList(normalPriority, displayNumber);
    }
    
    size_t displayTaskList(const vector<tuple<string, bool, int, time_t>>& tasks, size_t startNumber = 1) {
        size_t currentNumber = startNumber;
        for (const auto& [task, completed, priority, created] : tasks) {
            string color;
            if (completed) {
                color = STRIKE + RED;
            } else if (priority == 1) {
                color = YELLOW;
            } else if (priority == 2) {
                color = BLUE;
            }
    
            tm* ptm = localtime(&created);
            char buffer[32];
            strftime(buffer, 32, "%Y-%m-%d %H:%M", ptm);
    
            cout << currentNumber << ". " << color 
                      << (completed ? "[X] " : "[ ] ") 
                      << left << setw(35) << task << RESET
                      << MAGENTA << "(" << buffer << ")" << RESET << "\n";
            currentNumber++;
        }
        return currentNumber;
    }

    void addTask() {
         cout << "\nEnter new task: ";
         cin.ignore();
         getline( cin, currentInput);

        if (!currentInput.empty()) {
            int priority;
             cout << "Set priority (1=High, 2=Low): ";
             cin >> priority;
            if (priority < 1 || priority > 2) priority = 2;
            
            datBase.addTask(currentInput, priority);
            clearScreen();
             cout << GREEN << "Task added successfully!\n" << RESET;
        }
    }

    void toggleTask() {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
            cout << "No tasks to toggle!\n";
            return;
        }
    
        // First display the tasks with their current numbering
        viewTasks();
    
        size_t displayNumber;
        cout << "Toggle task number: ";
        cin >> displayNumber;
    
        // Rebuild the task order to map display numbers to actual indices
        vector<size_t> taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) == 1) {
                taskIndices.push_back(i);
            }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) != 1) {
                taskIndices.push_back(i);
            }
        }
    
        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
            size_t actualIndex = taskIndices[displayNumber - 1];
            auto& task = tasks[actualIndex];
            get<1>(task) = !get<1>(task); // Toggle completion status
            datBase.updateDataBase();
            clearScreen();
            cout << "Task toggled!\n";
        } else {
            cout << "Invalid task number!\n";
        }
    }

    void deleteTask() {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
            cout << "No tasks to delete!\n";
            return;
        }
    
        viewTasks();
    
        size_t displayNumber;
        cout << "Delete task number: ";
        cin >> displayNumber;
    
        vector<size_t> taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) == 1) {
                taskIndices.push_back(i);
            }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) != 1) {
                taskIndices.push_back(i);
            }
        }
    
        if (displayNumber > 0 && displayNumber <= taskIndices.size()) {
            size_t actualIndex = taskIndices[displayNumber - 1];
            tasks.erase(tasks.begin() + actualIndex);
            datBase.updateDataBase();
            clearScreen();
            cout << "Task deleted!\n";
        } else {
            cout << "Invalid task number!\n";
        }
    }
    
    void setPriority() {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
            cout << "No tasks to modify!\n";
            return;
        }
    
        viewTasks();
    
        size_t displayNumber;
        int priority;
        cout << "Task number: ";
        cin >> displayNumber;
        cout << "Priority (1=High, 2=Medium, 3=Low): ";
        cin >> priority;
    
        vector<size_t> taskIndices;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) == 1) {
                taskIndices.push_back(i);
            }
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (get<2>(tasks[i]) != 1) {
                taskIndices.push_back(i);
            }
        }
    
        if (displayNumber > 0 && displayNumber <= taskIndices.size() && priority >= 1 && priority <= 3) {
            size_t actualIndex = taskIndices[displayNumber - 1];
            get<2>(tasks[actualIndex]) = priority;
            datBase.updateDataBase();
            clearScreen();
            cout << "Priority updated!\n";
        } else {
            cout << "Invalid input!\n";
        }
    }

    void sortTasks() {
        int choice;
        do {
            clearScreen();
            viewTasks(false);
            displaySortMenu();
             cin >> choice;

            switch (choice) {
                case 1:
                    datBase.sortAlphabetically();
                    clearScreen();
                     cout << GREEN << "Tasks sorted alphabetically!\n" << RESET;
                    viewTasks();
                    break;
                case 2:
                    datBase.sortByCreationDate();
                    clearScreen();
                     cout << BLUE << "Tasks sorted by creation date!\n" << RESET;
                    viewTasks();
                    break;
                case 3:
                    break;
                default:
                     cout << RED << "Invalid option!\n" << RESET;
            }
        } while (choice != 4);
    }

public:
    void run() {
        datBase.loadData();
        int choice = 0;

        while (choice != 6) {
            clearScreen();
            viewTasks();
            displayMainMenu();
             cin >> choice;

            switch (choice) {
                case 1: addTask(); break;
                case 2: toggleTask(); break;
                case 3: setPriority(); break;
                case 4: deleteTask(); break;
                case 5: sortTasks(); break;
                case 6: clearScreen();  cout << GREEN << "Goodbye!\n" << RESET; break;
                default:
                     cout << RED << "Invalid choice!\n" << RESET;
                     cin.clear();
                     cin.ignore( numeric_limits< streamsize>::max(), '\n');
            }
        }
    }
};

int main() {
    ToDoApp app;
    app.run();
    return 0;
}