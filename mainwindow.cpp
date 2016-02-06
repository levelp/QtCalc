#include "mainwindow.h"
#include "ui_mainwindow.h"

const char NO_OPERATION = '#';

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->operation->setText(QChar(NO_OPERATION));

  setState(ENTER_NUMBER);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::clearDisplay() {
  ui->display->setText("0");
  setState(ENTER_NUMBER);
}

void MainWindow::on_digit_clicked() {
  /// Когда нажимаем на любую кнопку с цифрой
  /// вызывается этот слот:
  ///-->
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
  ///<--
}

void MainWindow::on_PointButton_clicked() {
  QString s = ui->display->text();
  int points = s.count(".");

  if(points == 0) {
    s += ".";
    ui->display->setText(s);
  }
}

/// Когда меняется текст на дисплее.
/// Изменяем состояние всех кнопок.
/// Можно нажимать ".", только если на дисплее точек ещё нет
///-->
void MainWindow::on_display_textChanged(const QString& arg1) {
  int points = ui->display->text().count(".");
  ui->PointButton->setEnabled(points == 0);
}
///<--

void MainWindow::on_operation_clicked() {
  if(calcState == OPERATION)
    return;

  /// Выполнение операции:
  /// Получаем компонент отправивший сигнал
  ///-->
  QPushButton* operButton =
    dynamic_cast<QPushButton*>
    (QObject::sender());
  ///<--

  // Следующая операция
  QString nextOp = operButton->text();

  /// Выполняем операцию над содержимым экрана и памятью
  ///-->
  Command* cmd = NULL;
  double res = 0;
  switch (ui->operation->text().at(0).toLatin1()) {
    case '+':
      cmd = new Add();
      break;
    case '-':
      cmd = new Sub();
      break;
    case '*':
      cmd = new Mul();
      break;
    case '/':
      cmd = new Div();
      break;

    case '=':
      cmd = new Eq();
      nextOp = QChar(NO_OPERATION);
      break;

    case NO_OPERATION:
      cmd = new Eq();
      break;

    default:
      throw new QString("Unknown operation: " + operButton->text());
  }

  if(cmd != NULL) {
    // Передаём данные в команду
    cmd->a = ui->memory->text().toDouble();
    cmd->b = ui->display->text().toDouble();
    // Добавляем команду в список
    cmds.append(cmd);

    // Выполняем команду
    cmd->execute();

    res = cmd->result;

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

  refreshList();
  ///<--
}

void MainWindow::setState(CalcStates state) {
  calcState = state;

  switch (calcState) {
    case OPERATION:
      ui->state->setText("Операция");
      break;

    case ENTER_NUMBER:
      ui->state->setText("Вводим число");
      break;

    default:
      ui->state->setText("Надо добавить описание :)");
      break;
  }
}

void MainWindow::on_ClearButton_clicked() {
  clearDisplay();
}

void MainWindow::on_undoButton_clicked() {
  // Получаем последнюю команду
  Command* cmd = cmds.at(cmds.size() - 1);
  // 2 + 3 + 5

  double res = cmd->a;

  QString strTemplate("%1");
  QString strRes = strTemplate.arg(res);

  // Показываем значение на экране
  ui->display->setText(strRes);
  ui->memory->setText(strRes); // + " +");

  // Удаляем последнюю
  cmds.removeAt(cmds.size() - 1);

  setState(OPERATION);

  refreshList();
}

void MainWindow::refreshList() {
  ui->listWidget->clear();
  for(int i = 0; i < cmds.size(); i++) {
    ui->listWidget->addItem(
      QString("%1. %2 %3 %4 = %5").arg(i + 1).
      arg(cmds.at(i)->a).
      arg(cmds.at(i)->operation).
      arg(cmds.at(i)->b).
      arg(cmds.at(i)->result)
    );
  }
}
