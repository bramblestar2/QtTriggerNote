#include "core/components/AudioListWidget.h"


PlayerEntryTableModel::PlayerEntryTableModel(std::vector<PlayerEntry> audioList, QObject *parent)
    : QAbstractTableModel(parent)
    , m_audioList(std::move(audioList))
{}

int PlayerEntryTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_audioList.size();
}

int PlayerEntryTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return 9;
}

QVariant PlayerEntryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    const auto& entry = m_audioList[index.row()];
    const auto& s = entry.info.settings;

    switch (index.column()) {
        case 0: return entry.id;
        case 1: return QString::fromStdString(entry.info.file);
        case 2: return s.volume;
        case 3: return s.start_time;
        case 4: return s.end_time;
        case 5: return s.fade_in;
        case 6: return s.fade_out;
        case 7: return s.speed;
        case 8: return s.looped;
        default: return QVariant();
    }
}

QVariant PlayerEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal) {
        static const char* headers[] = {
            "ID",
            "File",
            "Volume",
            "Start Time",
            "End Time",
            "Fade In",
            "Fade Out",
            "Speed",
            "Looped"
        };

        if (section >= 0 && section < 9) {
            return headers[section];
        }
    }
    else if (orientation == Qt::Vertical) {
        return section + 1;
    }

    return QVariant();
}





AudioListWidget::AudioListWidget(QWidget *parent)
    : QTableView(parent)
{
}

void AudioListWidget::update(std::vector<PlayerEntry> audioList)
{
    this->setModel(new PlayerEntryTableModel(audioList, this));
}