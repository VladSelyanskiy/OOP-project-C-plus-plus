#include <chrono>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Task.h"

// Реализация вспомогательной функции преобразования enum в строку
std::string task_status_to_string(TaskStatus status)
{
    switch (status)
    {
    case TaskStatus::PENDING:   return "PENDING";
    case TaskStatus::RUNNING:   return "RUNNING";
    case TaskStatus::COMPLETED: return "COMPLETED";
    case TaskStatus::FAILED:    return "FAILED";
    default:                    return "UNKNOWN";
    }
}

Task::Task(int id, const std::string& desc)
    : id(id)
    , description(desc)
    , created_at(std::chrono::system_clock::now())
    , completed_at(std::chrono::system_clock::time_point{}) // нулевое значение
{
    // Добавить начальную запись в лог
    log.push_back("Task created with ID: " + std::to_string(id));
}

std::string Task::get_status_string() const
{
    return task_status_to_string(status);
}

nlohmann::json Task::to_json() const
{
    nlohmann::json j;
    j["id"] = id;
    j["description"] = description;
    j["status"] = Task::get_status_string();
    j["created_at"] = std::chrono::system_clock::to_time_t(created_at);
    j["completed_at"] = std::chrono::system_clock::to_time_t(completed_at);
    j["log"] = log;
    return j;
}


void Task::from_json(const nlohmann::json& j)
{
    if (j.contains("id")) id = j["id"].get<int>();
    if (j.contains("description")) description = j["description"].get<std::string>();
    if (j.contains("status")) {
        std::string status_str = j["status"].get<std::string>();
        if (status_str == "PENDING") status = TaskStatus::PENDING;
        else if (status_str == "RUNNING") status = TaskStatus::RUNNING;
        else if (status_str == "COMPLETED") status = TaskStatus::COMPLETED;
        else if (status_str == "FAILED") status = TaskStatus::FAILED;
    }
    if (j.contains("created_at")) {
        std::time_t t = j["created_at"].get<std::time_t>();
        created_at = std::chrono::system_clock::from_time_t(t);
    }
    if (j.contains("completed_at")) {
        std::time_t t = j["completed_at"].get<std::time_t>();
        completed_at = std::chrono::system_clock::from_time_t(t);
    }
    if (j.contains("log")) log = j["log"].get<std::vector<std::string>>();
}