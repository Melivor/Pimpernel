
#include "parametricequation.h"
#include <QString>
#include <QDebug>
#include "standarditemmodel.h"

double ParametricEquation::rnd(double x)
{
    return std::round(x);
}
ParametricEquation::ParametricEquation()
{
    QString functionName="round";
    m_parser.DefineFun(functionName.toStdWString(), &ParametricEquation::rnd, false);
    setExpression("3*t");
}

ParametricEquationItem::ParametricEquationItem(ParametricEquation* equation, const QString& name, const QVariant& value, const QString& qmlFile, int complexity, const QString &description) : StandardItem(name,value, qmlFile, complexity,description), m_equation(equation)
{
    ParametricEquationItem::setData(false, StandardItemModel::ErrorRole);
    ParametricEquationItem::setData("", StandardItemModel::ErrorStringRole);
    if(!m_equation){
        qWarning()<<"equation* must be valid";
    }
}
void ParametricEquationItem::setData(const QVariant &value, int role)
{

    if(role==Qt::DisplayRole){

        QString expression=value.toString();


        if(m_equation && !m_equation->setExpression(expression)){
            setData(true, StandardItemModel::ErrorRole);
            setData(m_equation->errorString(), StandardItemModel::ErrorStringRole);
        }
        else{
            updateParametersUsed(expression);
            setData(false, StandardItemModel::ErrorRole);
        }
    }
    StandardItem::setData(value, role);
}

void ParametricEquationItem::updateParametersUsed(const QString& expression)
{
    for(int i=0; i<m_parameterItems.size();++i){
        if(expression.contains("p"+QString::number(i+1))){
            if(!m_isParametersUsed.at(i)){
                //m_parameterItems.at(i)->setEnabled(true);
                m_parameterItems[i]->giveToken();
                m_isParametersUsed[i]=true;
            }


        }
        else{
            if(m_isParametersUsed.at(i)){
                m_parameterItems[i]->takeToken();
                m_isParametersUsed[i]=false;
            }
            //m_parameterItems.at(i)->setEnabled(false);
        }
    }
}
void ParametricEquationItem::setEquation(ParametricEquation *equation)
{
    //qDebug()<<__PRETTY_FUNCTION__;
    m_equation=equation;
    for(int i=0; i<m_parameterItems.size();++i){
        m_equation->addParameter();
        // m_parameterItems.at(i)->setEnabled(false);
        m_parameterItems.at(i)->addEquation(m_equation);

    }
    if(!m_equation->setExpression(data(Qt::DisplayRole).toString())){
        setData(true, StandardItemModel::ErrorRole);
        setData(m_equation->errorString(), StandardItemModel::ErrorStringRole);
    }
    else{
        setData(false, StandardItemModel::ErrorRole);
    }
    // updateParametersUsed(data(Qt::DisplayRole).toString());
}

void ParametricEquationItem::addParameterItem(ParametricEquationParameterItem *item)
{
    m_parameterItems.push_back(item);
    m_isParametersUsed.push_back(false);
    if(m_equation){
        m_equation->addParameter();
    }


}


ParametricEquationParameterItem::ParametricEquationParameterItem(int index):StandardItem("p"+QString::number(index),0, "SliderMinMax.qml", 0,"Parameters for function","",-3.15,3.15), m_index(index)
{

}

void ParametricEquationParameterItem::takeToken()
{
    --m_tokens;
    if(m_tokens<1){
        setEnabled(false);
    }
}

void ParametricEquationParameterItem::giveToken()
{
    ++m_tokens;
    if(m_tokens==1){
        setEnabled(true);
    }
}
void ParametricEquationParameterItem::setData(const QVariant &value, int role)
{
    if(role==Qt::DisplayRole){

        for(auto equation : qAsConst(m_equations)){
            equation->setParameter(m_index-1, value.toDouble());
        }
        {
            double max=data(StandardItemModel::MaxRole).toDouble();
            if(value.toDouble()>max){
                setData(value, StandardItemModel::MaxRole);
            }
        }
        {
            double min=data(StandardItemModel::MinRole).toDouble();
            if(value.toDouble()<min){
                setData(value, StandardItemModel::MinRole);
            }
        }
    }
//    if(role==StandardItemModel::MaxRole && m_maxItem){
//        m_maxItem->setData(value);

//    }
//    if(role==StandardItemModel::MinRole && m_minItem){
//        m_minItem->setData(value);
//    }
    StandardItem::setData(value, role);
}

ParametricEquation::~ParametricEquation()
{
    for(auto p:qAsConst(m_parameters)){
        delete p;
    }
}
bool ParametricEquation::isError()
{
    return  m_isError;
}

void ParametricEquation::setError(bool isError)
{
    m_isError=isError;
}
bool ParametricEquation::setExpression(const QString& expression)
{

    //    try{
    //       // m_parserX.SetExpr(expression.toStdWString());

    //    }
    //    catch (mu::Parser::exception_type &e)
    //        {
    //            qWarning() << e.GetMsg();
    //            return false;
    //        }
    m_parser.SetExpr(expression.toStdWString());
    setError(false);

    return !checkError();

}

bool ParametricEquation::checkError()
{
    value();
    return isError();
}

double ParametricEquation::value()
{
    if(isError()){
        return 0;
    }
    double x=0;
    try{

        x=m_parser.Eval();
    }
    catch (mu::Parser::exception_type &e)
    {
        setError(true);
        m_errorString=QString::fromStdWString(e.GetMsg());
        return 0;
    }
    return x;

}

std::vector<double> ParametricEquation::mupValue(double mint, double maxt, double step)
{
    int size=(maxt-mint)/step;
    std::vector<double> testV;
    testV.reserve(size);
    double value=mint;
    for(int i=0; i<size; ++i){
        testV[i]=value;
        mint+=step;
    }
    m_parser.Eval(&testV.front(),size);
    return testV;
}

void ParametricEquation::addParameter()
{
    m_parameters.push_back(new double(0));
    QString name="p"+QString::number(m_parameters.size());
    m_parser.DefineVar(name.toStdWString(), m_parameters.last());
}

void ParametricEquation::setParameter(int index, double value)
{
    *m_parameters[index]=value;
}

QString ParametricEquation::expression()
{
    QString str=QString::fromStdWString(m_parser.GetExpr());
    for(int i=1; i<=m_parameters.size();++i){
        str.replace(QString("p")+QString::number(i), QString::number(*m_parameters[i-1]));
    }
    return str;
}
