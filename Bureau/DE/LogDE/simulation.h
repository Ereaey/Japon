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

struct Graph
{
    QString nameX;
    QString nameY;
    QLineSeries *series;
};

class Simulation : public QObject
{
    Q_OBJECT
    public:
        explicit Simulation(QString path, QObject *parent = 0);
        void setParam(QString name, int value);
        void addGraph(QString nameX, QString nameY);
        void simul();
        QVector<double> getMoyenne(QString nameRef, QString nameX, QString nameY);
        void treatment();
        QLineSeries *getSeries(QString nameX, QString nameY);

        QVector<QLineSeries*> series;


    signals:

    public slots:

    private:
        double m_moyenne;
        QString m_path;
        QString m_content;
        QVector<Param*> params;
        QMap<QString, QVector<double>*> valuesString;
        QMap<int, QVector<double>*> valuesInt;
};

#endif // SIMULATION_H
