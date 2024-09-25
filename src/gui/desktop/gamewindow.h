#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QObject>
#include <QTimer>

#include "../../brick_game/library_specification.h"

namespace s21 {

  class GameWindow : public QMainWindow {
      Q_OBJECT

  public:
      GameWindow(QWidget *parent = nullptr);

  protected:
      void paintEvent(QPaintEvent *event);

  private:
      void keyPressEvent(QKeyEvent *event) override;
      void set_gameinfo(GameInfo_t gi);

      friend class Controller;

      GameInfo_t game_info;
      static const int squareSize = 30;
  };

  class Controller : public QObject
  {
      Q_OBJECT
  public:
      Controller() = delete;
      Controller(QObject *parent = nullptr) = delete;
      Controller(const Controller&) = delete;
      void operator =(const Controller&) = delete;
      static Controller* get_controller(GameWindow *gw);

  private slots:
      void update_model();

  private:
      Controller(GameWindow *gw);
      void process_user_input(int key_event, bool hold);

      friend class GameWindow;

      bool pause;
      QTimer *updateTimer;
      GameWindow* gamewindow;
  };

}

#endif // GAMEWINDOW_H
