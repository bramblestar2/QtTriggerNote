#pragma once

#include <QListWidget>

class MidiDevice;

class DevicesListWidget : public QListWidget {
    Q_OBJECT
public:
    DevicesListWidget(QWidget *parent = nullptr);

public slots:
    void update(std::vector<MidiDevice*> devices);
};