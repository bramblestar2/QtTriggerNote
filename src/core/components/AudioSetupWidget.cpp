#include "core/components/AudioSetupWidget.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QDoubleValidator>

#include <sndfile.hh>

#include <filesystem>


AudioSetupWidget::AudioSetupWidget(QWidget *parent)
{
    QWidget *fileWidget = new QWidget(this);
    QHBoxLayout *fileLayout = new QHBoxLayout(fileWidget);
    fileWidget->setObjectName("fileWidget");
    fileLayout->setContentsMargins(0, 0, 0, 0);

    filename = new QLineEdit(this);
    filename->setObjectName("filename");
    filename->setPlaceholderText("Filename");

    fileBtn = new QToolButton(this);
    fileBtn->setObjectName("fileBtn");
    fileBtn->setToolTip("Open File");
    fileBtn->setIcon(QIcon::fromTheme("document-open"));
    connect(fileBtn, &QToolButton::clicked, this, &AudioSetupWidget::openFile);

    volume = new QLineEdit(this);
    volume->setPlaceholderText("Volume");
    volume->setValidator(new QDoubleValidator(0.0, 100.0, 2, volume));
    volume->setText("1");
    
    startTime = new QLineEdit(this);
    startTime->setPlaceholderText("Start Time");
    startTime->setValidator(new QDoubleValidator(0.0, 1.0, 1, startTime));

    endTime = new QLineEdit(this);
    endTime->setPlaceholderText("End Time");
    endTime->setValidator(new QDoubleValidator(0.0, 1.0, 1, endTime));

    fadeIn = new QLineEdit(this);
    fadeIn->setPlaceholderText("Fade In");
    fadeIn->setValidator(new QDoubleValidator(0.0, 1.0, 1, fadeIn));

    fadeOut = new QLineEdit(this);
    fadeOut->setPlaceholderText("Fade Out");
    fadeOut->setValidator(new QDoubleValidator(0.0, 1.0, 1, fadeOut));

    speed = new QLineEdit(this);
    speed->setPlaceholderText("Speed");
    speed->setValidator(new QDoubleValidator(0.0, 100.0, 2, speed));
    speed->setText("1");

    looped = new QCheckBox(this);

    createAudioBtn = new QPushButton(this);
    createAudioBtn->setText("Create Audio");
    connect(createAudioBtn, &QPushButton::clicked, this, &AudioSetupWidget::createPressed);

    fileLayout->addWidget(filename);
    fileLayout->addWidget(fileBtn);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setObjectName("formLayout");
    formLayout->addRow("File", fileWidget);
    formLayout->addRow("Volume", volume);
    formLayout->addRow("Start Time", startTime);
    formLayout->addRow("End Time", endTime);
    formLayout->addRow("Fade In", fadeIn);
    formLayout->addRow("Fade Out", fadeOut);
    formLayout->addRow("Speed", speed);
    formLayout->addRow("Loop", looped);
    formLayout->addWidget(createAudioBtn);

    setLayout(formLayout);
}

void AudioSetupWidget::openFile()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        "Open Audio File",
        QString{},
        "Audio Files (*.wav *.flac *.mp3)"
    );
    
    if (path.isEmpty())
        return;

    filename->setText(path);

    SndfileHandle handle(path.toStdString());
    if (!handle) {
        qWarning() << "Failed to open audio:" << path;
        return;
    }

    double durationSec = double(handle.frames()) / handle.samplerate();

    auto *durVal = new QDoubleValidator(0.0, durationSec, 3, startTime);
    startTime->setValidator(durVal);
    endTime->setValidator(new QDoubleValidator(0.0, durationSec, 3, endTime));
    fadeIn->setValidator(new QDoubleValidator(0.0, durationSec, 3, fadeIn));
    fadeOut->setValidator(new QDoubleValidator(0.0, durationSec, 3, fadeOut));

    startTime->setText("0.0");
    endTime->setText(QString::number(durationSec, 'f', 3));
    fadeIn->setText("0.0");
    fadeOut->setText(QString::number(durationSec, 'f', 3));
}

void AudioSetupWidget::createPressed()
{
    if (filename->text().isEmpty())
        return;

    PlayerInfo info;
    info.file = filename->text().toStdString();
    info.settings.volume = volume->text().toDouble();
    info.settings.start_time = startTime->text().toDouble();
    info.settings.end_time = endTime->text().toDouble();
    info.settings.fade_in = fadeIn->text().toDouble();
    info.settings.fade_out = fadeOut->text().toDouble();
    info.settings.speed = speed->text().toDouble();
    info.settings.looped = looped->isChecked();
    
    emit audioCreated(info);
}