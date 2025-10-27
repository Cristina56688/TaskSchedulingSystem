#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Inițializează framework-ul Qt
    MainWindow window;            // Creează fereastra principală
    window.show();                // O afișează pe ecran
    return app.exec();            // Pornește bucla principală de evenimente
}
