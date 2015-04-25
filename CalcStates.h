#ifndef CALCSTATES_H
#define CALCSTATES_H

/// Состояние калькулятора:
///-->
enum CalcStates {
  ENTER_NUMBER, // Ввели какие-то цифры и
  // продолжаем вводить число
  OPERATION, // Только что нажали "+", "-" или другую
  // бинарную операцию
};
///<--

#endif // CALCSTATES_H
