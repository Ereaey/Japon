#ifndef DETEST_H
#define DETEST_H

#define N_DIM 10
#define N_POP 200
#define MAX_GENERATIONS	200

#include <stdio.h>
#include <QFileDialog>
#include "DESolver.h"
#include <QObject>
#include "simulation.h"
#include <QThread>

// Polynomial fitting problem
class PolynomialSolver : public QThread, public DESolver
{
    Q_OBJECT
    public:
        PolynomialSolver(int dim,int pop, Simulation *ref, QObject *e = 0) : DESolver(dim,pop), QThread(e)
        {
            count = 0;
            reference = ref;
        }
        double EnergyFunction(double trial[],bool &bAtSolution);

    public slots:
        void load();

    protected:
        void run();

    signals:
        void simA(int *trial, double fitness);

    private:
        int count;
        Simulation *reference;
};


#endif // DETEST_H
