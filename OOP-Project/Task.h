#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

enum class TaskStatus
{
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED
};

// Преобразование enum в строку (объявление)
std::string task_status_to_string(TaskStatus status);

class Task {
protected:
    int id;
    std::string description;
    TaskStatus status = TaskStatus::PENDING;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point completed_at;
    std::vector<std::string> log;

public:
    // Конструктор
    Task(int id, const std::string& desc);

    // Виртуальный деструктор
    virtual ~Task() = default;

    // Запрет копирования (для unique_ptr)
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // Разрешаем перемещение
    Task(Task&&) = default;
    Task& operator=(Task&&) = default;

    // Чисто виртуальный метод — сердце задачи
    virtual void execute() = 0;

    // Сериализация (виртуальные методы)
    virtual nlohmann::json to_json() const;
    virtual void from_json(const nlohmann::json& j);

    // Геттеры (константные!)
    int get_id() const noexcept { return id; }
    TaskStatus get_status() const noexcept { return status; }
    std::string get_status_string() const;
    std::chrono::system_clock::time_point get_created_at() const noexcept { return created_at; }
    std::chrono::system_clock::time_point get_completed_at() const noexcept { return completed_at; }
    virtual const std::string& get_description() const { return description; }

    void addToLog(const std::string& message)
    {
        log.push_back(message);
    }
};