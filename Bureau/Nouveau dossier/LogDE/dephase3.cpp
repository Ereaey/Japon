#include "dephase3.h"

#define MAX_GENERATIONS	400

double DEPhase3::EnergyFunction(double *trial,bool &bAtSolution)
{
    double result = 0.0;

    Simulation *s = new Simulation(m_path);
    s->setParam("fn_Vb", (int)trial[0]);
    s->setParam("fn_Vdlt", (int)trial[1]);
    s->setParam("gn_Vm", (int)trial[2]);
    s->setParam("Ian_Vin", (int)trial[3]);

    s->setParam("Ian_Vb", 390);
    s->setParam("Iav_Vb", 413);
    s->setParam("rn_Vm", 0);

    s->setParam("fv_Vb", 0);
    s->setParam("fv_Vdlt", 0);
    s->setParam("gv_Vm", 0);
    s->setParam("Iav_Vin", 0);

    s->simul();
    s->treatment();

    result = reference->comparaison(s, "v(x0.net108)", "i(v32)");

    qDebug() << "STEP 3 : Fitness" << result;

    if (count++ % nPop == 0)
    {
        int t[10] = {100};

        Simulation *best = new Simulation(m_path);

        best->setParam("Ian_Vb", 390);
        best->setParam("Iav_Vb", 413);
        best->setParam("rn_Vm", 0);

        best->setParam("fv_Vb", 0);
        best->setParam("fv_Vdlt", 0);
        best->setParam("gv_Vm", 0);
        best->setParam("Iav_Vin", 0);

        best->setParam("Ian_Vin", (int)Solution()[3]);
        best->setParam("fn_Vb", (int)Solution()[0]);
        best->setParam("fn_Vdlt", (int)Solution()[1]);
        best->setParam("gn_Vm", (int)Solution()[2]);


        best->simul();
        best->treatment();


        qDebug() << "Nouvelle génération";
        qDebug() << count / nPop + 1 << Energy();

        QVector<QLineSeries*> te;
        te.push_back(best->getSeries("v(x0.net108)", "i(v32)"));
        te.push_back(reference->getSeries("v(x0.net108)", "i(v32)"));
        emit simA(te, t, Energy());

        delete best;
    }

    delete s;

    return(result);
}

void DEPhase3::load()
{
    start();
}

void DEPhase3::run()
{
     Solve(MAX_GENERATIONS);
}
