#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

#include "Task.h"
#include "TaskManager.h"
#include "MyTask.h"

// Добавление задачи
void TaskManager::add_task(std::unique_ptr<Task> task)
{
    if (task)
    {
        tasks.push_back(std::move(task));
        std::cout << "Task added successfully. Total tasks: " << tasks.size() << std::endl;
    }
}

// Управление

// Выполнение конкретной задачи по ID
void TaskManager::run_task(int id)
{
    for (auto& task : tasks)
    {
        if (task->get_id() == id)
        {
            // Проверка статуса задачи на ожидание
            if (task->get_status() == TaskStatus::PENDING)
            {
                std::cout << "Running task ID: " << id << std::endl;
                try
                {
                    task->execute();
                }
                catch (const std::exception& e)
                {
                    task->addToLog("Error during task execution: " + std::string(e.what()));
                    std::cerr << "Error: " << e.what() << std::endl;
                    return;
                }
                std::cout << "Task completed with status: " << task->get_status_string() << std::endl;
            }
            else
            {
                std::cout << "Task ID " << id << " is not pending (status: "
                    << task->get_status_string() << ")" << std::endl;
            }
            return;
        }
    }
    std::cout << "Task with ID " << id << " not found!" << std::endl;
}

// Выполнение всех ожидающих задач
void TaskManager::run_all_pending()
{
    int pending_count = 0;
    int completed_count = 0;

    for (auto& task : tasks)
    {
        if (task->get_status() == TaskStatus::PENDING)
        {
            pending_count++;
            std::cout << "Running task ID: " << task->get_id()
                << " - " << task->get_status_string() << std::endl;
            try
            {
                task->execute();
            }
            catch (const std::exception& e)
            {
                task->addToLog("Error during task execution: " + std::string(e.what()));
                std::cerr << "Error: " << e.what() << std::endl;
                return;
            }
            std::cout << "Task finished with status: " << task->get_status_string() << std::endl;
            completed_count++;
        }
    }

    std::cout << "\nExecuted " << completed_count << " out of " << pending_count
        << " pending tasks" << std::endl;
}

// Удаление задачи
void TaskManager::remove_task(int id)
{
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [id](const std::unique_ptr<Task>& task)
        {
            return task->get_id() == id;
        });

    if (it != tasks.end()) {
        std::cout << "Removing task ID: " << id << std::endl;
        tasks.erase(it);
        std::cout << "Task removed. Remaining tasks: " << tasks.size() << std::endl;
    }
    else {
        std::cout << "Task with ID " << id << " not found!" << std::endl;
    }
}

// Отображение (константные методы)

