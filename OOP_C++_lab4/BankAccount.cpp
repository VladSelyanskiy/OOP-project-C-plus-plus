#include "BankAccount.h"

#include <iostream>
#include <stdexcept>

using namespace std;

// Конструктор с проверкой (слайд 4.5)
BankAccount::BankAccount(const string& name, double initialBalance)
    : owner(name), balance(initialBalance)
{
    if (initialBalance < 0) {
        // TODO: бросить исключение invalid_argument (слайд 4.3)
        // с сообщением "Начальный баланс не может быть отрицательным"
        throw invalid_argument("Начальный баланс не может быть отрицательным");
    }
    cout << "Создан счёт для " << owner << ". Баланс: " << balance << endl;
}

// Метод deposit с проверкой
void BankAccount::deposit(double amount) {
    if (amount <= 0) {
        // TODO: бросить invalid_argument
        throw invalid_argument("Депозит должен быть больше нуля");
    }
    balance += amount;
    cout << "Депозит " << amount << ". Баланс: " << balance << endl;
}

// Метод withdraw с проверкой
void BankAccount::withdraw(double amount) {
    if (amount <= 0) {
        // TODO: бросить invalid_argument
        throw invalid_argument("Депозит должен быть больше нуля");
    }
    if (amount > balance) {
        // TODO: бросить runtime_error (слайд 4.3)
        throw runtime_error("Недостаточно средств на балансе");
    }
    balance -= amount;
    cout << "Снятие " << amount << ". Баланс: " << balance << endl;
}

// TODO: Добавьте геттер для balance с noexcept (слайд 4.6)
double BankAccount::getBalance() noexcept
{
    return balance;
}

void BankAccount::print() const {
    cout << owner << ", баланс: " << balance << endl;
}
