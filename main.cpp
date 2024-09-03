#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;

// open (create) json file
std::fstream f;

int generateId() {
    f.open("tasks.json", std::ios::in);
    json tasks = json::parse(f);
    int highest_id = 0;

    for (auto &task : tasks) {
        if (task["id"] > highest_id) {
            highest_id = task["id"];
            highest_id++;
        }
    }
    std::cout << "[DEBUG] new id is " << highest_id;
    f.close();
}

int main(int argc, char *argv[]) {


    // check what first arg is
        /*  add,
            update,
            delete, 
            mark-in-progress,
            mark-done, 
            list
        */

    if (argc <= 1) {
        std::cout << "\nno arguments.\ntry 'task-tracker help' to see available commands.\n" << std::endl;
        return 0;
    }

    std::string command(argv[1]); // get first argument

    if (command.compare("help") == 0) {
        std::cout << "\nlist of available commands:\n\n";
        std::cout << " - add <task name>\n";
        std::cout << " - update <id> <new-text>\n";
        std::cout << " - delete <id>\n";
        std::cout << " - mark-in-progress <id>\n";
        std::cout << " - mark-done <id>\n";
        std::cout << " - list\n\n";
    }

    if (command.compare("add") == 0) {
        f.open("tasks.json", std::ios::out);
        id = generateId();
        json newTask = {{"id", id}} 
        f.close();
    }

    if (command.compare("update") == 0) {
        
    }

    if (command.compare("delete") == 0) {
        
    }

    if (command.compare("mark-in-progress") == 0) {
        
    }

    if (command.compare("mark-done") == 0) {
        
    }

    if (command.compare("list") == 0) {
        
    }

    return 0;
}