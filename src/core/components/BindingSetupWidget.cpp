#include "core/components/BindingSetupWidget.h"

#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QTextLine>
#include <QToolButton>

#include <QVBoxLayout>

BindingSetupWidget::BindingSetupWidget(QWidget *parent) 
    : QWidget(parent) 
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setObjectName("bindingSetupLayout");

    QLabel *deviceLabel = new QLabel("Device", this);
    deviceLabel->setObjectName("deviceLabel");

    QPushButton *createBindingBtn = new QPushButton("Create Binding", this);
    createBindingBtn->setObjectName("createBindingBtn");

    layout->addWidget(deviceLabel);
    layout->addWidget(createBindingBtn);

    this->setLayout(layout);
}