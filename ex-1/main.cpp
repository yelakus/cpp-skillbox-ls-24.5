#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

struct task
{
    bool is_active = false;
    std::string name = "unknown";
    std::time_t start_time;
    std::time_t end_time;
    double total_time;
};


std::string get_comand(){
    std::string comand;
    std::cin >> comand;
    return comand;
}

void end_comand(bool* is_there_a_active_task, task* active_task,std::vector<task>* tasks_history)
{
    if(!(*is_there_a_active_task)){
        std::cout << "No active task" << std::endl;
        return;
    }

    active_task->end_time = std::time(nullptr);
    active_task->total_time = std::difftime(active_task->end_time, active_task->start_time);
    *is_there_a_active_task = false;

    tasks_history->push_back(*active_task);

    std::cout << "The active task has been closed" << std::endl;

}

void begin_comand(bool* is_there_a_active_task, task* active_task,std::vector<task>* tasks_history)
{
    if (*is_there_a_active_task) end_comand(is_there_a_active_task, active_task, tasks_history);

    active_task->start_time = std::time(nullptr);
    active_task->is_active = true;

    std::cout << "Enter new task title:" << std::endl;
    std::cin.ignore(); 
    std::getline(std::cin, active_task->name);

    *is_there_a_active_task = true;
}

void status_comand(bool* is_there_a_active_task, task* active_task,std::vector<task>* tasks_history)
{
    for (int i = 0; i < tasks_history->size(); i++)
    {
        std::cout << "Task #" + std::to_string(i+1) + ": " + tasks_history->at(i).name << std::endl;

        double total_time_in_hours = tasks_history->at(i).total_time / 3600.0;
        std::cout << "Total time in hours: " << std::fixed << std::setprecision(2) << total_time_in_hours << " hours." << std::endl;
        std::cout << "Total time in seconds: " << std::fixed << std::setprecision(2) << tasks_history->at(i).total_time << " sec." << std::endl;
    }

    if (*is_there_a_active_task) {
        std::cout << "Task " + active_task->name + " is active " << std::endl;
    }
}

int main(){

    std::string users_comand;
    task active_task;
    std::vector<task> tasks_history;
    bool is_there_a_active_task = false;

    while(true){  
        std::cout << "Enter comand(begin, end, status, exit):" << std::endl;
        
        users_comand = get_comand();  

        if(users_comand == "exit")          break;
        else if(users_comand == "begin")    begin_comand(&is_there_a_active_task, &active_task, &tasks_history);
        else if(users_comand == "end")      end_comand(&is_there_a_active_task, &active_task, &tasks_history);
        else if(users_comand == "status")   status_comand(&is_there_a_active_task, &active_task, &tasks_history);
        
    }
    
}