#pragma once

#include <QMainWindow>
#include <QWidget>
#include <kddockwidgets/MainWindow.h>

class MainWindow : public KDDockWidgets::QtWidgets::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    QWidget *m_centralWidget;
};