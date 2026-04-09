#include "MyTask.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <random>

// ─────────────────────────────────────────────────────────────────────────────
// Конструктор
// ─────────────────────────────────────────────────────────────────────────────
MyTask::MyTask(int id, const std::string& desc, const nlohmann::json& params)
    : Task(id, desc),
    conditionNumber(0.0),
    sensitivity(0.0)
{
    // ── Метод ────────────────────────────────────────────────────────────────
    std::string m = params.value("method", "gauss");
    if (m == "lu")
        method = Method::LU;
    else
        method = Method::GAUSS;

    // ── Матрица A ────────────────────────────────────────────────────────────
    const auto& mat = params.at("matrix");
    const int n = static_cast<int>(mat.size());
    if (n == 0 || n > 10)
        throw std::invalid_argument("Matrix size must be in [1, 10].");

    A.resize(n, n);
    for (int i = 0; i < n; ++i)
    {
        if (static_cast<int>(mat[i].size()) != n)
            throw std::invalid_argument("Matrix must be square.");
        for (int j = 0; j < n; ++j)
            A(i, j) = mat[i][j].get<double>();
    }

    // ── Вектор b ─────────────────────────────────────────────────────────────
    const auto& bj = params.at("b");
    if (static_cast<int>(bj.size()) != n)
        throw std::invalid_argument("Vector b size must match matrix size.");

    b.resize(n);
    for (int i = 0; i < n; ++i)
        b(i) = bj[i].get<double>();
}

// ─────────────────────────────────────────────────────────────────────────────
// Метод Гаусса с частичным выбором ведущего элемента
// ─────────────────────────────────────────────────────────────────────────────
Eigen::VectorXd MyTask::solveGauss(Eigen::MatrixXd Ac, Eigen::VectorXd bc) const
{
    const int n = static_cast<int>(Ac.rows());

    for (int col = 0; col < n; ++col)
    {
        // Поиск строки с максимальным элементом в текущем столбце
        int pivotRow = col;
        double maxVal = std::abs(Ac(col, col));
        for (int row = col + 1; row < n; ++row)
        {
            if (std::abs(Ac(row, col)) > maxVal)
            {
                maxVal = std::abs(Ac(row, col));
                pivotRow = row;
            }
        }

        if (maxVal < 1e-12)
        {
            throw std::runtime_error("Matrix is singular (or nearly singular).");
        }

        // Перестановка строк
        if (pivotRow != col)
        {
            Ac.row(col).swap(Ac.row(pivotRow));
            std::swap(bc(col), bc(pivotRow));
        }

        // Обнуление элементов ниже ведущего
        for (int row = col + 1; row < n; ++row)
        {
            double factor = Ac(row, col) / Ac(col, col);
            Ac.row(row) -= factor * Ac.row(col);
            bc(row) -= factor * bc(col);
        }
    }

    // Обратный ход
    Eigen::VectorXd x(n);
    for (int i = n - 1; i >= 0; --i)
    {
        x(i) = bc(i);
        for (int j = i + 1; j < n; ++j)
            x(i) -= Ac(i, j) * x(j);
        x(i) /= Ac(i, i);
    }
    return x;
}

// ─────────────────────────────────────────────────────────────────────────────
// LU-разложение (Eigen::PartialPivLU)
// ─────────────────────────────────────────────────────────────────────────────
Eigen::VectorXd MyTask::solveLU(const Eigen::MatrixXd& Ac,
                                const Eigen::VectorXd& bc) const
{
    Eigen::PartialPivLU<Eigen::MatrixXd> lu(Ac);
    // PartialPivLU предполагает невырожденность; проверяем через определитель
    if (std::abs(lu.determinant()) < 1e-12)
    {
        throw std::runtime_error("Matrix is singular (or nearly singular).");
    }
    return lu.solve(bc);
}

// ─────────────────────────────────────────────────────────────────────────────
// Приближённое число обусловленности: cond(A) ≈ ||A||₁ · ||A⁻¹||₁
// ─────────────────────────────────────────────────────────────────────────────
double MyTask::computeConditionNumber(const Eigen::MatrixXd& Ac) const
{
    // ||A||₁  = максимальная сумма по столбцам
    double normA = Ac.cwiseAbs().colwise().sum().maxCoeff();

    // ||A⁻¹||₁ через явное обращение (n ≤ 10, допустимо)
    Eigen::MatrixXd Ainv = Ac.inverse();
    double normAinv = Ainv.cwiseAbs().colwise().sum().maxCoeff();

    return normA * normAinv;
}

