#include "Juego.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow mainWindow;
  mainWindow.setWindowTitle("myGatekeeper");
  mainWindow.setFixedSize(1024, 768); // Resolucion
  Juego juego(&mainWindow);
  mainWindow.show();
  return app.exec();
}
