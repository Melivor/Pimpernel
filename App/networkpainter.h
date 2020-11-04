#ifndef NETWORKPAINTER_H
#define NETWORKPAINTER_H
#include "patternpainter.h"

class NetworkPainter : PatternPainter
{
public:
    NetworkPainter();
    virtual void paint(QPainter* painter, double width, double height) override;
    void addNextPt(int row, int column, const QList<QList<QPointF>> &grid);
private:
    int m_count=0;
    QList<std::pair<int, int>> m_networkPoints;
    QList<QLineF> m_connections;
};

#endif // NETWORKPAINTER_H
