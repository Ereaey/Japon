#ifndef DEPHASE3_H
#define DEPHASE3_H

#include <stdio.h>
#include <QFileDialog>
#include "DESolver.h"
#include <QObject>
#include "simulation.h"
#include <QThread>

class DEPhase3 : public QThread, public DESolver
{
    Q_OBJECT
    public:
        DEPhase3(QString path, int dim,int pop, Simulation *ref, QObject *e = 0) : DESolver(dim,pop), QThread(e)
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


    private:
        int count;
        Simulation *reference;
        QString m_path;
};

#endif // DEPHASE3_H
