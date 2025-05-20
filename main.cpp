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
         cout << "[1] Add task      [2] Toggle complete  [3] Pin/Unpin Task\n";
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
            cout << YELLOW << "                 >>> PINNED TASKS <<<\n" << RESET;
            displayNumber = displayTaskList(highPriority, displayNumber);
        }
    
        if (!normalPriority.empty() && showPriorityHeaders) {
            cout << "\n                     >>> TASKS <<<\n";
        }
        displayTaskList(normalPriority, displayNumber);
    }
    
    size_t displayTaskList(const vector<tuple<string, bool, int, time_t>>& tasks, size_t startNumber = 1) {
        size_t currentNumber = startNumber;
        for (const auto& [task, completed, priority, created] : tasks) {
            string color;
            if (completed) {
                color = STRIKE + GREEN;
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
			clearScreen();
            viewTasks();
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                      ADD TASK";
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";                       
                          
         cout << "Enter new task: ";
         cin.ignore();
         getline( cin, currentInput);
	while(true){
	
        if (!currentInput.empty()) {
            int priority;
            cout << "\n__________________________________________________________\n"; 
             cout << "\nPin this task? ([1] Yes, [2] No): ";
             cin >> priority;
            if (priority == 1 ) {priority = 1; 
            datBase.addTask(currentInput, priority);
            clearScreen();
             cout << GREEN << "Task added successfully!\n" << RESET;
			 break;}; 
            if (priority == 2 ) {priority = 2;
			datBase.addTask(currentInput, priority);
            clearScreen();
             cout << GREEN << "Task added successfully!\n" << RESET;
			 break;};
            if (cin.fail() || !(priority == 1 && priority == 2) ){
            	cin.clear();
cin.ignore( numeric_limits< streamsize>::max(), '\n');
clearScreen();
cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
viewTasks();
cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                      ADD TASK"; 
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
            cout << "Enter new task: " << currentInput;
			}
		}
			}
			
        }
    

    void toggleTask() {
        auto& tasks = datBase.getToDoList();
        if (tasks.empty()) {
            cout << "No tasks to toggle!\n";
            return;
        }
        
        clearScreen();
        viewTasks();
        while(true){
        // First display the tasks with their current numbering
        cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";    
        cout << "                      TOGGLE TASK"; 
         cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";      
        size_t displayNumber;
        cout << "Toggle task number: ";
        cin >> displayNumber;
        
        if (!cin.fail()){
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
            cout << BOLD << YELLOW << "Task toggled!\n" << RESET;
			 break;
			 }
        
		else {cin.clear();
cin.ignore( numeric_limits< streamsize>::max(), '\n');
clearScreen();
cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
viewTasks();
		}
		}
		if(cin.fail()) {cin.clear();
cin.ignore( numeric_limits< streamsize>::max(), '\n');
clearScreen();
cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
viewTasks();
		}
		 
         
	}          
        
      
        }
    
        
    

void deleteTask() {
    auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
        cout << "No tasks to delete!\n";
        return;
    }

    clearScreen();
    viewTasks();

    while (true) {
        cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
        cout << "                      DELETE TASK";
        cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
        
        size_t displayNumber;
        cout << "Delete task number: ";
        cin >> displayNumber;
        
        

        clearScreen();
        viewTasks();
		if(!cin.fail()){
			
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
        while (true) {
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                      DELETE TASK";
            cout << "\n                 [1] Yes        [2] No";
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";

            size_t confirmNumber;
            cout << "Confirm Delete? : ";
            cin >> confirmNumber;

            if (cin.fail() || confirmNumber != 1 && confirmNumber != 2 ) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clearScreen();
                cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
                cout << "                    INPUT ERROR!";
                cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
                viewTasks();
                continue;  // Ask for confirmation again
            }

            if (confirmNumber == 1) {
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
                    cout << BOLD << RED << "Task deleted!\n" << RESET;
                } else {
                    cout << BOLD << RED << "Invalid task number!\n" << RESET;
                }
                return;  // Done, exit function

            } else if (confirmNumber == 2) {
            	clearScreen();
                cout << "Deletion Cancelled\n";
                return;  // Exit function without deleting

            } else {
                cout << "Please enter 1 (Yes) or 2 (No).\n";
                // Loop again for valid input
            }
        }
    }
    }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                    INPUT ERROR!";
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
            viewTasks();
            continue;  // Prompt again
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                    INPUT ERROR!";
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
            viewTasks();
            continue;  // Prompt again
    }
}
}
    void setPriority() {
 auto& tasks = datBase.getToDoList();
    if (tasks.empty()) {
        cout << "No tasks to modify!\n";
        return;
    }

    clearScreen();
    viewTasks();
    while(true){
        cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
        cout << "               	TOGGLE PIN/UNPIN TASK"; 
        cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";       
	
		size_t displayNumber;
        cout << "Task number: ";
        cin >> displayNumber;

        if(!cin.fail()){
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
                get<2>(task) = (get<2>(task) == 1) ? 2 : 1; // Toggle pin state: 1 => 2, 2 => 1
                datBase.updateDataBase();
                clearScreen();
                cout << BOLD << YELLOW << "Task pin state toggled!\n" << RESET;
            break;
			} else {
            cin.clear();
            cin.ignore( numeric_limits< streamsize>::max(), '\n');
            clearScreen();
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
            viewTasks();
        }
                
            }
            if(cin.fail()) {cin.clear();
cin.ignore( numeric_limits< streamsize>::max(), '\n');
clearScreen();
cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
			cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
viewTasks();
		}
            
        }

        
        }
    

    void sortTasks() {
        int choice;
        
            clearScreen();
            viewTasks();
        do {
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
                    
                case 4:
                	clearScreen();
					break;    
                default:
                     cin.clear();
            cin.ignore( numeric_limits< streamsize>::max(), '\n');
            clearScreen();
            cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
            cout << "                    INPUT ERROR!";  
            cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";     
            viewTasks();
            }
        } while (choice != 4);
    }
    
    void mainPage(){
string enterKey;
cout <<  "+----------------------------------------------------+\n";
    cout <<BOLD << GREEN << R"(       _________ _______    ______   _______ 
       \__   __/(  ___  )  (  __  \ (  ___  )
   	  ) (   | (   ) |  | (  \  )| (   ) |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | |   | |  | |   | || |   | |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | (___) |  | (__/  )| (___) |
   	  )_(   (_______)  (______/ (_______)
                                      
  	   _       _________ _______ _________
  	  ( \      \__   __/(  ____ \\__   __/
   	  | (         ) (   | (    \/   ) (   
   	  | |         | |   | (_____    | |   
   	  | |         | |   (_____  )   | |   
   	  | |         | |         ) |   | |   
   	  | (____/\___) (___/\____) |   | |   
   	  (_______/\_______/\_______)   )_(   
                                      
)" << RESET;
cout << "______________________________________________________\n";
cout << BOLD << YELLOW << "       Stay focused. Stay organized. Stay in control.\n" << RESET;
cout << R"(
This to-do list app is more than just a task manager, 
it's your daily partner in productivity. Whether it's 
managing school projects, planning your day, or 
tracking personal goals, this app helps you stay on 
top of everything. Add tasks, set priorities, mark 
them done, and make progress one step at a time.)";
cout <<  "\n+----------------------------------------------------+\n";
cout << "Enter any key to continue. ";
cin >> enterKey;
	
}

public:
    void run() {
        datBase.loadData();
        int choice = 0;
		mainPage();
        
            clearScreen();
            viewTasks();
            while (choice != 6) {
            displayMainMenu();
             cin >> choice;

            switch (choice) {
                case 1: addTask(); viewTasks(); break ;
                case 2: toggleTask(); viewTasks(); break;
                case 3: setPriority(); viewTasks(); break;
                case 4: deleteTask();viewTasks();  break;
                case 5: sortTasks(); viewTasks();  break;
                case 6: clearScreen();  cout <<  "+----------------------------------------------------+\n";
    cout <<BOLD << GREEN << R"(       _________ _______    ______   _______ 
       \__   __/(  ___  )  (  __  \ (  ___  )
   	  ) (   | (   ) |  | (  \  )| (   ) |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | |   | |  | |   | || |   | |
   	  | |   | |   | |  | |   ) || |   | |
   	  | |   | (___) |  | (__/  )| (___) |
   	  )_(   (_______)  (______/ (_______)
                                      
  	   _       _________ _______ _________
  	  ( \      \__   __/(  ____ \\__   __/
   	  | (         ) (   | (    \/   ) (   
   	  | |         | |   | (_____    | |   
   	  | |         | |   (_____  )   | |   
   	  | |         | |         ) |   | |   
   	  | (____/\___) (___/\____) |   | |   
   	  (_______/\_______/\_______)   )_(   
                                      
)" << RESET;
cout << "______________________________________________________\n";
cout << BOLD << YELLOW << "       Stay focused. Stay organized. Stay in control.\n" << RESET;
cout << BOLD << GREEN << "                        Good Bye!!" << RESET;
cout << BOLD << MAGENTA << "\n              Asoy - Jainar - Labrador - Tortuya" << RESET;
cout <<  "\n+----------------------------------------------------+\n"; break;
                default:
                     cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                clearScreen();
                cout << BOLD << GREEN << "+---------------------------------------------------+" << RESET << "\n";
                cout << "                    INPUT ERROR!";
                cout << BOLD << GREEN << "\n+---------------------------------------------------+" << RESET << "\n";
                viewTasks();
            }
        }
    }
};

 

int main() {
    ToDoApp app;
    app.run();
    return 0;
}