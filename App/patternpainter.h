#ifndef PATTERNPAINTER_H
#define PATTERNPAINTER_H


#include <QMainWindow>
#include <QQuickPaintedItem>


class Parameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant maxValue READ maxValue CONSTANT)
    Q_PROPERTY(QVariant minValue READ minValue CONSTANT)
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(QList<QVariant> options READ options CONSTANT)
public:
    Parameter(const QString& name, const QVariant& value, const QVariant& minValue=0, const QVariant& maxValue=0, const QList<QVariant>& options=QList<QVariant>(), QObject* parent=nullptr):QObject(parent), m_name(name), m_value(value), m_minValue(minValue), m_maxValue(maxValue), m_options(options){};
    const QString& name(){return m_name;}
    const QVariant& value() const{return m_value;}
    void setValue(const QVariant& value){m_value=value;emit valueChanged();}
    void setMax(const QVariant& max){m_maxValue=max;}
    void setMin(const QVariant& min){m_minValue=min;}
    const QVariant& maxValue() const{return m_maxValue;}
    const QVariant& minValue() const{return m_minValue;}
    const QVariantList& options(){return m_options;}
    int type(){return m_value.type();}
private:
    QString m_name;
    QVariant m_value;
    QVariant m_minValue;
    QVariant m_maxValue;

    QVariantList m_options;
signals:
    void valueChanged();

};

class PatternPainter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int maxIt READ maxIt WRITE setMaxIt NOTIFY maxItChanged)
    Q_PROPERTY(double stroke READ stroke WRITE setStroke NOTIFY strokeChanged)
    Q_PROPERTY(QList<double> startValues READ startValues CONSTANT)
    Q_PROPERTY(QList<double> startValuesMin READ startValuesMin CONSTANT)
    Q_PROPERTY(QList<double> startValuesMax READ startValuesMax CONSTANT)
    Q_PROPERTY(QList<double> parameters READ parameters CONSTANT)
    Q_PROPERTY(QList<double> maxValues READ maxValues CONSTANT)
    Q_PROPERTY(QList<Parameter*> variantParameters READ variantParameters CONSTANT)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged)
    QML_ELEMENT

public:
    PatternPainter(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);
    virtual void paint(QPainter* painter, double width, double height)=0;
    int maxIt() const{return m_maxIt;}
    void setMaxIt(int it){m_maxIt=it; update();emit maxItChanged();}
    double stroke()const{return m_stroke;}
    void setStroke(double stroke){m_stroke=stroke;update();}
    Q_INVOKABLE void setStartValues(int index, double value){if(index>=0 && index<m_startValues.size())m_startValues[index]=value;update();}
    QList<double> startValues(){return m_startValues;};
    QList<double> startValuesMin(){return m_startValuesMin;};
    QList<double> startValuesMax(){return m_startValuesMax;};
    Q_INVOKABLE void setParameters(int index, double value){if(index>=0 && index<m_parameters.size())m_parameters[index]=value;update();}
    QList<double> parameters(){return m_parameters;}
    QList<double> minValues(){return m_minValues;};
    QList<double> maxValues(){return m_maxValues;};
    double stepSize() const{return m_stepSize;}
    void stepSize(double value){m_stepSize=value;update();}
    Q_INVOKABLE void saveAsPng(QUrl url);
    const QColor& backgroundColor(){return m_backgroundColor;}
    void setBackgroundColor(const QColor& color){m_backgroundColor=color;emit backgroundColorChanged();update();}
    const QColor& strokeColor(){return m_strokeColor;}
    void setStrokeColor(const QColor& color){m_strokeColor=color;emit strokeColorChanged();update();}
    QList<Parameter*> variantParameters(){return m_variantParameters.values();}
    void reDraw(){update();}
    void connectParameters();
    QVariant value(const QString& parameter){return m_variantParameters.value(parameter)->value();}

protected:
    QColor m_backgroundColor;
    QColor m_strokeColor;
    int m_maxIt=80;
    double m_stepSize=1;
    double m_stroke=1.2;
    QList<double> m_startValues;
    QList<double> m_startValuesMin;
    QList<double> m_startValuesMax;
    QList<double> m_parameters;
    QList<double> m_minValues;
    QList<double> m_maxValues;
    QMap<QString, Parameter*> m_variantParameters;
    double function(double p1, double p2, double p3);

signals:
    void maxItChanged();
    void strokeChanged();
    void stepSizeChanged();
    void backgroundColorChanged();
    void strokeColorChanged();
};

class MetaPainter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
public:
  //  MetaPainter(QObject *parent=nullptr) {QObject(parent);}
    virtual const QString name(){return "Meta painter";}
    Q_INVOKABLE QQuickPaintedItem* item() {return m_item;}
    Q_INVOKABLE virtual void init(){};
    Q_INVOKABLE virtual void release(){if(m_item!=nullptr){delete m_item;m_item=nullptr;};}
protected:
    PatternPainter* m_item;
};

#endif // PATTERNPAINTER_H
