#include "gamewindow.h"

#include <QDebug>

using namespace s21;

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) {
  setFixedSize(540, 600);
  setStyleSheet("background-color: rgb(235, 255, 147);");
  Controller::get_controller(this)->update_model();
}

void GameWindow::paintEvent(QPaintEvent *event [[maybe_unused]]) {
  QPainter painter(this);

  set_painter(painter);
  print_field(painter, 0, game_info.field, false);

  int infoFieldX = FIELD_COLS * squareSize + 15;

  print_info(painter, infoFieldX, 12, 195, QString("SCORE"), true);
  print_info(painter, infoFieldX, 12, 195, QString("%1").arg(game_info.score),
             false);

  print_info(painter, infoFieldX, 42, 195, QString("HIGHSCORE"), true);
  print_info(painter, infoFieldX, 42, 195,
             QString("%1").arg(game_info.high_score), false);

  print_info(painter, infoFieldX, 102, 195, QString("NEXT"), true);
  print_field(painter, 102, game_info.next, true);

  print_info(painter, infoFieldX, 162, 195, QString("LEVEL"), true);
  print_info(painter, infoFieldX, 162, 195, QString("%1").arg(game_info.level),
             false);

  print_info(painter, infoFieldX, 192, 195, QString("SPEED"), true);
  print_info(painter, infoFieldX, 192, 195, QString("%1").arg(game_info.speed),
             false);

  print_info(painter, infoFieldX, 222, 195, QString("SPEED UP"), true);
  print_info(painter, infoFieldX, 222, 195,
             QString("%1%").arg(game_info.speed < 10
                                    ? ((game_info.score % 5) * 100) / 5
                                    : 100),
             false);

  print_info(painter, infoFieldX, 282, 195, QString("CONTROL:"), true);

  infoFieldX += 15;
  print_info(painter, infoFieldX, 312, 53, QString("ENTER"), true);
  print_info(painter, infoFieldX + 53, 312, 150, QString("- START / PAUSE"),
             true);

  print_info(painter, infoFieldX, 342, 53, QString("←"), true);
  print_info(painter, infoFieldX + 53, 342, 150, QString("- LEFT"), true);

  print_info(painter, infoFieldX, 372, 53, QString("→"), true);
  print_info(painter, infoFieldX + 53, 372, 150, QString("-RIGHT"), true);

  print_info(painter, infoFieldX, 402, 53, QString("↓"), true);
  print_info(painter, infoFieldX + 53, 402, 150, QString("- MOVE DOWN"), true);

  print_info(painter, infoFieldX, 432, 53, QString("↑"), true);
  print_info(painter, infoFieldX + 53, 432, 150, QString("- PUT DOWN / UP"),
             true);

  print_info(painter, infoFieldX, 462, 53, QString("SPACE"), true);
  print_info(painter, infoFieldX + 53, 462, 150, QString("- ROTATE / BOOST"),
             true);

  print_info(painter, infoFieldX, 492, 53, QString("ESC"), true);
  print_info(painter, infoFieldX + 53, 492, 150, QString("- QUIT"), true);

  QFont font = painter.font();
  font.setPointSize(36);
  painter.setFont(font);

  painter.setPen(game_info.pause ? QColor(PAUSE_ACTIVE)
                                 : QColor(PAUSE_UNACTIVE));
  painter.drawText(infoFieldX + 15, 572, QString("PAUSE"));

  painter.setPen(QColor(PAUSE_ACTIVE));
}

void GameWindow::closeEvent(QCloseEvent *event [[maybe_unused]]) {
  Controller::get_controller(this)->process_user_input(Qt::Key_Escape, false);
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
  Controller::get_controller(this)->process_user_input(event->key(),
                                                       event->isAutoRepeat());
}

void GameWindow::set_gameinfo(GameInfo_t gi) {
  game_info.field = gi.field;
  game_info.next = gi.next;
  game_info.score = gi.score;
  game_info.high_score = gi.high_score;
  game_info.level = gi.level;
  game_info.speed = gi.speed;
  game_info.pause = gi.pause;
}

void GameWindow::set_painter(QPainter &painter) const {
  QFont font = painter.font();
  font.setPointSize(12);
  painter.setFont(font);
  painter.setPen(QColor(PAUSE_ACTIVE));
}

void GameWindow::print_info(QPainter &painter, const int &x, const int &y,
                            const int &width, const QString &label,
                            bool left_align) const {
  QRect rect(x, y, width, squareSize);
  painter.drawText(rect, left_align ? Qt::AlignLeft : Qt::AlignRight, label);
}

void GameWindow::print_field(QPainter &painter, const int &y, int **field,
                             bool next) const {
  for (int i = 0; i < (next ? NEXT_ROWS : FIELD_ROWS); ++i)
    for (int j = 0; j < (next ? NEXT_COLS : FIELD_COLS); ++j) {
      QRect square(next ? (FIELD_COLS * squareSize + 90 + j * squareSize)
                        : (j * squareSize),
                   next ? (y - 12 + i * squareSize) : (i * squareSize),
                   squareSize, squareSize);
      painter.setBrush(field[i][j] ? QColor(GREEN_ACTIVE) : QColor(GREEN_BGR));
      painter.drawRect(square);
    }
}

s21::Controller *s21::Controller::get_controller(GameWindow *gw) {
  static Controller controller(gw);
  return &controller;
}

void s21::Controller::update_model() {
  GameInfo_t gameinfo = updateCurrentState();
  gamewindow->set_gameinfo(gameinfo);
  gamewindow->update();
}

s21::Controller::Controller(GameWindow *gw) : gamewindow(gw) {
  updateTimer = new QTimer(this);
  connect(updateTimer, &QTimer::timeout, this, &Controller::update_model);
}

Controller::~Controller() { delete updateTimer; }

void s21::Controller::process_user_input(
    int key_event, bool hold) {  // update model after userInput

  switch (key_event) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
      userInput(updateTimer->isActive() ? Pause : Start, hold);
      if (updateTimer->isActive()) {
        update_model();
        updateTimer->stop();
      } else
        updateTimer->start(1);
      break;
    case Qt::Key_Escape:
      userInput(Terminate, hold);
      gamewindow->close();
      break;
    case Qt::Key_Left:
      userInput(Left, hold);
      break;
    case Qt::Key_Right:
      userInput(Right, hold);
      break;
    case Qt::Key_Up:
      userInput(Up, hold);
      break;
    case Qt::Key_Down:
      userInput(Down, hold);
      break;
    case Qt::Key_Space:
      userInput(Action, hold);
      break;
  }
}
