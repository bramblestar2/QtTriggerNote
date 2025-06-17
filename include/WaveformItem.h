#pragma once
#include <QGraphicsItem>
#include <QVector>
#include <QRect>

class WaveformItem : public QGraphicsItem {
public:
    WaveformItem(const QRectF &r = QRectF(0,0,800,200), QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*) override;

    void setLevels(const QVector<float> &newLevels);

private:
    QRectF m_rect;
    QVector<float> m_levels;
};