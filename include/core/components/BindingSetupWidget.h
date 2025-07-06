#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include <App/MidiBindingBuilder.h>

class BindingSetupWidget : public QWidget
{
    Q_OBJECT
public:
    BindingSetupWidget(QWidget *parent = nullptr);

signals:
    void bindingCreated(MidiBinding binding);

public slots:
    void update(std::vector<class MidiDevice*> devices);

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