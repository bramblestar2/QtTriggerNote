#pragma once

#include <QTabWidget>
#include <QWidget>

class AssetManagerWidget : public QWidget
{
    Q_OBJECT
public:
    AssetManagerWidget(QWidget *parent = nullptr);

private:
    QTabWidget *m_tabWidget = nullptr;
    
};