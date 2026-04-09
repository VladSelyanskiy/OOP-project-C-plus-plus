#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

#include "Task.h"

class TaskManager {
private:
    std::vector<std::unique_ptr<Task>> tasks;

public:
    TaskManager() = default;
    ~TaskManager() = default;

    // Запрет копирования
    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    // Разрешаем перемещение
    TaskManager(TaskManager&&) = default;
    TaskManager& operator=(TaskManager&&) = default;

    // Добавление задачи
    void add_task(std::unique_ptr<Task> task);

    // Управление
    void run_task(int id);
    void run_all_pending();
    void remove_task(int id);

    // Отображение (константные методы)
    void show_all() const;
    void show_by_status(TaskStatus status) const;
    void show_statistics() const;

    // Работа с файлами
    void save_to_file(const std::string& filename) const;
    void load_from_file(const std::string& filename);

    // Поиск
    std::vector<int> find_tasks_by_description(const std::string& pattern) const;

    int get_max_id() const;
};