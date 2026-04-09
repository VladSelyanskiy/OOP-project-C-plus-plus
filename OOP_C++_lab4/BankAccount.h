#pragma once

//#include <iostream>
#include <stdexcept>
using namespace std;

class BankAccount {
private:
    string owner;
    double balance;

public:
    // Конструктор с проверкой
    BankAccount(const string& name, double initialBalance);

    // Метод deposit с проверкой
    void deposit(double amount);

    // Метод withdraw с проверкой
    void withdraw(double amount);
    
    // TODO: Добавьте геттер для balance с noexcept (слайд 4.6)
    double getBalance() noexcept;

    // Метод print
    void print() const;
};