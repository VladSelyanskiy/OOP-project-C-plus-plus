#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include "Task.h"

enum class Method
{
    GAUSS,
    LU
};

class MyTask : public Task
{
private:
    // ── Входные параметры ────────────────────────────────────────────────────
    Method          method;     // Выбранный метод решения
    Eigen::MatrixXd A;          // Матрица коэффициентов (n×n, n ≤ 10)
    Eigen::VectorXd b;          // Вектор правой части

    // ── Результаты (заполняются в execute()) ─────────────────────────────────
    Eigen::VectorXd solution;           // Решение x
    Eigen::VectorXd noisySolution;      // Решение при возмущённой правой части
    double          conditionNumber;    // Приближённое число обусловленности
    double          sensitivity;        // ||x - x_noisy|| / ||x||  (чувствительность)
    bool            executed = false;   // Флаг: execute() уже был вызван

    // ── Вспомогательные методы ───────────────────────────────────────────────

    // Метод Гаусса с частичным выбором ведущего элемента
    // Возвращает решение, модифицирует переданную копию матрицы и вектора
    Eigen::VectorXd solveGauss(Eigen::MatrixXd Ac, Eigen::VectorXd bc) const;

    // LU-разложение (через Eigen::PartialPivLU)
    Eigen::VectorXd solveLU(const Eigen::MatrixXd& Ac,
        const Eigen::VectorXd& bc) const;

    // Приближённое число обусловленности: ||A|| * ||A⁻¹||  (норма-1)
    double computeConditionNumber(const Eigen::MatrixXd& Ac) const;

public:
    // Конструктор: принимает ID, описание и JSON с параметрами.
    // Ожидаемый формат JSON:
    // {
    //   "method": "gauss" | "lu",
    //   "matrix": [[...], [...], ...],   // n строк × n столбцов
    //   "b":      [...]                  // n элементов
    // }
    MyTask(int id, const std::string& desc, const nlohmann::json& params);

    // Основной метод — вся математика здесь
    void execute() override;

    // Сериализация: возвращает JSON с параметрами и результатами
    nlohmann::json to_json() const override;

    // Красивый вывод результатов в консоль
    void print_result() const;
};