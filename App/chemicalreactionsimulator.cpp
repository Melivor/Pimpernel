#include "chemicalreactionsimulator.h"
#include "standarditemwithactions.h"
#include "random"

ChemicalReactionSimulatorSettings::ChemicalReactionSimulatorSettings(QObject* parent) :WallpaperGeneratorSettings(setupPrototype(), parent)
{
    qDebug()<<"chemical reaction settings done";
}

StandardItemModel* ChemicalReactionSimulatorSettings::setupPrototype()
{
    StandardItemModel* mainModel=new StandardItemModel("ChemicalReactionSimulator","ChemicalReactionSimulator");
    mainModel->appendSection(ChemicalReactionSimulatorSettings::setupStandardSection());
    mainModel->appendSection(setupStartValue());
    mainModel->appendSection(setupChemicalReactionParameters());
    mainModel->appendSection(setupSpecificSection());
    //mainModel->appendSection(setupAnimationParameters());
    return mainModel;
}

void ChemicalReactionSimulatorSettings::initSpecies()
{
    auto& model=activeModel()->sections().at(2);


    int i=1;
    while(i<model->rowCount()){
        ChemicalSpecies chemicalSpecie{
            model->data(model->index(i,0)).toString(),
            ParametricEquation(),
            grid()
        };
        m_chemicalSpecies.push_back(chemicalSpecie);
        i+=3;

    }


}
StandardItemModel* ChemicalReactionSimulatorSettings::setupStandardSection()
{
    StandardItemModel* model=new StandardItemModel("Standard");
    model->appendRow(new StandardItem(QObject::tr("Pt number"),1000,"BoundIntNumberField.qml",0,QObject::tr("Number of pts"),QObject::tr(""),100,100000));
    model->appendRow(new StandardItem(QObject::tr("Base color"),QColor("grey"),"ColorField.qml",0,QObject::tr("Base color")));
    model->appendRow(new StandardItem(QObject::tr("Background color"),QColor("white"),"ColorField.qml",0,QObject::tr("White color")));
    model->appendRow(new StandardItem(QObject::tr("Stroke"),1,"BoundNumberField.qml",0,QObject::tr("Stroke"),QObject::tr(""),0,10));
    model->appendRow(new StandardItem(QObject::tr("Colorful"), true,"CheckBox.qml",0,tr("Colorful")));

    return model;
}

StandardItemModel* ChemicalReactionSimulatorSettings::setupSpecificSection()
{
    StandardItemModel* model=new StandardItemModel("Transformations");
    model->appendRow(new StandardItem(QObject::tr("Scale"),20,"BoundNumberField.qml",0,QObject::tr("Scaling"),QObject::tr(""),0,100));
    model->appendRow(new StandardItem(QObject::tr("Rotation number"),4,"SliderInt.qml",0,QObject::tr("Number of rotation"),QObject::tr(""),1,10));
    model->appendRow(new StandardItem(QObject::tr("Angle"),0,"BoundNumberField.qml",0,QObject::tr("Angle of figure"),QObject::tr("degrees"),0,360));
    return model;
}

StandardItemModel* ChemicalReactionSimulatorSettings::setupStartValue()
{
    StandardItemModel* model=new StandardItemModel("Start value");
    model->appendRow(new StandardItem(QObject::tr("Init method"),0,"ComboBox.qml",0,QObject::tr("Init concentration method"),QObject::tr(""),0,1,{"Random", "Equi"}));

    return model;
}

