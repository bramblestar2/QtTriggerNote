#pragma once

#include <QMainWindow>
#include <QGraphicsView>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:

    QGraphicsView *view = nullptr;
};