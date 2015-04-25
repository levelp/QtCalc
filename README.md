<!-- doc.py -->
Калькулятор на Qt5
------------------

Калькулятор аналогичный калькулятору Windows.
* Цифры: 0,1,2...9
* Арифметические операции: + - * /



Состояние калькулятора:
``` cpp
enum CalcStates {
  ENTER_NUMBER, // Ввели какие-то цифры и
  // продолжаем вводить число
  OPERATION, // Только что нажали "+", "-" или другую
  // бинарную операцию
};
```

[CalcStates.h](CalcStates.h)

Когда нажимаем на любую кнопку с цифрой
вызывается этот слот:
``` cpp
  switch (calcState) {
    case OPERATION: // Если сейчас операция
      clearDisplay(); // Очищаем дисплей
      break;

    default:
      break;
  }

  // UTF-8
  QString s = ui->display->text();

  // (QPushButton *)QObject::sender() -
  //  отправитель текущего сигнала
  QPushButton* digitButton =
    (QPushButton*)QObject::sender();

  if(digitButton == NULL)
    return;

  QChar zero('0');

  // Удаляем из s все лидирующие нули
  if(s.at(0) == zero)
    s.remove(0, 1);

  // Дописываем новую цифру на экран
  s += digitButton->text();

  ui->display->setText(s);
```

Когда меняется текст на дисплее.
Изменяем состояние всех кнопок.
Можно нажимать ".", только если на дисплее точек ещё нет
``` cpp
void MainWindow::on_display_textChanged(const QString& arg1) {
  int points = ui->display->text().count(".");
  ui->PointButton->setEnabled(points == 0);
}
```

Выполнение операции:
Получаем компонент отправивший сигнал
``` cpp
  QPushButton* operButton =
    dynamic_cast<QPushButton*>
    (QObject::sender());
```

Выполняем операцию над содержимым экрана и памятью
``` cpp
  double a = ui->memory->text().toDouble();
  double b = ui->display->text().toDouble();
  double res = 0;
  QString nextOp = operButton->text();

  switch (ui->operation->text().at(0).toLatin1()) {
    case '+':
      res = a + b;
      break;

    case '-':
      res = a - b;
      break;

    case '*':
      res = a * b;
      break;

    case '/':
      res = a / b;
      break;

    case '=':
      res = b;
      nextOp = QChar(NO_OPERATION);
      break;

    case NO_OPERATION:
      res = b;
      break;

    default:
      throw new QString("Unknown operation: " + operButton->text());
  }

  // Запоминаем нажатую кнопку - следующую операцию
  if(nextOp == QStringLiteral("="))
    ui->operation->setText(QChar(NO_OPERATION));
  else
    ui->operation->setText(nextOp);

  QString strTemplate("%1");
  QString strRes = strTemplate.arg(res);

  // Показываем значение на экране
  ui->display->setText(strRes);
  ui->memory->setText(strRes); // + " +");

  setState(OPERATION);
```

[mainwindow.cpp](mainwindow.cpp)

