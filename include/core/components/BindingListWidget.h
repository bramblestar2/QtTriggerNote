#pragma once
#include <QWidget>
#include <QListWidget>

struct MidiBinding;

class BindingListWidget : public QListWidget {
public:
    BindingListWidget(QWidget* parent = nullptr);

    void setBindingsList(const std::map<int, std::vector<MidiBinding>>& bindings);

public slots:
    void update(const std::map<int, std::vector<MidiBinding>>& bindings);

private:
};