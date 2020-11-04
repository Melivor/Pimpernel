#include "networkpainter.h"
#include <QPainter>
NetworkPainter::NetworkPainter()
{
    m_startValues={0,0};
    m_parameters={5,50}; //
}

void NetworkPainter::paint(QPainter *painter, double width, double height)
{
    QList<QList<QPointF>> grid;
    m_count=0;
    double x=0;
    double y=0;
    while(x<width){
        QList<QPointF> row;
        while(y<height){
            y+=m_parameters.at(0);
            row.push_back(QPointF(x,y));
        }
        y=0;
        grid.push_back(row);
        x+=m_parameters.at(0);
    }
    int i=0;
    QList<QPointF> networkPoints;
    int row=m_startValues.at(0);
    int column=m_startValues.at(1);
    grid[m_startValues.at(0)];
    networkPoints.push_back(grid[m_startValues.at(0)][m_startValues.at(1)]);
    //painter->drawLine(QLineF(grid[m_startValues.at(0)][m_startValues.at(1)]))
    while(i<m_maxIt){
            auto p=rand()%100;
            //connect right top
            int countRow=0;
            int countColumn=0;
            while(p>m_parameters.at(1)&&row>0&&row<grid.size()&&countRow<m_parameters.at(2)){
                ++countRow;
                p=rand()%100;
            }
            p=rand()%100;
            while(p>m_parameters.at(1)&& column>0 && column<grid.at(0).size()&&countColumn<m_parameters.at(2)){
                ++countColumn;
                p=rand()%100;
            }
            //connect right bottom
            //connect left top
            //connect left bottom
            //networkPoints.push_back()
    }
   // for()
}

void NetworkPainter::addNextPt(int row, int column, const QList<QList<QPointF> > &grid)
{
    if(m_count>m_maxIt){
        return;
    }
    auto p=rand()%100;
    //connect right top
    int countRow=-1;
    int countColumn=-1;
    while(p>m_parameters.at(1)&&row<grid.size()&&countRow<m_parameters.at(2)){
        ++countRow;
        p=rand()%100;
    }
    p=rand()%100;
    while(p>m_parameters.at(1) && column<grid.at(0).size()&&countColumn<m_parameters.at(2)){
        ++countColumn;
        p=rand()%100;
    }
    if(row>=0 && column>=0){
        ++m_count;
        m_connections.push_back(QLineF(grid[row][column], grid[row+countRow][column+countColumn]));
        m_networkPoints.push_back({row+countRow,column+countColumn});
    }
}
