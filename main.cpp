#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

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
        return tasks;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << "\n";
        return json::array();
    }
}

// save tasks to file
void saveTasks(const json &tasks, const std::string filename)
{
    std::ofstream outFile(filename);
    outFile << tasks.dump(4);
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
        std::cout << "\nno arguments.\ntry 'task-tracker help' to see available "
                     "commands.\n"
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
        newTask["status"] = "todo";

        // push new task
        tasks.push_back(newTask);

        // save to file
        saveTasks(tasks, "tasks.json");
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

        // save to file
        saveTasks(tasks, "tasks.json");
        return 0;
    }

    if (command == "delete")
    {
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

        // erase task
        json::iterator it;
        for (json::iterator it = tasks.begin(); it != tasks.end();)
        {
            if ((*it)["id"] == id)
            {
                it = tasks.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // save to file
        saveTasks(tasks, "tasks.json");
        return 0;
    }

    if (command == "mark-in-progress")
    {
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
        for (auto &task : tasks)
        {
            if (task["id"] == id)
            {
                task["status"] = "in-progress";
            }
        }

        // save to file
        saveTasks(tasks, "tasks.json");
        return 0;
    }

    if (command == "mark-done")
    {
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
        for (auto &task : tasks)
        {
            if (task["id"] == id)
            {
                task["status"] = "done";
            }
        }

        // save to file
        saveTasks(tasks, "tasks.json");
        return 0;
    }

    if (command == "list")
    {
        // get the tasks array
        json tasks = loadTasks("tasks.json");

        // check if any flags were given
        if (argc > 2)
        {
            // get flag
            std::string flag = argv[2];

            std::cout << "\n";
            std::cout << "[" << std::string(flag) << "] \n";

            // print tasks of specified flag
            for (auto &task : tasks)
            {
                if (task["status"] == flag)
                {
                    std::cout << "id " << task["id"] << ", " << task["name"] << "\n";
                }
            }
            std::cout << "\n";
        }
        else
        {
            // print all tasks
            std::cout << "\n";
            for (auto &task : tasks)
            {
                std::cout << "id " << task["id"] << ", [" << std::string(task["status"])
                          << "] " << task["name"] << "\n";
            }
            std::cout << "\n";
        }
    }

    return 0;
}
