#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QtCharts/QLineSeries>
#include <QMap>

QT_CHARTS_USE_NAMESPACE

struct Param
{
    QString name;
    double value;
};

class Simulation : public QObject
{
    Q_OBJECT
    public:
        explicit Simulation(QString path, QObject *parent = 0);
        ~Simulation();
        void setParam(QString name, int value);
        void addGraph(QString nameX, QString nameY);
        void simul();
        double getMoyenne(QString nameRef, QString nameX);
        QVector<double> getPointAxe(QString nameRef, QString nameX);
        QVector<double> getPoint(QString nameRef, QString nameX, QString nameY);
        void treatment();
        QLineSeries *getSeries(QString nameX, QString nameY);

        QVector<Param*> params;
        QMap<QString, QVector<double>*> valuesString;

        double comparaison(Simulation *s, QString nameRef, QString nameX);
    signals:

    public slots:

    private:
        double m_moyenne;
        QString m_path;
        QString m_content;


        QMap<int, QVector<double>*> valuesInt;
};

#endif // SIMULATION_H
