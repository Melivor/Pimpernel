#include "parametricequationanimator.h"
#include "parametricequationshapegenerator.h"
#include <QTimer>
ParametricEquationAnimator::ParametricEquationAnimator(ParametricEquationsShapeGeneratorSettings *pESGenerator, QObject *parent) : StandardItemModel("ParametricEquationAnimator", "", parent), m_pESGenerator(pESGenerator)
{


    setParameters();
}

void ParametricEquationAnimator::setParameters()
{
    if(!m_pESGenerator){
        return;
    }
    beginResetModel();
    clear();
    auto pName=QObject::tr("Start t");
    auto p=new StandardItem(pName, 0,"",0,tr(""));
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MinRole), StandardItemModel::MinRole);
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MaxRole), StandardItemModel::MaxRole);
    p->setData(0, FromRole);
    p->setData(0, ToRole);
    appendRow(p);

    pName=QObject::tr("End t");
    p=new StandardItem(pName, 0,"",0,tr(""));
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MinRole), StandardItemModel::MinRole);
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MaxRole), StandardItemModel::MaxRole);
    p->setData(0, FromRole);
    p->setData(0, ToRole);
    appendRow(p);

    pName=QObject::tr("Scale");
    p=new StandardItem(pName, 0,"",0,tr(""));
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MinRole), StandardItemModel::MinRole);
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MaxRole), StandardItemModel::MaxRole);
    p->setData(0, FromRole);
    p->setData(0, ToRole);
    appendRow(p);

    pName=QObject::tr("Rotation number");
    p=new StandardItem(pName, 0,"",0,tr(""));
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MinRole), StandardItemModel::MinRole);
    p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MaxRole), StandardItemModel::MaxRole);
    p->setData(0, FromRole);
    p->setData(0, ToRole);
    appendRow(p);


    int i=1;
    QVariant pValue=m_pESGenerator->getActiveData("p"+QString::number(i));
    while(pValue!=QVariant()){
        auto pName="p"+QString::number(i);
        auto pFlags=m_pESGenerator->getActiveData(pName, StandardItemModel::FlagRole);
        if(pFlags.toInt()&Qt::ItemIsEnabled){
            auto p=new StandardItem(pName, 0,"",0,tr(""));
            p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MinRole), StandardItemModel::MinRole);
            p->setData(m_pESGenerator->getActiveData(pName, StandardItemModel::MaxRole), StandardItemModel::MaxRole);
            p->setData(0, FromRole);
            p->setData(10, ToRole);
            appendRow(p);
        }
        ++i;
        pValue=m_pESGenerator->getActiveData("p"+QString::number(i));
    }
    endResetModel();
}


void ParametricEquationAnimator::stopAnimation()
{
    disconnect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameter);
    disconnect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameterReverse);
    m_timer->stop();
    m_timer.reset();
    emit isRunningChanged();
}
void ParametricEquationAnimator::startAnimation(bool loop)
{
        m_repeat=loop;
        if(m_timer!=nullptr){
            stopAnimation();
        }
        m_timer.reset(new QTimer());
        m_timer->setInterval(40);

        initAnimation();
        connect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameter);
        m_timer->start();
        emit isRunningChanged();
}

void ParametricEquationAnimator::initAnimation()
{

    for(int row=0; row<rowCount(); ++row){
        auto ind=index(row,0);
        setData(ind, data(ind,StandardItemModel::MinRole));
        double timeInterval=((data(ind,ToRole).toInt()-data(ind,FromRole).toInt())*10);
        if(timeInterval!=0){

            double interval=((data(ind,StandardItemModel::MaxRole).toDouble()-data(ind,StandardItemModel::MinRole).toDouble()))/((data(ind,ToRole).toInt()-data(ind,FromRole).toInt())*10);
            if(interval!=0){
                m_pESGenerator->setActiveData(data(ind, StandardItemModel::NameRole).toString(), data(ind,StandardItemModel::MinRole));
            }
            setData(ind, interval, IntervalRole);

        }
        else{
            setData(ind, 0, IntervalRole);
        }


    }
    countInterval=0;
}

void ParametricEquationAnimator::updateParameter()
{
    bool isSomeoneStillChanging=false;

    for(int row=0; row<rowCount(); ++row){

        auto ind=index(row,0);
        if(countInterval<data(ind, FromRole).toInt()*10){
            isSomeoneStillChanging=true;
            continue;
        }
        if(countInterval>data(ind, ToRole).toInt()*10){
            continue;
        }
        double oldValue=data(ind,Qt::DisplayRole).toDouble();
        double newValue=oldValue+data(ind,IntervalRole).toDouble();

        if(oldValue!=newValue){
            setData(ind, newValue);

            m_pESGenerator->setActiveData(data(ind, StandardItemModel::NameRole).toString(), QVariant(newValue));
            isSomeoneStillChanging=true;

        }

    }
    ++countInterval;
    if(!isSomeoneStillChanging){
        if(m_repeat){
            disconnect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameter);
            connect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameterReverse);
            updateParameterReverse();
        }
        else{
            stopAnimation();
        }


    }
}

void ParametricEquationAnimator::updateParameterReverse()
{
    bool isSomeoneStillChanging=false;

    for(int row=0; row<rowCount(); ++row){

        auto ind=index(row,0);
        if(countInterval<data(ind, FromRole).toInt()*10){
            continue;
        }
        if(countInterval>data(ind, ToRole).toInt()*10){
            isSomeoneStillChanging=true;
            continue;
        }
        double oldValue=data(ind,Qt::DisplayRole).toDouble();
        double newValue=oldValue-data(ind,IntervalRole).toDouble();

        if(oldValue!=newValue){
            setData(ind, newValue);

            m_pESGenerator->setActiveData(data(ind, StandardItemModel::NameRole).toString(), QVariant(newValue));
            isSomeoneStillChanging=true;

        }

    }
    --countInterval;
    if(!isSomeoneStillChanging){
        if(m_repeat){
            connect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameter);
            disconnect(m_timer.get(), &QTimer::timeout, this, &ParametricEquationAnimator::updateParameterReverse);
        }
        else{
             stopAnimation();
        }


    }
}

QHash<int, QByteArray> ParametricEquationAnimator::roleNames() const
{
    QHash<int, QByteArray> roleNames = StandardItemModel::roleNames();
    roleNames.insert(FromRole, "from");
    roleNames.insert(ToRole, "to");


    return roleNames;
}
