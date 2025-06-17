#include "WaveformItem.h"
#include <QPainter>
void WaveformItem::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
{
    p->setPen(Qt::green);
    int w = rect.width(), h = rect.height();
    int n = levels.size();
    for (int i = 0; i < w && i < n; ++i) {
        float v = levels[n * i / w];
        float y = rect.center().y() * (1 - v);
        p->drawLine(rect.x() + i, rect.center().y(), rect.x() + i, y);
    }
}
