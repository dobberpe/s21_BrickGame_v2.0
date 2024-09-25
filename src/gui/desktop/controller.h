#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>

#include "gamewindow.h"
#include "../../brick_game/library_specification.h"


namespace s21 {

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

#endif // CONTROLLER_H
