#pragma once
#include <QObject>
#include "windows/ProjectWindow.h"

class Application : public QObject {
    Q_OBJECT
public:
    Application();

private:
    ProjectWindow m_projectWindow;
};