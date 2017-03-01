#ifndef SIMULATIONDE_H
#define SIMULATIONDE_H

#include "dephase1.h"
#include "dephase2.h"
#include "dephase3.h"
#include "qobject.h"

class SimulationDE : public QObject
{
    Q_OBJECT
    public:
        SimulationDE();
        void startAlgo();
        void getReferenceSTEP1();
        void getReferenceSTEP23();

    public slots:
        void inputGraph(QVector<QLineSeries*> lines, int *trial, double fitness);

    signals:
        void outputGraph(QVector<QList<QPointF>> lines, int *trial, double fitness);

    private:
        void startAlgoPhase1();
        void startAlgoPhase2();
        void startAlgoPhase3();

        QString m_filename;
        Simulation *m_referenceSTEP1;
        Simulation *m_referenceSTEP23;
        DEPhase1 *m_solve1;
        DEPhase2 *m_solve2;
        DEPhase3 *m_solve3;

        QString m_pathSTEP1;
        QString m_pathSTEP23;
};

#endif // SIMULATIONDE_H
