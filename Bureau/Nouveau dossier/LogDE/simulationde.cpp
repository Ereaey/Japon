#include "simulationde.h"

SimulationDE::SimulationDE()
{
    m_pathSTEP1 = "/home/etudiant/Bureau/ngspice/DE/net_cl1_DC_pulse(for_step1).sp";
    m_pathSTEP23 = "/home/etudiant/Bureau/ngspice/DE/net_cl1_DC_cont.sp";

    m_referenceSTEP1 = new Simulation(m_pathSTEP1);
    m_referenceSTEP1->setParam("Iav_Vb", 415.0);
    m_referenceSTEP1->setParam("Ian_Vb", 390.0);

    m_referenceSTEP1->simul();
    m_referenceSTEP1->treatment();


    m_referenceSTEP23 = new Simulation(m_pathSTEP23);

    m_referenceSTEP23->setParam("fv_Vb", 238.0);
    m_referenceSTEP23->setParam("fv_Vdlt", 580.0);
    m_referenceSTEP23->setParam("gv_Vm", 432.0);
    m_referenceSTEP23->setParam("Ian_Vin", 451.0);//461.0
    m_referenceSTEP23->setParam("Iav_Vb", 415.0);
    m_referenceSTEP23->setParam("fn_Vb", 237.0);
    m_referenceSTEP23->setParam("fn_Vdlt", 520.0);
    m_referenceSTEP23->setParam("gn_Vm", 156.0);
    m_referenceSTEP23->setParam("Iav_Vin", 461.0);
    m_referenceSTEP23->setParam("Ian_Vb", 390.0);
    m_referenceSTEP23->setParam("rn_Vm", 0.0);

    m_referenceSTEP23->simul();
    m_referenceSTEP23->treatment();

}

void SimulationDE::getReferenceSTEP1()
{
    QVector<QList<QPointF>> t;
    t.append(m_referenceSTEP1->getSeries("v(net34)", "i(v45)")->points());
    t.append(m_referenceSTEP1->getSeries("v(net34)", "i(v46)")->points());

    int trial[10];
    double fitness = 0.1;
    emit outputGraph(t, trial, fitness);
}

void SimulationDE::getReferenceSTEP23()
{
    QVector<QList<QPointF>> t;
    t.append(m_referenceSTEP23->getSeries("v(x0.net108)", "i(v49)")->points());
    t.append(m_referenceSTEP23->getSeries("v(x0.net108)", "i(v32)")->points());

    int trial[10];
    double fitness = 0.1;
    emit outputGraph(t, trial, fitness);
}

void SimulationDE::inputGraph(QVector<QLineSeries *> lines, int *trial, double fitness)
{
    QVector<QList<QPointF>> t;
    for (int i = 0; i < lines.size(); i++)
        t.append(lines[i]->points());
    emit outputGraph(t, trial, fitness);
}

void SimulationDE::startAlgo()
{
    startAlgoPhase3();
}

void SimulationDE::startAlgoPhase1()
{
    m_solve1 = new DEPhase1(m_pathSTEP1, 2, 50, m_referenceSTEP1);
    connect(m_solve1, SIGNAL(simA(QVector<QLineSeries*>,int*,double)), this, SLOT(inputGraph(QVector<QLineSeries*>,int*,double)), Qt::DirectConnection);

    double min[2];
    double max[2];

    min[0] = 100.0;
    max[0] = 800.0;

    min[1] = 100.0;
    max[1] = 800.0;


    QThread *thread = new QThread;

    m_solve1->Setup(min, max, stBest1Exp, 0.3, 0.6);
    m_solve1->moveToThread(thread);
    m_solve1->load();
}


void SimulationDE::startAlgoPhase2()
{
    m_solve2 = new DEPhase2(m_pathSTEP23, 4, 100, m_referenceSTEP23);
    connect(m_solve2, SIGNAL(simA(QVector<QLineSeries*>,int*,double)), this, SLOT(inputGraph(QVector<QLineSeries*>,int*,double)), Qt::DirectConnection);

    double min[4];
    double max[4];

    min[0] = 100.0;
    max[0] = 800.0;

    min[1] = 100.0;
    max[1] = 800.0;

    min[2] = 100.0;
    max[2] = 800.0;

    min[3] = 100.0;
    max[3] = 800.0;

    QThread *thread = new QThread;

    m_solve2->Setup(min, max, stBest2Exp, 0.3, 0.2);
    m_solve2->moveToThread(thread);
    m_solve2->load();
}

void SimulationDE::startAlgoPhase3()
{
    m_solve3 = new DEPhase3(m_pathSTEP23, 4, 100, m_referenceSTEP23);
    connect(m_solve3, SIGNAL(simA(QVector<QLineSeries*>,int*,double)), this, SLOT(inputGraph(QVector<QLineSeries*>,int*,double)), Qt::DirectConnection);

    double min[4];
    double max[4];

    min[0] = 100.0;
    max[0] = 800.0;

    min[1] = 100.0;
    max[1] = 800.0;

    min[2] = 100.0;
    max[2] = 800.0;

    min[3] = 100.0;
    max[3] = 800.0;

    QThread *thread = new QThread;

    m_solve3->Setup(min, max, stBest2Exp, 0.3, 0.2);
    m_solve3->moveToThread(thread);
    m_solve3->load();
}
