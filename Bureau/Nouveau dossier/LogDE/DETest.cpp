// Differential Evolution Test Program
// Based on algorithms developed by Dr. Rainer Storn & Kenneth Price
// Written By: Lester E. Godwin
//             PushCorp, Inc.
//             Dallas, Texas
//             972-840-0208 x102
//             godwin@pushcorp.com
// Created: 6/8/98
// Last Modified: 6/8/98
// Revision: 1.0

#include "detest.h"

double PolynomialSolver::EnergyFunction(double *trial,bool &bAtSolution)
{
    //qDebug() << "\n\nDebut fitness";
    Simulation *s = new Simulation("/home/etudiant/Bureau/Ngspice_anthony/net_cl1.sp");

    s->setParam("fv_Vb", (int)trial[0]);
    s->setParam("fv_Vdlt", (int)trial[1]);
    s->setParam("gv_Vm", (int)trial[2]);
    s->setParam("Ian_Vin", (int)trial[3]);
    s->setParam("Iav_Vb", (int)trial[4]);
    s->setParam("fn_Vb", (int)trial[5]);
    s->setParam("fn_Vdlt", (int)trial[6]);
    s->setParam("gn_Vm", (int)trial[7]);
    s->setParam("Iav_Vin", (int)trial[8]);
    s->setParam("Ian_Vb", (int)trial[9]);

    s->simul();
    s->treatment();


    /*
    double e = s->getMoyenne("v(x0.net108)", "i(v49)", "i(v32)");
    double ref = (reference->getMoyenne("v(x0.net108)", "i(v49)", "i(v32)"));


    QVector<double> e1 = s->getPoint("v(x0.net108)", "i(v49)", "i(v32)");
    QVector<double> ref1 = reference->getPoint("v(x0.net108)", "i(v49)", "i(v32)");
    double result = 0.0;*/

    double pointV = s->getPointAxe("v(x0.net108)", "i(v49)").size() - reference->getPointAxe("v(x0.net108)", "i(v49)").size();
    double pointN = s->getPointAxe("v(x0.net108)", "i(v32)").size() - reference->getPointAxe("v(x0.net108)", "i(v32)").size();
    if (pointV < 0)
        pointV *= -1;
    if (pointN < 0)
        pointN *= -1;
    double result = pow(pointV, 2) + pow(pointN, 2);
    result = pointV;

    qDebug() << s->getPointAxe("v(x0.net108)", "i(v49)").size() << reference->getPointAxe("v(x0.net108)", "i(v49)").size();
    qDebug() << s->getPointAxe("v(x0.net108)", "i(v32)").size() << reference->getPointAxe("v(x0.net108)", "i(v32)").size();

    double V1 = 0.0;
    double V2 = 0.0;
    for (int i = 0; i < reference->getPointAxe("v(x0.net108)", "i(v49)").size(); i++)
    {
        double value = abs(reference->getPointAxe("v(x0.net108)", "i(v49)")[i] - reference->getPointAxe("v(x0.net108)", "i(v49)")[i]);
        V1 += value;
        value = abs(reference->getPointAxe("v(x0.net108)", "i(v32)")[i]  - reference->getPointAxe("v(x0.net108)", "i(v32)")[i]);
        V2 += value;
    }
    V1 /= reference->getPointAxe("v(x0.net108)", "i(v49)").size();
    V2 /= reference->getPointAxe("v(x0.net108)", "i(v49)").size();
    result = sqrt(pow(V1, 2) + pow(V2, 2));
    /*
    double result = pow(s->getMoyenne("v(x0.net108)", "i(v49)") - reference->getMoyenne("v(x0.net108)", "i(v49)"), 2) + pow(s->getMoyenne("v(x0.net108)", "i(v32)") - reference->getMoyenne("v(x0.net108)", "i(v32)"), 2);
    result = sqrt(result);
    */
    /*
    double value = 0.0;
    if (ref.size() - e.size() < 0)
        value = 0 + -1 * (ref.size() - e.size());
    else
        value = ref.size() - e.size();

    int v = 0;
    if (value <= 30)
        v = reference->comparaison(s,"v(x0.net108)", "i(v49)", "i(v32)");

    result += (value * 10000.0) + 1500.0 +  v * (-15.0);
    qDebug() << e.size() << ref.size() << result << "Equi :" << v;
    */

//
    //result = 50000;
    //bool a = false;
    /*
    if (ref < 0)
        ref *= -1;
    if (e < 0)
        e *= -1;
    *//*
    if (e > ref*0.6)
        result += 50000;
    else if (e < ref*1.4)
        result += 50000;
    else
    {
        if (e > ref)
            result += (ref / e)*(-50000);
        else
            result += (e / ref)*(-50000);
    }

    if (result < 0)
        result *= -1;*/

   //qDebug() << "Valeur A : " << pow(s->getMoyenne("v(x0.net108)", "i(v49)") - reference->getMoyenne("v(x0.net108)", "i(v49)"), 2) << " Valeur b : " << pow(s->getMoyenne("v(x0.net108)", "i(v32)") - reference->getMoyenne("v(x0.net108)", "i(v32)"), 2) << " fitness" << result;

    /*
    if (result <= 10000)
    {
        for (int i = 0; i < e1.size(); i++)
        {
            qDebug() << "Point " << i << " : " << e1[i] << " Ref : " << ref1[i] << "Intervalle[" << ref1[i]*1.025 << ";" << ref1[i]*0.975 << "]";
            if (e1[i] > ref1[i]*0.975)
                result += 20000;
            else if (e1[i] < ref1[i]*1.025)
                result += 20000;
        }
        a = true;
    }
    qDebug() << "Fitness:" << result;

*/


    if (result == 0)
    {
        int t[10];
        for (int i = 0; i < 10; i++)
            t[i] = Solution()[i];

        emit simA(t, Energy());
    }
    qDebug() << "Fitness" << result;
	if (count++ % nPop == 0)
    {
        int t[10];
        for (int i = 0; i < 10; i++)
            t[i] = Solution()[i];

        emit simA(t, Energy());
		printf("%d %lf\n",count / nPop + 1,Energy());
    }

    //if (result <= 10000)


    delete s;

    //qDebug() << "Fin fitness\n";
	return(result);
}

void PolynomialSolver::load()
{
    start();
}

void PolynomialSolver::run()
{
     Solve(800);
}
