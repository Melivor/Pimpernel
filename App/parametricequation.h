#ifndef PARAMETRICEQUATION_H
#define PARAMETRICEQUATION_H


#include "include/muParser.h"
#include "QPointF"
#include <string>
#include <QString>
#include "standarditem.h"
#include <QDebug>


class ParametricEquation
{
public:
    ParametricEquation();
    ~ParametricEquation();
    static double rnd(double x);
    void defineVar(const QString& varName, double* var){m_parser.DefineVar(varName.toStdWString(),var);};
    void clearVar(){m_parser.ClearVar();}
    bool setExpression(const QString& expression);
    bool checkError();
    double value();
    //double value(QList<double> variables);
    std::vector<double> mupValue(double mint, double maxt, double step);
    bool isError();
    void setError(bool isError);
    QString errorString(){return m_errorString;}
    void addParameter();
    void setParameter(int index, double value);
    QString expression();
private:
    //exprtk::expression<double> m_expressionX;
    //exprtk::expression<double> m_expressionY;
    //exprtk::symbol_table<double> m_symbolTable;
    mu::Parser m_parser;
    bool m_isError;
    QString m_errorString;
    QList<double*> m_parameters;

};

class ParametricEquationParameterItem;
class ParametricEquationItem : public StandardItem
{
public:
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
    void setEquation(ParametricEquation* equation);
    void addParameterItem(ParametricEquationParameterItem* item);
    ParametricEquationItem(ParametricEquation* equation, const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", int complexity=0, const QString& description="");
private :
    void updateParametersUsed(const QString& expression);
    ParametricEquation* m_equation;
    QList<ParametricEquationParameterItem*> m_parameterItems;
    QList<bool> m_isParametersUsed;
};

class ParametricEquationParameterItem : public StandardItem
{
public:
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
    //void setMaxItem(StandardItem* item){m_maxItem=item;}
    //void setMinItem(StandardItem* item){m_minItem=item;}
    void addEquation(ParametricEquation* equation){m_equations.push_back(equation);equation->setParameter(m_index-1, data(Qt::DisplayRole).toDouble());};
    ParametricEquationParameterItem(int index=0);
    void takeToken();
    void giveToken();
private :
    QList<ParametricEquation*> m_equations;
    //StandardItem* m_maxItem;
    //StandardItem* m_minItem;
    int m_index=0;
    uint m_tokens=0;
};

#endif // PARAMETRICEQUATION_H
