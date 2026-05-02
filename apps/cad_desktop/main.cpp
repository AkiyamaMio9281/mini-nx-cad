#include "AppController.h"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    mnx::AppController controller;
    mnx::MainWindow window(controller);
    window.show();

    return app.exec();
}
