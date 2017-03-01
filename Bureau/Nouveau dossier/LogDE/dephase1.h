#ifndef DEPHASE1_H
#define DEPHASE1_H

#include <stdio.h>
#include <QFileDialog>
#include "DESolver.h"
#include <QObject>
#include "simulation.h"
#include <QThread>

class DEPhase1 : public QThread, public DESolver
{
    Q_OBJECT
    public:
        DEPhase1(QString path, int dim,int pop, Simulation *ref, QObject *e = 0) : DESolver(dim,pop), QThread(e)
        {
            m_path = path;
            count = 0;
            reference = ref;
        }
        double EnergyFunction(double trial[],bool &bAtSolution);
    public slots:
        void load();

    protected:
        void run();

    signals:
        void simA(QVector<QLineSeries*> lines, int *trial, double fitness);
        void best(QVector<QLineSeries*> lines, int *trial, double fitness);

    private:
        int count;
        QString m_path;
        Simulation *reference;
};

#endif // DEPHASE1_H
