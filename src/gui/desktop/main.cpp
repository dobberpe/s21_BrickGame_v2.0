#include <QApplication>

#include "gamewindow.h"

using namespace s21;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GameWindow w;
  w.show();
  return a.exec();
}
