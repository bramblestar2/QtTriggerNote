#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <kddockwidgets/MainWindow.h>
#include <kddockwidgets/DockWidget.h>

#include "core/QtApp.h"

class MainWindow : public KDDockWidgets::QtWidgets::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
private:

};