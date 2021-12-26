#ifndef CHEMICALSPECIES_H
#define CHEMICALSPECIES_H

#include <vector>
#include <QString>
#include <parametricequation.h>
typedef std::vector<std::vector<double>> grid;
struct ChemicalSpecies
{
    QString name;
    ParametricEquation equation;
    grid concentrations;

};

/*
class ChemicalSpeciesItem : public StandardItem
{
public:
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;
    void setEquation(ParametricEquation* equation);
    void addParameterItem(ParametricEquationParameterItem* item);
    ChemicalSpeciesItem(ParametricEquation* equation, const QString& name="", const QVariant &value=QVariant(), const QString& qmlFile="", int complexity=0, const QString& description="");
private :
    void updateParametersUsed(const QString& expression);
    ParametricEquation* m_equation;
    QList<ParametricEquationParameterItem*> m_parameterItems;
    QList<bool> m_isParametersUsed;
};
*/
#endif // CHEMICALSPECIES_H
