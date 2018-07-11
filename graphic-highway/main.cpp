#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "pthread.h"
#include "unistd.h"
#include "semaphore.h"
#include <QApplication>
//#include "Graphics/highway_gui.h"
#include "threads_controller.h"

int main(int argc, char **argv)
{

//DA METTERE NEL THREAD DELLA GUI?????!!??!?!??!?

  QApplication app(argc, argv);

  HighwayGui& gui = HighwayGui::getInstance();
  gui.show();
  gui.displayMainMenu();

	//Avvio il thread della gui, da cui partiranno gli altri thread
	//quando l'utente premerà su start
	//initGuiThread();

	//return 0;
  return app.exec();
}
