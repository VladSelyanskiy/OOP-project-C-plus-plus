#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "TaskManager.h"
#include "MyTask.h"


// ─────────────────────────────────────────────────────────────────────────────
// Вспомогательные функции ввода
// ─────────────────────────────────────────────────────────────────────────────

static void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt)
{
    int v;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> v) { clearInput(); return v; }
        std::cout << "  [!] Введите целое число.\n";
        clearInput();
    }
}

static double readDouble(const std::string& prompt)
{
    double v;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> v) { clearInput(); return v; }
        std::cout << "  [!] Введите число.\n";
        clearInput();
    }
}

static std::string readLine(const std::string& prompt)
{
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

// ─────────────────────────────────────────────────────────────────────────────
// Построение JSON-параметров задачи в интерактивном режиме
// ─────────────────────────────────────────────────────────────────────────────

static nlohmann::json buildTaskParams()
{
    nlohmann::json params;

    // Метод
    std::cout << "\n  Метод решения:\n"
        << "    1. Гаусс\n"
        << "    2. LU-разложение\n";
    int methodChoice = readInt("  Ваш выбор [1/2]: ");
    params["method"] = (methodChoice == 2) ? "lu" : "gauss";

    // Размерность
    int n = 0;
    while (n < 1 || n > 10)
        n = readInt("  Размер матрицы n (1–10): ");

    // Матрица
    nlohmann::json matrix = nlohmann::json::array();
    std::cout << "  (Введите " << n << " строк, в каждой " << n << " чисел через пробел)\n";
    for (int i = 0; i < n; ++i)
    {
        nlohmann::json row = nlohmann::json::array();
        std::cout << "    A[" << i << "]: ";
        for (int j = 0; j < n; ++j)
        {
            double v;
            std::cin >> v;
            row.push_back(v);
        }
        clearInput();
        matrix.push_back(row);
    }
    params["matrix"] = matrix;

    // Вектор b
    std::cout << "\n  Введите вектор b (" << n << " элементов): ";
    nlohmann::json bVec = nlohmann::json::array();
    for (int i = 0; i < n; ++i)
    {
        double v;
        std::cin >> v;
        bVec.push_back(v);
    }
    clearInput();
    params["b"] = bVec;

    return params;
}

// ─────────────────────────────────────────────────────────────────────────────
// Пункты меню
// ─────────────────────────────────────────────────────────────────────────────

static void menuAddTask(TaskManager& tm)
{
    std::cout << "\n---- Добавление задачи ----\n";
    std::string desc = readLine("  Описание задачи: ");

    try
    {
        nlohmann::json params = buildTaskParams();
        int id = tm.get_max_id() + 1;
        auto task = std::make_unique<MyTask>(id, desc, params);
        tm.add_task(std::move(task));
        std::cout << "Задача #" << id << " добавлена.\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  [!] Ошибка при создании задачи: " << e.what() << "\n";
    }
}

static void menuShowAll(const TaskManager& tm)
{
    std::cout << "\n";
    tm.show_all();
}

static void menuShowByStatus(const TaskManager& tm)
{
    std::cout << "\n---- Фильтр по статусу ----\n"
        << "  1. PENDING\n"
        << "  2. RUNNING\n"
        << "  3. COMPLETED\n"
        << "  4. FAILED\n";
    int ch = readInt("  Статус [1-4]: ");
    TaskStatus s;
    switch (ch)
    {
    case 2:  s = TaskStatus::RUNNING;   break;
    case 3:  s = TaskStatus::COMPLETED; break;
    case 4:  s = TaskStatus::FAILED;    break;
    default: s = TaskStatus::PENDING;   break;
    }
    tm.show_by_status(s);
}

static void menuRunTask(TaskManager& tm)
{
    int id = readInt("\n  ID задачи для выполнения: ");
    tm.run_task(id);
}

static void menuRunAll(TaskManager& tm)
{
    std::cout << "\n";
    tm.run_all_pending();
}

static void menuStatistics(const TaskManager& tm)
{
    tm.show_statistics();
}

static void menuSave(const TaskManager& tm)
{
    std::string fname = readLine("\n  Имя файла для сохранения (например, tasks.json): ");
    if (fname.empty()) fname = "tasks.json";
    tm.save_to_file(fname);
}

static void menuLoad(TaskManager& tm)
{
    std::string fname = readLine("\n  Имя файла для загрузки: ");
    if (fname.empty()) fname = "tasks.json";
    tm.load_from_file(fname);
}

// ─────────────────────────────────────────────────────────────────────────────
// Отображение меню
// ─────────────────────────────────────────────────────────────────────────────

static void printMenu()
{
    std::cout << "\n"
        << "============================================\n"
        << "|     СИСТЕМА УПРАВЛЕНИЯ ЗАДАЧАМИ          |\n"
        << "============================================\n"
        << "|  1. Добавить новую задачу                |\n"
        << "|  2. Показать все задачи                  |\n"
        << "|  3. Показать задачи по статусу           |\n"
        << "|  4. Выполнить задачу (по ID)             |\n"
        << "|  5. Выполнить все ожидающие задачи       |\n"
        << "|  6. Показать статистику                  |\n"
        << "|  7. Сохранить задачи в файл              |\n"
        << "|  8. Загрузить задачи из файла            |\n"
        << "|  9. Выход                                |\n"
        << "============================================\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────

int main()
{
    setlocale(LC_ALL, "Russian");

    TaskManager tm;

    while (true)
    {
        printMenu();
        int choice = readInt("  Выберите пункт [1-9]: ");
        switch (choice)
        {
        case 1: menuAddTask(tm);         break;
        case 2: menuShowAll(tm);         break;
        case 3: menuShowByStatus(tm);    break;
        case 4: menuRunTask(tm);         break;
        case 5: menuRunAll(tm);          break;
        case 6: menuStatistics(tm);      break;
        case 7: menuSave(tm);            break;
        case 8: menuLoad(tm);            break;
        case 9:
            return 0;
        default:
            std::cout << "  [!] Неверный выбор. Введите число от 1 до 9.\n";
            break;
        }
    }
}