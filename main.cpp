#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;

// open (create) json file
std::fstream f;

// check if file is empty
bool isFileEmpty(const std::string &filename)
{
    std::ifstream file(filename);
    return file.peek() == std::ifstream::traits_type::eof();
}

// load tasks from file or initialize as empty array
json loadTasks(const std::string &filename)
{

    // checking if file is empty
    if (isFileEmpty(filename))
    {
        return json::array();
    }

    // try to parse non-empty file
    std::ifstream f(filename);
    try
    {
        json tasks = json::parse(f);
        f.close();
        return tasks;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << "\n";
        return json::array();
    }
}

// generate next id acording to exsisting ids
int generateId(const json &tasks)
{
    int highest_id = 0;
    for (const auto &task : tasks)
    {
        if (task["id"] > highest_id)
        {
            highest_id = task["id"];
        }
    }
    return highest_id + 1;
}

int main(int argc, char *argv[])
{

    // check arguments
    if (argc <= 1)
    {
        std::cout << "\nno arguments.\ntry 'task-tracker help' to see available commands.\n"
                  << std::endl;
        return 0;
    }

    // get first argument
    std::string command(argv[1]);

    /* --------------- commands --------------- */

    if (command == "help")
    {
        std::cout << "\nlist of available commands:\n\n";
        std::cout << " - add <task name>\n";
        std::cout << " - update <id> <new-text>\n";
        std::cout << " - delete <id>\n";
        std::cout << " - mark-in-progress <id>\n";
        std::cout << " - mark-done <id>\n";
        std::cout << " - list\n\n";
    }

    if (command == "add")
    {

        // check if second argument was given
        if (argc <= 2)
        {
            std::cout << "\nNot enough arguments (expected 2).\n\n";
            return 0;
        }

        // get task information
        std::string taskName(argv[2]);
        json tasks = loadTasks("tasks.json");
        int id = generateId(tasks);

        // create new task
        json newTask;
        newTask["id"] = id;
        newTask["name"] = taskName;

        // push new task
        tasks.push_back(newTask);

        // write changes
        std::ofstream outFile("tasks.json");
        outFile << tasks.dump(4);
        outFile.close();
        return 0;
    }

    if (command == "update")
    {

        // check if second and third arguments were given
        if (argc <= 3)
        {
            std::cout << "\nNot enough arguments (expected 2).\n\n";
            return 0;
        }

        // get the tasks array
        json tasks = loadTasks("tasks.json");

        // get task information
        int id = std::stoi(argv[2]);
        std::string newTaskName = argv[3];

        // edit task
        for (auto &task : tasks)
        {
            if (task["id"] == id)
            {
                task["name"] = newTaskName;
            }
        }

        // write changes
        std::ofstream outFile("tasks.json");
        outFile << tasks.dump(4);
        outFile.close();
        return 0;
    }

    if (command == "delete")
    {
        std::cout << "cucu\n";
        // check if second argument was given
        if (argc <= 2)
        {
            std::cout << "\nNot enough arguments (expected 2).\n\n";
            return 0;
        }

        // get the tasks array
        json tasks = loadTasks("tasks.json");

        // get task information
        int id = std::stoi(argv[2]);

        // edit task
        json::iterator it;
        for (json::iterator it = tasks.begin(); it != tasks.end();) {
        if ((*it)["id"] == id) {
            it = tasks.erase(it); 
        } else {
            ++it;
        }
}

        // write changes
        std::ofstream outFile("tasks.json");
        outFile << tasks.dump(4);
        outFile.close();
        return 0;
    }

    if (command == "mark-in-progress")
    {
    }

    if (command == "mark-done")
    {
    }

    if (command == "list")
    {
    }

    return 0;
}
