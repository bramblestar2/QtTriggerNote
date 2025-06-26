#include "core/components/BindingSetupWidget.h"

#include <QSpinBox>
#include <QLabel>

#include <QSpacerItem>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

/*

To create a binding, I would need to provide the following: 

Required:
    - Device Name (String)

Optional:
    - Event Type (int)
    - Key (int)
    - On Page (int)
    - To Page (int)
    - Audio ID (int)

*/

BindingSetupWidget::BindingSetupWidget(QWidget *parent) 
    : QWidget(parent) 
{
    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->setObjectName("formLayout");
    
    QWidget *fileEditWidget = new QWidget(this);
    QHBoxLayout *fileEditLayout = new QHBoxLayout(fileEditWidget);
    fileEditWidget->setObjectName("fileWidget");
    fileEditWidget->setLayout(fileEditLayout);

    deviceList = new QComboBox(this);
    deviceList->setObjectName("fileLine");

    eventType = new QLineEdit(this);
    eventType->setObjectName("eventType");
    eventType->setPlaceholderText("Event Type");
    eventType->setValidator(new QIntValidator(0, 0x7F, this));
    eventType->setText("0");

    key = new QLineEdit(this);
    key->setObjectName("key");
    key->setPlaceholderText("Key");
    key->setValidator(new QIntValidator(0, 0x7f, this));
    key->setText("0");

    onPage = new QLineEdit(this);
    onPage->setObjectName("onPage");
    onPage->setPlaceholderText("On Page");
    onPage->setValidator(new QIntValidator(0, 0x7f, this));
    onPage->setText("0");

    toPage = new QLineEdit(this);
    toPage->setObjectName("toPage");
    toPage->setPlaceholderText("To Page");
    toPage->setValidator(new QIntValidator(0, 0x7f, this));
    toPage->setText("0");

    audioId = new QLineEdit(this);
    audioId->setObjectName("audioId");
    audioId->setPlaceholderText("Audio ID");
    audioId->setValidator(new QIntValidator(this));
    audioId->setText("-1");

    createBindingBtn = new QPushButton("Create Binding", this);
    createBindingBtn->setObjectName("createBindingBtn");

    connect(createBindingBtn, &QPushButton::pressed, this, &BindingSetupWidget::createBinding);

    fileEditLayout->addWidget(deviceList);

    this->setLayout(formLayout);
    formLayout->addRow("Device", fileEditWidget);
    formLayout->addRow("Event Type", eventType);
    formLayout->addRow("Key", key);
    formLayout->addRow("On Page", onPage);
    formLayout->addRow("To Page", toPage);
    formLayout->addRow("Audio ID", audioId);
    formLayout->addWidget(createBindingBtn);




    // QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->setObjectName("bindingSetupLayout");

    // QLabel *deviceLabel = new QLabel("Device", this);
    // deviceLabel->setObjectName("deviceLabel");

    

    // fileEditLayout->addWidget(fileLine);
    // fileEditLayout->addWidget(openFileBtn);

    // layout->addStretch();
    // layout->addWidget(deviceLabel);
    // layout->addWidget(fileEditWidget);
    // layout->addWidget(createBindingBtn);
    // layout->addStretch();

    // this->setLayout(layout);
}

void BindingSetupWidget::createBinding() {
    emit bindingCreated(deviceList->currentText(), 
                       eventType->text().toInt(), 
                       key->text().toInt(), 
                       onPage->text().toInt(), 
                       toPage->text().toInt(), 
                       audioId->text().toInt()
                    );

    qDebug() << "Creating Binding with Device: " << deviceList->currentText();
    qDebug() << "Creating Binding with Event Type: " << eventType->text().toInt();
    qDebug() << "Creating Binding with Key: " << key->text().toInt();
    qDebug() << "Creating Binding with On Page: " << onPage->text().toInt();
    qDebug() << "Creating Binding with To Page: " << toPage->text().toInt();
    qDebug() << "Creating Binding with Audio ID: " << audioId->text().toInt();
}