#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QTimer>

#include "../../brick_game/library_specification.h"

#define GREEN_BGR 235, 255, 147
#define GREEN_ACTIVE 146, 178, 47
#define PAUSE_ACTIVE 109, 132, 40
#define PAUSE_UNACTIVE 184, 217, 102

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

class Controller : public QObject {
  Q_OBJECT
 public:
  Controller() = delete;
  Controller(QObject *parent = nullptr) = delete;
  Controller(const Controller &) = delete;
  void operator=(const Controller &) = delete;
  static Controller *get_controller(GameWindow *gw);

 private slots:
  void update_model();

 private:
  Controller(GameWindow *gw);
  void process_user_input(int key_event, bool hold);

  friend class GameWindow;

  bool pause;
  QTimer *updateTimer;
  GameWindow *gamewindow;
};

}  // namespace s21

#endif  // GAMEWINDOW_H
