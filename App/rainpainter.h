#ifndef RAINPAINTER_H
#define RAINPAINTER_H
#include "patternpainter.h"

class RainPainter : public PatternPainter
{
public:
    RainPainter();
    virtual void paint(QPainter* painter, double width, double height) override;
};

#endif // RAINPAINTER_H
