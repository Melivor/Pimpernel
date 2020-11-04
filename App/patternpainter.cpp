#include "patternpainter.h"
#include <QPainter>
#include <QImageWriter>

PatternPainter::PatternPainter(QQuickItem *parent):QQuickPaintedItem(parent)
{
    m_backgroundColor=QColor::fromHsl(0,0,80,255);
    m_strokeColor=QColor::fromHsl(0,0,255,255);
}

void PatternPainter::paint(QPainter *painter)
{

    paint(painter, width(), height());
}

void PatternPainter::saveAsPng(QUrl url)
{

    QImage image(1920, 1080, QImage::Format_RGB32);
    image.fill(QColor("white"));
    QPainter painter;
    painter.begin(&image);
    paint(&painter, image.width(), image.height());
    painter.end();
    QImageWriter writer(url.toLocalFile());
    if(!writer.write(image)){
        qWarning()<<writer.errorString();
    }
}

void PatternPainter::connectParameters()
{
    for(auto&& parameter:m_variantParameters){
        connect(parameter,&Parameter::valueChanged, this, &PatternPainter::reDraw);
    }
}
