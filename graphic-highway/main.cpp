#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "pthread.h"
#include "unistd.h"
#include "semaphore.h"
#include <QApplication>
#include "threads_controller.h"

int main(int argc, char **argv)
{

  QApplication app(argc, argv);

  HighwayGui& gui = HighwayGui::getInstance();
  gui.show();
  gui.displayMainMenu();

  return app.exec();
}
