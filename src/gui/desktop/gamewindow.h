#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>

#include "controller.h"

namespace s21 {

  class GameWindow : public QMainWindow {
      Q_OBJECT

  public:
      GameWindow(QWidget *parent = nullptr);

  protected:
      void paintEvent(QPaintEvent *event) override;

  private:
      void keyPressEvent(QKeyEvent *event) override;
      void updateInterface(GameInfo_t game_info);

      friend class Controller;

      static const squareSize = 30;
  };
}

#endif // GAMEWINDOW_H
