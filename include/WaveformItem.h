#pragma once
#include <QGraphicsItem>
#include <QVector>
#include <QRect>

class WaveformItem : public QGraphicsItem {
public:
    QVector<float> levels;
    QRectF boundingRect() const override { return rect; }

    void paint(QPainter *p, const QStyleOptionGraphicsItem*, QWidget*) override;

    QRectF rect;
};