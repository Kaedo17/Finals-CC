#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <limits>
#include "database.h" // Using the database class from previous conversion

class ToDoApp {
private:
    ToDoDataBase db;
    std::string currentInput;

    void displayMenu() {
        std::cout << "\n=== TO-DO LIST ===\n";
        std::cout << "1. View all tasks\n";
        std::cout << "2. Add new task\n";
        std::cout << "3. Toggle task completion\n";
        std::cout << "4. Delete task\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose an option: ";
    }

    void viewTasks() {
        const auto& tasks = db.getToDoList();
        if (tasks.empty()) {
            std::cout << "\nNo tasks found!\n";
            return;
        }

        std::cout << "\n=== YOUR TASKS ===\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            const auto& task = tasks[i];
            std::cout << i + 1 << ". [" << (std::get<1>(task) ? "X" : " ") << "] "
                      << std::get<0>(task) << "\n";
        }
    }

    void addTask() {
        std::cout << "\nEnter new task: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, currentInput);

        if (!currentInput.empty()) {
            auto tasks = db.getToDoList();
            tasks.emplace_back(currentInput, false);
            db.setToDoList(tasks);
            db.updateDataBase();
            std::cout << "Task added successfully!\n";
        }
    }

    void toggleTask() {
        viewTasks();
        if (db.getToDoList().empty()) return;

        std::cout << "Enter task number to toggle: ";
        size_t index;
        std::cin >> index;

        if (index > 0 && index <= db.getToDoList().size()) {
            auto tasks = db.getToDoList();
            auto& task = tasks[index - 1];
            std::get<1>(task) = !std::get<1>(task); // Toggle completion status
            db.setToDoList(tasks);
            db.updateDataBase();
            std::cout << "Task updated!\n";
        } else {
            std::cout << "Invalid task number!\n";
        }
    }

    void deleteTask() {
        viewTasks();
        if (db.getToDoList().empty()) return;

        std::cout << "Enter task number to delete: ";
        size_t index;
        std::cin >> index;

        if (index > 0 && index <= db.getToDoList().size()) {
            auto tasks = db.getToDoList();
            tasks.erase(tasks.begin() + index - 1);
            db.setToDoList(tasks);
            db.updateDataBase();
            std::cout << "Task deleted!\n";
        } else {
            std::cout << "Invalid task number!\n";
        }
    }

public:
    void run() {
        db.loadData(); // Load existing data or create initial data

        int choice = 0;
        while (choice != 5) {
            displayMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    viewTasks();
                    break;
                case 2:
                    addTask();
                    break;
                case 3:
                    toggleTask();
                    break;
                case 4:
                    deleteTask();
                    break;
                case 5:
                    std::cout << "Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid choice! Try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
};

int main() {
    ToDoApp app;
    app.run();
    return 0;
}