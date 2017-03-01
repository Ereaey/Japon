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

#include <stdio.h>
#include <QFileDialog>
#include "DESolver.h"
#include "simulation.h"

Simulation *reference;
QString m_path;

// Polynomial fitting problem
class PolynomialSolver : public DESolver
{
public:
	PolynomialSolver(int dim,int pop) : DESolver(dim,pop), count(0) {;}
	double EnergyFunction(double trial[],bool &bAtSolution);

private:
	int count;
};

double PolynomialSolver::EnergyFunction(double *trial,bool &bAtSolution)
{
    Simulation s(m_path);

    s.setParam("fv_Vb", (int)trial[0]);
    s.setParam("fv_Vdlt", (int)trial[1]);
    s.setParam("gv_Vm", (int)trial[2]);
    s.setParam("Ian_Vin", (int)trial[3]);
    s.setParam("Iav_Vb", (int)trial[4]);
    s.setParam("fn_Vb", (int)trial[5]);
    s.setParam("fn_Vdlt", (int)trial[6]);
    s.setParam("gn_Vm", (int)trial[7]);
    s.setParam("Iav_Vin", (int)trial[8]);
    s.setParam("Ian_Vb", (int)trial[9]);

    s.simul();
    s.treatment();

    /*
    qDebug() << "fv_Vb" << (int)trial[0];
    qDebug() << "fv_Vdlt" << (int)trial[1];
    qDebug() << "gv_Vm" << (int)trial[2];
    qDebug() << "Ian_Vin" << (int)trial[3];
    qDebug() << "Iav_Vb" << (int)trial[4];
    qDebug() << "fn_Vb" << (int)trial[5];
    qDebug() << "fn_Vdlt"  << (int)trial[6];
    qDebug() << "gn_Vm" <<  (int)trial[7];
    qDebug() << "Iav_Vin" << (int) trial[8];
    qDebug() << "Ian_Vb"  << (int)trial[9];*/

    QVector<double> e = s.getMoyenne("v(x0.net108)", "i(v49)", "i(v48)");
    QVector<double> ref = (reference->getMoyenne("v(x0.net108)", "i(v49)", "i(v48)"));
    double result = 0;
    int execnb = 0;
    int refcount = 0;

    if (e.size() > ref.size())
        refcount = ref.size();
    else
        refcount = e.size();

    for (int i = 0; i < refcount; i++)
    {
        if (e[i] == ref[i])
            execnb++;
    }


    result += (ref.size() - e.size() + ref.size()) + execnb * (-150);
    qDebug() << e.size() << ref.size() << result;
    if (result < -1500)
        exit(1);
    /*
    if ()
	int i, j;
	int const M=60;
	double px, x=-1, dx=M, result=0;

	dx = 2.0 / dx;
	for (i=0; i<=M; i++)
	{
		px = trial[0];
		for (j=1;j<nDim;j++)
			px = x*px + trial[j];

		if (px<-1 || px>1)
			result += (1 - px) * (1 - px);

		x += dx;
	}

	px = trial[0];
	for (j=1;j<nDim;j++)
		px = 1.2*px + trial[j];

	px = px - 72.661;
	if (px<0)
		result += px * px;

	px = trial[0];
	for (j=1; j<nDim; j++)
		px = -1.2*px + trial[j];

	px = px - 72.661;
	
	if (px<0)
		result+=px*px;

	if (count++ % nPop == 0)
		printf("%d %lf\n",count / nPop + 1,Energy());
    */
	return(result);
}

#define N_DIM 10
#define N_POP 100
#define MAX_GENERATIONS	80000
int main(void)
{

    m_path = "/home/etudiant/Bureau/Ngspice_anthony/net_cl1.sp";
    reference = new Simulation(m_path);

    reference->setParam("fv_Vb", 238.0);
    reference->setParam("fv_Vdlt", 580.0);
    reference->setParam("gv_Vm", 432.0);
    reference->setParam("Ian_Vin", 461.0);//461.0
    reference->setParam("Iav_Vb", 415.0);
    reference->setParam("fn_Vb", 237.0);
    reference->setParam("fn_Vdlt", 520.0);
    reference->setParam("gn_Vm", 156.0);
    reference->setParam("Iav_Vin", 461.0);
    reference->setParam("Ian_Vb", 390.0);

    reference->simul();
    reference->treatment();

	double min[N_DIM];
	double max[N_DIM];
	int i;

    PolynomialSolver solver(N_DIM, N_POP);

    for (i = 0; i < N_DIM; i++)
	{
        max[i] = 600.0;
        min[i] = 150.0;
	}

    solver.Setup(min, max, stRandToBest1Exp, 0.7, 0.1);
	
	printf("Calculating...\n\n");
	solver.Solve(MAX_GENERATIONS);

	double *solution = solver.Solution();

	printf("\n\nBest Coefficients:\n");
	for (i=0;i<N_DIM;i++)
		printf("[%d]: %lf\n",i,solution[i]);

    return 0;
}
