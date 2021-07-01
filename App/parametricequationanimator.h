#ifndef PARAMETRICEQUATIONANIMATOR_H
#define PARAMETRICEQUATIONANIMATOR_H

#include <QObject>
#include <standarditemmodel.h>
#include <QTimer>
class ParametricEquationsShapeGeneratorSettings;

class ParametricEquationAnimator : public StandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
public:
    enum Roles {
        QmlFileRole = RolesTobeSaved + 500,
        FromRole,
        ToRole,
        IntervalRole

    };
    explicit ParametricEquationAnimator(ParametricEquationsShapeGeneratorSettings* pESGenerator, QObject *parent = nullptr);
    virtual QHash<int,QByteArray> roleNames() const override;
    Q_INVOKABLE void startAnimation(bool loop=true);
    Q_INVOKABLE void stopAnimation();
    void setParameters();
    bool isRunning(){return m_timer!=nullptr;}
private:
    void initAnimation();
    void updateParameter();
    void updateParameterReverse();
    ParametricEquationsShapeGeneratorSettings* m_pESGenerator;
    std::unique_ptr<QTimer> m_timer;
    int countInterval=0;
    bool m_repeat=false;

signals:
    void isRunningChanged();
};

#endif // PARAMETRICEQUATIONANIMATOR_H
