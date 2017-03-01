#include "dephase1.h"

#define N_DIM 3
#define N_POP 50
#define MAX_GENERATIONS	200

double DEPhase1::EnergyFunction(double *trial,bool &bAtSolution)
{
    double result = 0.0;

    Simulation *s = new Simulation(m_path);
    s->setParam("Ian_Vb", (int)trial[0]);
    s->setParam("Iav_Vb", (int)trial[1]);

    s->simul();
    s->treatment();

    result = sqrt(pow(reference->comparaison(s, "v(net34)", "i(v45)"), 2) +
                  pow(reference->comparaison(s, "v(net34)", "i(v46)"), 2));

    qDebug() << "STEP 1 : Fitness" << result;
    if (count++ % nPop == 0)
    {
        int t[10] = {100};
        for (int i = 0; i < 3; i++)
            t[i] = Solution()[i];


        Simulation *best = new Simulation(m_path);

        best->setParam("Ian_Vb", t[0]);
        best->setParam("Iav_Vb", t[1]);

        best->simul();
        best->treatment();



        QVector<QLineSeries*> te;
        te.push_back(best->getSeries("v(net34)", "i(v45)"));
        te.push_back(best->getSeries("v(net34)", "i(v46)"));
        emit simA(te, t, Energy());
        printf("%d %lf\n",count / nPop + 1,Energy());
    }

    delete s;

    return(result);
}

void DEPhase1::load()
{
    start();
}

void DEPhase1::run()
{
     Solve(MAX_GENERATIONS);
}
