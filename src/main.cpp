#include <QApplication>
#include <QMainWindow>
#include <QWidget>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.show();
    window.resize(800, 600);

    return app.exec();
}