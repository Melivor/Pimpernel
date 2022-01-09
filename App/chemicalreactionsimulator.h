#ifndef CHEMICALREACTIONSIMULATOR_H
#define CHEMICALREACTIONSIMULATOR_H
#include <vector>
#include <QStringList>
#include <chemicalspecies.h>
#include <standarditemmodel.h>
#include <wallpapergenerator.h>


class ChemicalReactionSimulatorSettings : public WallpaperGeneratorSettings
{
    Q_OBJECT
public:
    ChemicalReactionSimulatorSettings(QObject* parent=nullptr);
    StandardItemModel* setupPrototype();
    QPointF concentrationValue(int species);
    void initSpecies();
   // QString xEquation(){return "x="+m_xEquation.expression();}
   // QString yEquation(){return "y="+m_yEquation.expression();}
    //Q_INVOKABLE QAbstractItemModel* getAnimator(){return m_animator.get();}

private :
    void resetAnimator();
    StandardItemModel* setupSpecificSection();
    StandardItemModel* setupStartValue();
    StandardItemModel* setupStandardSection();
    StandardItemModel* setupChemicalReactionParameters();
   // StandardItemModel* setupAnimationParameters();
    //std::unique_ptr<ParametricEquationAnimator> m_animator;
    grid getGrid(int width, int height);

    std::list<ChemicalSpecies> m_chemicalSpecies;
    int m_gridWidth;
    int m_gridHeight;
};


class ChemicalReactionSimulator: public WallpaperGenerator
{

public:
    ChemicalReactionSimulator(QQuickItem *parent=nullptr);
    virtual void paint(QPainter* painter, double width, double height) override{};
    void increment();
    void initConcentrations();
    WallpaperGenerator* copy() override;
private:


   QStringList m_componentsName;
   std::vector<ParametricEquation> m_componentEquations;
};

class MetaChemicalReactionSimulator : public MetaGenerator{
    const QString name() override{return "Chemical reaction simulator";}
    void init() override {m_item.reset(new ChemicalReactionSimulator());}
};

#endif // CHEMICALREACTIONSIMULATOR_H
