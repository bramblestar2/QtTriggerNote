#include "core/components/BindingListWidget.h"
#include <App/types.h>
#include <map>
#include <QListWidgetItem>
#include <QVBoxLayout>

BindingListWidget::BindingListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

void BindingListWidget::setBindingsList(const std::map<int, std::vector<MidiBinding>> &bindings)
{
    this->clear();
    for (auto& [page, bindings] : bindings) {
        for (auto& binding : bindings) {
            auto item = new QListWidgetItem(this);
            item->setText(QString::number(binding.id) + " " + QString::fromStdString(binding.deviceName) + " " + QString::number((int)binding.eventType) + " " + QString::number(binding.key));
            this->addItem(item);
        }
    }
}

void BindingListWidget::update(const std::map<int, std::vector<MidiBinding>> &bindings) {
    setBindingsList(bindings);
}