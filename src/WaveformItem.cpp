#include "WaveformItem.h"
#include <QPainter>
WaveformItem::WaveformItem(const QRectF &r, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_rect(r)
{
}

QRectF WaveformItem::boundingRect() const
{
    return m_rect;
}

void WaveformItem::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_levels.isEmpty())
        return;

    p->setPen(Qt::green);
    int w = int(m_rect.width());
    int h2 = int(m_rect.height() / 2);
    int N = m_levels.size();

    for (int x = 0; x < w; ++x) {
        float level = m_levels[int((float)x / w * N)];
        int y = int(level * h2);
        p->drawLine(QPointF(m_rect.x() + x, m_rect.y() + h2 - y),
                    QPointF(m_rect.x() + x, m_rect.y() + h2 + y));
    }
}

void WaveformItem::setLevels(const QVector<float> &newLevels)
{
    m_levels = newLevels;
    update(boundingRect());
}
