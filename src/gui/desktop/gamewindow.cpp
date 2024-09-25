#include "gamewindow.h"

#include <QDebug>

using namespace s21;

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) {
  setFixedSize(540, 600);
  setStyleSheet("background-color: rgb(235, 255, 147);");
  Controller::get_controller(this)->update_model();
}

void GameWindow::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  QFont font = painter.font();
  font.setPointSize(12);
  painter.setFont(font);
  painter.setPen(QColor(PAUSE_ACTIVE));

  for (int i = 0; i < FIELD_ROWS; ++i)
    for (int j = 0; j < FIELD_COLS; ++j) {
      QRect square(j * squareSize, i * squareSize, squareSize, squareSize);
      painter.setBrush(game_info.field[i][j] ? QColor(GREEN_ACTIVE)
                                             : QColor(GREEN_BGR));
      painter.drawRect(square);
    }

  int infoFieldX = FIELD_COLS * squareSize + 15;
  int y = 30;

  painter.drawText(infoFieldX, y, QString("SCORE"));
  painter.drawText(infoFieldX + 125, y,
                   QString("%1").arg(game_info.score, 11, 10, QChar(' ')));
  y += 30;

  painter.drawText(infoFieldX, y, QString("HIGHSCORE"));
  painter.drawText(infoFieldX + 125, y,
                   QString("%1").arg(game_info.high_score, 11, 10, QChar(' ')));
  y += 60;

  painter.drawText(infoFieldX, y, QString("NEXT"));
  y -= 30;
  for (int i = 0; i < NEXT_ROWS; ++i)
    for (int j = 0; j < NEXT_COLS; ++j) {
      QRect square(FIELD_COLS * squareSize + 90 + j * squareSize,
                   y + i * squareSize, squareSize, squareSize);
      painter.setBrush(game_info.next[i][j] ? QColor(GREEN_ACTIVE)
                                            : QColor(GREEN_BGR));
      painter.drawRect(square);
    }
  y += 90;

  painter.drawText(infoFieldX, y, QString("LEVEL"));
  painter.drawText(infoFieldX + 125, y,
                   QString("%1").arg(game_info.level, 11, 10, QChar(' ')));
  y += 30;

  painter.drawText(infoFieldX, y, QString("SPEED"));
  painter.drawText(infoFieldX + 125, y,
                   QString("%1").arg(game_info.speed, 11, 10, QChar(' ')));
  y += 30;

  painter.drawText(infoFieldX, y, QString("SPEED UP"));
  painter.drawText(
      infoFieldX + 125, y,
      QString("%1").arg(
          (game_info.speed < 10 ? ((game_info.score % 600) * 100) / 600 : 100),
          11, 10, QChar(' ')));
  y += 30;

  painter.drawText(infoFieldX, y, QString("CONTROL:"));
  y += 30;

  infoFieldX += 15;
  painter.drawText(infoFieldX, y, QString("ENTER"));
  painter.drawText(infoFieldX + 53, y, QString("- START / PAUSE"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("←"));
  painter.drawText(infoFieldX + 53, y, QString("- LEFT"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("→"));
  painter.drawText(infoFieldX + 53, y, QString("- RIGHT"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("↓"));
  painter.drawText(infoFieldX + 53, y, QString("- MOVE DOWN"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("↑"));
  painter.drawText(infoFieldX + 53, y, QString("- PUT DOWN / UP"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("SPACE "));
  painter.drawText(infoFieldX + 53, y, QString("- ROTATE / BOOST"));
  y += 30;

  painter.drawText(infoFieldX, y, QString("ESC"));
  painter.drawText(infoFieldX + 53, y, QString("- QUIT"));
  y += 80;

  font.setPointSize(36);
  painter.setFont(font);

  painter.setPen(game_info.pause ? QColor(PAUSE_ACTIVE)
                                 : QColor(PAUSE_UNACTIVE));
  painter.drawText(infoFieldX + 15, y, QString("PAUSE"));

  painter.setPen(QColor(Qt::black));
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
        updateTimer->start(100);
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
