#ifndef CONFORMALDISKMODEL_H
#define CONFORMALDISKMODEL_H

#endif // CONFORMALDISKMODEL_H
#include <QPointF>
#include <math.h>
#include <QLineF>
#include <QDebug>

namespace ConformalDiskModel {

static QPointF toBeltramiCoordinate(const QPointF& pt)
{
    return QPointF(tanh(pt.x()/2), tanh(pt.y())/2);
}

static QPointF toConformalDisk(const QPointF& pt)
{
    double xb=tanh(pt.x());
    double yb=tanh(pt.y());
    double den=1+sqrt(1-pow(xb,2)-pow(yb,2));
    return QPointF(xb/den,yb/den);
}

static double invariant(double x)
{
    //return log((1+x)/(1-x));
    return 2*atanh(x);
    //return 2*std::log((x+sqrt(pow(x,2)+1))/sqrt((1-pow(x,2))*(1)));
}

static QPointF toConformalDisk2(const QPointF& pt)
{
    return QPointF(invariant(pt.x()), invariant(pt.y()));
}



namespace LinesToConformalDisk {

static void getAB(double u1, double u2, double v1, double v2, double& a, double& b)
{
    a=(u2*(pow(v1,2)+pow(v2,2))-v2*(pow(u1,2)+pow(u2,2))+u2-v2) /(u1*v2-u2*v1);
    b=(v1*(pow(u1,2)+pow(u2,2))-u1*(pow(v1,2)+pow(v2,2))+v1-u1)/(u1*v2-u2*v1);
}
static void getCircle(double a, double b, QPointF& center, double& radius)
{
    center=QPointF(-a/2,-b/2);
    radius=sqrt(pow(a,2)/4+pow(b,2)/4-1);
}
static void get(const QPointF& p1,const QPointF& p2, QPointF& start, QPointF& end, double& radius, QPointF& center)
{
    start=toBeltramiCoordinate(p1);
    end=toBeltramiCoordinate(p2);
//    start=toConformalDisk2(p1);
  //  end=toConformalDisk2(p2);
    double a;
    double b;
    getAB(start.x(),start.y(), end.x(), end.y(),a,b);
    getCircle(a,b,center,radius);
    //qDebug()<<__PRETTY_FUNCTION__<<" verification: "<<pow(start.x(),2)+pow(start.y(),2)+a*start.x()+b*start.y()<<pow(end.x(),2)+pow(end.y(),2)+a*end.x()+b*end.y();

}



}
}
