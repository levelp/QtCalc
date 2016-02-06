#ifndef COMMAND_H
#define COMMAND_H

// Команда - одна операция
class Command {
 public:
  double a, b; // Аргументы операции
  double result; // Результат операции
  char operation; // Символ операции
  virtual void execute() = 0; // Чистый виртуальный метод
  //    virtual void undo() = 0; // Чистый виртуальный метод
};

class Add : public Command {
 public:
  Add() {
    operation = '+';
  }
  void execute() {
    result = a + b;
  }
};

class Sub : public Command {
 public:
  Sub() {
    operation = '-';
  }
  void execute() {
    result = a - b;
  }
};

class Mul : public Command {
 public:
  Mul() {
    operation = '*';
  }
  void execute() {
    result = a * b;
  }
};

class Div : public Command {
 public:
  Div() {
    operation = '/';
  }
  void execute() {
    result = a / b;
  }
};

class Eq : public Command {
 public:
  Eq() {
    operation = '=';
  }
  void execute() {
    result = b;
  }
};

#endif // COMMAND_H
