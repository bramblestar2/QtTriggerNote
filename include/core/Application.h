#pragma once
#include <QObject>
#include "windows/ProjectWindow.h"
#include "core/QtApp.h"

class Application : public QObject {
    Q_OBJECT
public:
    Application();

private:
    QtApp m_app;
    ProjectWindow m_projectWindow;
};