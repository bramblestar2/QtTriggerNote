#include "core/components/BindingListWidget.h"
#include <App/types.h>
#include <map>

BindingListWidget::BindingListWidget(QWidget *parent)
    : QWidget(parent)
{
}

void BindingListWidget::setBindingsList(const std::map<int, std::vector<MidiBinding>> &bindings)
{
    for (auto& [page, bindings] : bindings) {
        for (auto& binding : bindings) {
            qDebug() << "Binding: " << binding.id << " " << binding.deviceName << " " << binding.eventType << " " << binding.key;
        }
    }
}

void BindingListWidget::update(const std::map<int, std::vector<MidiBinding>> &bindings) {
    setBindingsList(bindings);
}