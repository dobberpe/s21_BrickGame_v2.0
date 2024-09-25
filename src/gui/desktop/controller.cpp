#include "controller.h"

s21::Controller *s21::Controller::get_controller(GameWindow *gw) {
    static Controller controller(gw);
    return &controller;
}

void s21::Controller::update_model() {
    GameInfo_t gameinfo = updateCurrentState();
    gamewindow->updateInterface(gameinfo);
}

s21::Controller::Controller(GameWindow *gw) : gamewindow(gw) {
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Controller::update_model);
}

void s21::Controller::process_user_input(int key_event, bool hold) { // =invalid key
    UserAction_t action;
    bool valid_key = true;

    if (key_event == Qt::Key_Enter) {
        if (updateTimer->isActive()) {
            action = Pause;
            update_model();
            updateTimer->stop();
        } else {
            action = Start;
            updateTimer->start(100);
        }
    } else if (key_event == Qt::Key_Escape) {
        action = Terminate;
    } else if (key_event == Qt::Key_Left) {
        action = Left;
    } else if (key_event == Qt::Key_Right) {
        action = Right;
    } else if (key_event == Qt::Key_Up) {
        action = Up;
    } else if (key_event == Qt::Key_Down) {
        action = Down;
    } else if (key_event == Qt::Key_Space) {
        action = Action;
    } else valid_key = false;

    if (valid_key) {
        userInput(action, false);
        if (action == Terminate) gamewindow->close();
    }
}
