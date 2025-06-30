#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>

#include <Audio/types.h>

class AudioSetupWidget : public QWidget
{
    Q_OBJECT
public:
    AudioSetupWidget(QWidget *parent = nullptr);


signals:
    void audioCreated(PlayerInfo info);

private slots:
    void openFile();
    void createPressed();

private:
    QLineEdit *filename = nullptr;
    QToolButton *fileBtn = nullptr;

    QLineEdit *volume = nullptr;
    QLineEdit *startTime = nullptr;
    QLineEdit *endTime = nullptr;
    QLineEdit *fadeIn = nullptr;
    QLineEdit *fadeOut = nullptr;
    QLineEdit *speed = nullptr;
    QCheckBox *looped = nullptr;

    QPushButton *createAudioBtn = nullptr;

};