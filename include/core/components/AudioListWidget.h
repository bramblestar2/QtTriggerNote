#pragma once

#include <QWidget>
#include <QTableView>
#include <QVariantList>
#include <QAbstractTableModel>

#include "Audio/types.h"

class PlayerEntryTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    PlayerEntryTableModel(std::vector<PlayerEntry> audioList, QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    std::vector<PlayerEntry> m_audioList;
};

class AudioListWidget : public QTableView {
    Q_OBJECT
public:
    AudioListWidget(QWidget* parent = nullptr);

public slots:
    void update(std::vector<PlayerEntry> audioList);
};