// Показать все задачи
void TaskManager::show_all() const
{
    if (tasks.empty())
    {
        std::cout << "No tasks in the manager." << std::endl;
        return;
    }
    std::cout << "\n=== ALL TASKS ===" << std::endl;
    std::cout << "Total tasks: " << tasks.size() << "\n" << std::endl;

    for (const auto& task : tasks) {
        std::cout << "ID: " << task->get_id() << std::endl;
        std::cout << "Status: " << task->get_status_string() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

// Показать задачи по статусу
void TaskManager::show_by_status(TaskStatus status) const
{
    std::vector<const Task*> filtered_tasks;

    for (const auto& task : tasks)
    {
        if (task->get_status() == status) {
            filtered_tasks.push_back(task.get());
        }
    }

    if (filtered_tasks.empty())
    {
        std::cout << "No tasks with this status" << std::endl;
        return;
    }

    std::cout << "\n=== TASKS WITH THIS STATUS" << " ===" << std::endl;
    std::cout << "Found: " << filtered_tasks.size() << " tasks\n" << std::endl;

    for (const auto* task : filtered_tasks)
    {
        std::cout << "ID: " << task->get_id() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

// Показать статистику
void TaskManager::show_statistics() const
{
    if (tasks.empty())
    {
        std::cout << "No tasks to show statistics." << std::endl;
        return;
    }

    int pending = 0, running = 0, completed = 0, failed = 0;

    for (const auto& task : tasks)
    {
        switch (task->get_status())
        {
        case TaskStatus::PENDING:   pending++;   break;
        case TaskStatus::RUNNING:   running++;   break;
        case TaskStatus::COMPLETED: completed++; break;
        case TaskStatus::FAILED:    failed++;    break;
        }
    }

    std::cout << "\n=== TASK STATISTICS ===" << std::endl;
    std::cout << "Total tasks:   " << tasks.size() << std::endl;
    std::cout << "Pending:       " << pending << " (" << (pending * 100 / tasks.size()) << "%)" << std::endl;
    std::cout << "Running:       " << running << " (" << (running * 100 / tasks.size()) << "%)" << std::endl;
    std::cout << "Completed:     " << completed << " (" << (completed * 100 / tasks.size()) << "%)" << std::endl;
    std::cout << "Failed:        " << failed << " (" << (failed * 100 / tasks.size()) << "%)" << std::endl;
    std::cout << "========================\n" << std::endl;
}

// Работа с файлами

// Сохранить в файл
void TaskManager::save_to_file(const std::string& filename) const {
    nlohmann::json j;
    j["task_count"] = tasks.size();

    nlohmann::json tasks_array = nlohmann::json::array();
    for (const auto& task : tasks) {
        tasks_array.push_back(task->to_json());
    }
    j["tasks"] = tasks_array;

    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); // pretty print с отступами
        std::cout << "Successfully saved " << tasks.size() << " tasks to " << filename << std::endl;
    }
    else {
        std::cerr << "Error: Could not open file " << filename << " for writing!" << std::endl;
    }
}

// Загрузить из файла
void TaskManager::load_from_file(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for reading!" << std::endl;
        return;
    }

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "Error: JSON parse error — " << e.what() << std::endl;
        return;
    }

    if (!j.contains("tasks") || !j["tasks"].is_array())
    {
        std::cerr << "Error: Invalid file format!" << std::endl;
        return;
    }

    // Собираем уже занятые ID, чтобы не создавать дубликаты
    std::vector<int> existingIds;
    for (const auto& t : tasks)
        existingIds.push_back(t->get_id());

    int added = 0, skipped = 0;

    for (const auto& task_json : j["tasks"])
    {
        // Валидация обязательных полей
        if (!task_json.contains("id") || !task_json.contains("matrix") ||
            !task_json.contains("b") || !task_json.contains("method"))
        {
            std::cerr << "  [!] Пропуск задачи: отсутствуют обязательные поля.\n";
            ++skipped;
            continue;
        }

        int id = task_json["id"].get<int>();

        // Меняем ID чтобы не было дубликатов
        if (std::find(existingIds.begin(), existingIds.end(), id) != existingIds.end())
        {
            id = get_max_id() + 1;
        }

        try
        {
            std::string desc = task_json.value("description", "Загружено из файла");
            // Передаём весь task_json как params — MyTask умеет читать
            // поля "method", "matrix", "b" напрямую из него
            auto task = std::make_unique<MyTask>(id, desc, task_json);
            existingIds.push_back(id);
            tasks.push_back(std::move(task));
            std::cout << "Задача ID " << id << " добавлена.\n";
            ++added;
        }
        catch (const std::exception& e)
        {
            std::cerr << "  [!] Ошибка при создании задачи ID " << id
                << ": " << e.what() << "\n";
            ++skipped;
        }
    }

    std::cout << "\nЗагрузка завершена: добавлено " << added
        << ", пропущено " << skipped << ".\n";
}

// Поиск задач по описанию
std::vector<int> TaskManager::find_tasks_by_description(const std::string& pattern) const
{
    std::vector<int> found_descriptions;

    for (const auto& task : tasks)
    {
        // Здесь нужен доступ к description (нужно добавить геттер в Task)
        // Пока ищем по ID как пример
        if (task->get_description().find(pattern) != std::string::npos)
        {
            found_descriptions.push_back(task->get_id());
        }
    }

    return found_descriptions;
}

int TaskManager::get_max_id() const
{
    int maxId = 0;
    for (const auto& task : tasks)
        maxId = std::max(maxId, task->get_id());
    return maxId;
}

