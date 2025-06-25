#pragma once

#include <QWidget>

class BindingSetupWidget : public QWidget
{
    Q_OBJECT
public:
    BindingSetupWidget(QWidget *parent = nullptr);

private slots:
    void openFile();
};