StandardItemModel* ChemicalReactionSimulatorSettings::setupChemicalReactionParameters()
{
    StandardItemModel* model=new StandardItemModel("Chemical reaction");
    StandardItemWithActions* addSpeciesButton=(new StandardItemWithActions(QObject::tr("Add chemical species"),0,"ActionButton.qml"));

    auto addSpeciesButtonAction =[model,this](const QVariant& value, int role){
        if(role!=Qt::DisplayRole){
            return;
        }
        if(value.toBool()){
            //addSpeciesButton->setData(0);
            StandardItem* nameItem=new StandardItem(QObject::tr("Name of species"),"A","TextField.qml",0,QObject::tr("Name of the species"),"");
            //nameItem->addAction(changeNameAction);
            model->appendRow(nameItem);
            //model->appendRow(new StandardItem(QObject::tr("Equation"),"0","TextFieldWithErrorWarning.qml",0,QObject::tr("Equation"),""));

            ChemicalSpecies chemicalSpecie{
                    "A",
                    ParametricEquation(),
                    grid()
            };
            qDebug()<<"Pushing chemical specie";
            m_chemicalSpecies.push_back(chemicalSpecie);
            qDebug()<<"creating item";
            //ChemicalSpecies* chemicalSpeciesPtr=&(m_chemicalSpecies.back());
            ParametricEquationItem* equationItem=new ParametricEquationItem(&m_chemicalSpecies.back().equation,m_chemicalSpecies.back().name+"(t+1)",0,"TextFieldWithErrorWarning.qml",0,"Equation to compute concentration at next step");
            model->appendRow(equationItem);
            StandardItemWithActions* removeSpeciesButton=(new StandardItemWithActions(QObject::tr("Remove chemical species"),0,"ActionButton.qml"));

            auto removeButtonAction =[removeSpeciesButton, this](const QVariant& value, int role){
                if(role!=Qt::DisplayRole){
                    return;
                }
                if(value.toInt()==1){
                    auto&& model=removeSpeciesButton->model();
                    int index=removeSpeciesButton->index().row();
                    qDebug()<<"Removing species index "<<index<<" rowCount:"<<model->rowCount();
                    const QString name=model->data(model->index(index-2,0)).toString();
                    qDebug()<<"Removing specie: "<<name;
                    model->removeRow(index);
                    model->removeRow(index-1);
                    model->removeRow(index-2);

                    for (auto i =  m_chemicalSpecies.begin(); i !=  m_chemicalSpecies.end();) {
                        if (i->name==name){
                            m_chemicalSpecies.erase(i);
                            break;
                        }
                        else
                            ++i;
                    }

                    //m_chemicalSpecies.erase(chemicalSpeciesPtr);
                    //model->removeRow(index);


                }

            };
            removeSpeciesButton->addAction(removeButtonAction);
            model->appendRow(removeSpeciesButton);
            removeSpeciesButton->setData({-1}, StandardItemModel::RolesTobeSaved);

        }

    };
    addSpeciesButton->setData({-1}, StandardItemModel::RolesTobeSaved);
    addSpeciesButton->addAction(addSpeciesButtonAction);
    model->appendRow(addSpeciesButton);
    return model;
    //model->appendRow(new StandardItem(QObject::tr("Scale"),20,"BoundNumberField.qml",0,QObject::tr("Scaling"),QObject::tr(""),0,100));
}
grid ChemicalReactionSimulatorSettings::getGrid(int width, int height)
{
    for(auto& chemicalSpecies:m_chemicalSpecies){
        chemicalSpecies.concentrations.clear();
        int i=0;
        while(i<height){
            std::vector<double> vec;
            vec.reserve(width);
            int j=0;
            while(j<width){
                vec.push_back(0);
            }
           chemicalSpecies.concentrations.push_back(vec);
        }
    }
    return grid();

}
ChemicalReactionSimulator::ChemicalReactionSimulator(QQuickItem *parent):WallpaperGenerator(parent)
{
    initConcentrations();
    auto settings=new ChemicalReactionSimulatorSettings(this);
    m_settings=settings;
}

WallpaperGenerator* ChemicalReactionSimulator::copy()
{
    auto copy=new ChemicalReactionSimulator();
    copy->setWidth(this->width());
    copy->setHeight(this->height());
    return copy;
}

void ChemicalReactionSimulator::initConcentrations()
{
//   / m_grids
    /*
    m_componentConcentrations.clear();
    m_componentConcentrations.reserve(m_componentEquations.size());
    const size_t gridWidth=1920;
    const size_t gridHeight=1080;
    grid baseGrid;
    baseGrid.resize(gridWidth);
    std::default_random_engine generator;
    double max=1;
    std::uniform_real_distribution<> distribution(0,max);
    for(size_t x=0; x<baseGrid.size(); ++x){
        baseGrid[x].resize(gridHeight);
    }
    for(size_t x=0; x<baseGrid.size(); ++x){
        double total=0;
        for(size_t y=0; y<baseGrid.size(); ++y){
            baseGrid[x][y]=distribution(generator);
            total+=baseGrid[x][y];
        }
        for(size_t y=0; y<baseGrid.size(); ++y){
            baseGrid[x][y]/=total;
        }
    }
    for(size_t i=0; i<m_componentEquations.size();++i){
        m_componentConcentrations.push_back(baseGrid);
    }
    */
}

void ChemicalReactionSimulator::increment()
{
    /*
    for(int i=0; i<m_componentEquations.size();++i){
    for(size_t x=0; x<m_componentConcentrations[i].size(); ++x){
        for(size_t y=0; y<m_componentConcentrations[i][x].size(); ++y){

           // m_componentConcentrations[x][y]=m_componentEquations[i].value();
        }
    }
}
*/
}
