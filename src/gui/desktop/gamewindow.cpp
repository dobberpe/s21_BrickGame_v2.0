#include "gamewindow.h"
#include "controller.h"

using namespace s21;

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent) { Controller::get_controller(this)->update_model(); }

void GameWindow::updateInterface(GameInfo_t game_info) {
    update();
}

void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    for (int i = 0; i < FIELD_ROWS; ++i)
        for (int j = 0; j < FIELD_COLS; ++j) {
            QRect square(j * squareSize, i * squareSize, squareSize, squareSize);
            painter.setBrush(game_info.field[i][j] ? Qt::blue : Qt::lightGrey);
            painter.drawRect(square);
        }

    int infoFieldX = FIELD_COLS * squareSize + 50;
    int y = 50;

    painter.drawText(infoFieldX, y, QString("SCORE: %1").arg(game_info.score));
    y += 30;

    painter.drawText(infoFieldX, y, QString("HIGHSCORE: %1").arg(game_info.high_score));
    y += 30;

    painter.drawText(infoFieldX, y, QString("NEXT:").arg());

    for (int i = 0; i < NEXT_ROWS; ++i)
        for (int j = 0; j < NEXT_COLS; ++j) {
            QRect square(FIELD_COLS * squareSize + 100 + j * squareSize, y + i * squareSize, squareSize, squareSize);
            painter.setBrush(game_info.next[i][j] ? Qt::blue : Qt::lightGrey);
            painter.drawRect(square);
        }
    y += 30;

    painter.drawText(infoFieldX, y, QString("LEVEL: %1").arg(game_info.level));
    y += 30;

    painter.drawText(infoFieldX, y, QString("SPEED: %1").arg(game_info.speed));
    y += 30;
}

void GameWindow::keyPressEvent(QKeyEvent *event) { Controller::get_controller(this)->process_user_input(event->key(), true); }
