#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

class BindingSetupWidget : public QWidget
{
    Q_OBJECT
public:
    BindingSetupWidget(QWidget *parent = nullptr);

signals:
    void bindingCreated(QString filename, int eventType, int key, int onPage, int toPage, int audioId);

private slots:
    void createBinding();

private:
    QComboBox *deviceList = nullptr;
    QLineEdit *eventType = nullptr;
    QLineEdit *key = nullptr;
    QLineEdit *onPage = nullptr;
    QLineEdit *toPage = nullptr;
    QLineEdit *audioId = nullptr;
    QPushButton *createBindingBtn = nullptr;
};