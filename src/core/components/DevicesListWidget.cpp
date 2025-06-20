#include "core/components/DevicesListWidget.h"
#include <Midi/mididevice.h>

DevicesListWidget::DevicesListWidget(QWidget *parent) 
    : QListWidget(parent) 
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void DevicesListWidget::update(std::vector<MidiDevice*> devices) {
    this->clear();
    for (MidiDevice* device : devices) {
        auto item = new QListWidgetItem(this);
        item->setText(QString::fromStdString(device->name()));
        this->addItem(item);
    }
}