// ─────────────────────────────────────────────────────────────────────────────
// execute()  — основная математика
// ─────────────────────────────────────────────────────────────────────────────
void MyTask::execute()
{
    status = TaskStatus::RUNNING;
    log.push_back("Task started");

    // 1. Проверка вырожденности через определитель
    double det = A.determinant();
    if (std::abs(det) < 1e-12)
    {
        status = TaskStatus::FAILED;
        throw std::runtime_error("Matrix is singular: det ~ 0.");
        return;
    }

    // 2. Решение системы выбранным методом
    if (method == Method::GAUSS)
        solution = solveGauss(A, b);
    else
        solution = solveLU(A, b);

    // 3. Число обусловленности
    conditionNumber = computeConditionNumber(A);

    // 4. Малый шум к правой части (ε = 1e-5 · ||b||)
    const double eps = 1e-5 * b.norm();
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-eps, eps);

    Eigen::VectorXd bNoisy = b;
    for (int i = 0; i < bNoisy.size(); ++i)
        bNoisy(i) += dist(rng);

    if (method == Method::GAUSS)
        noisySolution = solveGauss(A, bNoisy);
    else
        noisySolution = solveLU(A, bNoisy);

    // 5. Относительная чувствительность: ||δx|| / ||x||
    double normX = solution.norm();
    sensitivity = (normX > 1e-15)
        ? (noisySolution - solution).norm() / normX
        : (noisySolution - solution).norm();

    status = TaskStatus::COMPLETED;
    completed_at = std::chrono::system_clock::now();
    log.push_back("Task completed successfully");
    executed = true;
}

// ─────────────────────────────────────────────────────────────────────────────
// to_json()
// ─────────────────────────────────────────────────────────────────────────────
nlohmann::json MyTask::to_json() const
{
    nlohmann::json j;
    j["id"] = Task::get_id();
    j["description"] = Task::get_description();
    j["method"] = (method == Method::GAUSS) ? "gauss" : "lu";
    j["status"] = Task::get_status_string();
    j["created_at"] = std::chrono::system_clock::to_time_t(created_at);
    j["completed_at"] = std::chrono::system_clock::to_time_t(completed_at);
    j["log"] = log;

    // Матрица
    nlohmann::json matJson = nlohmann::json::array();
    for (int i = 0; i < A.rows(); ++i)
    {
        nlohmann::json row = nlohmann::json::array();
        for (int k = 0; k < A.cols(); ++k)
            row.push_back(A(i, k));
        matJson.push_back(row);
    }
    j["matrix"] = matJson;

    // Вектор b
    nlohmann::json bJson = nlohmann::json::array();
    for (int i = 0; i < b.size(); ++i)
        bJson.push_back(b(i));
    j["b"] = bJson;

    if (executed)
    {
        // Решение
        nlohmann::json xJson = nlohmann::json::array();
        for (int i = 0; i < solution.size(); ++i)
            xJson.push_back(solution(i));
        j["solution"] = xJson;

        // Решение при шуме
        nlohmann::json xnJson = nlohmann::json::array();
        for (int i = 0; i < noisySolution.size(); ++i)
            xnJson.push_back(noisySolution(i));
        j["noisy_solution"] = xnJson;

        j["condition_number"] = conditionNumber;
        j["sensitivity"] = sensitivity;
    }

    return j;
}

// ─────────────────────────────────────────────────────────────────────────────
// print_result()
// ─────────────────────────────────────────────────────────────────────────────
void MyTask::print_result() const
{
    const int w = 14;
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "\n══════════════════════════════════════════════════════\n";
    std::cout << "  Task #" << Task::get_id()
        << "  [" << Task::get_description() << "]\n";
    std::cout << "  Метод: " << (method == Method::GAUSS ? "Гаусс" : "LU-разложение") << "\n";
    std::cout << "──────────────────────────────────────────────────────\n";

    if (!executed)
    {
        std::cout << "  execute() ещё не был вызван.\n";
        std::cout << "══════════════════════════════════════════════════════\n";
        return;
    }

    // Решение
    std::cout << "  Решение x:\n";
    for (int i = 0; i < solution.size(); ++i)
        std::cout << "    x[" << i << "] = " << std::setw(w) << solution(i) << "\n";

    // Число обусловленности
    std::cout << "\n  Число обусловленности (прибл.): "
        << std::scientific << conditionNumber << std::fixed << "\n";
    if (conditionNumber > 1e6)
        std::cout << "Матрица плохо обусловлена!\n";

    // Чувствительность
    std::cout << "\n  Чувствительность к шуму (||δx||/||x||): "
        << std::scientific << sensitivity << std::fixed << "\n";

    // Разница решений
    std::cout << "\n  Решение при возмущённой правой части:\n";
    for (int i = 0; i < noisySolution.size(); ++i)
    {
        double diff = noisySolution(i) - solution(i);
        std::cout << "    x_noisy[" << i << "] = " << std::setw(w) << noisySolution(i)
            << "   (δ = " << std::showpos << diff << std::noshowpos << ")\n";
    }

    std::cout << "══════════════════════════════════════════════════════\n\n";
}
