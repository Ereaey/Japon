#include "simulation.h"

Simulation::Simulation(QString path, QObject *parent) : QObject(parent)
{
    m_path = path;
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    m_content = file.readAll();
    file.close();
}

void Simulation::setParam(QString name, double value)
{
    for (int i = 0; i < params.size(); i++)
    {
        if (params[i]->name == name)
        {
            params[i]->value = value;
            return;
        }
    }
    Param *p = new Param;
    p->name = name;
    p->value = value;

    params.push_back(p);
}


void Simulation::simul()
{
    for (int i = 0; i < params.size(); i++)
        m_content.replace(params[i]->name + "=xm", params[i]->name + "=" + QString::number(params[i]->value) + "m");

    QString newpath = m_path.replace(".sp", "_temp.sp");
    QFile file(newpath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

    QTextStream out(&file);
    out << m_content;

    file.close();

    QProcess process;

    process.start("ngspice -b -r " + m_path.replace(".sp", "_out") + " -o task_1.out " + newpath);
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput() + process.readAllStandardError();
    qDebug() << stdout;
}

void Simulation::treatment()
{
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();

    series.push_back(series1);
    series.push_back(series2);

    QString fileName = m_path.replace(".sp", "_out");

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QString g;
    int i = 0;
    QByteArray line;
    int nbVar = 0;
    for (int e = 0; e < 7; e++)
    {
         line = file.readLine();
         if (line.contains("No. Variables:"))
         {
            nbVar = (line.split(' ')[2].remove(line.split(' ')[2].size() - 1, 1)).toInt();
            qDebug() << "ok" << (line.split(' ')[2]);
         }
    }

    qDebug() << "Nombres de variables : " << nbVar;

    for (int e = 0; e < nbVar; e++)
    {
         line = file.readLine();
         QStringList l = QString(line).split('\t');
         qDebug() << l[0] << ":" << l[1] << ":" << l[2] << ":" << l[3];
         QVector<double> *value = new QVector<double>;
         valuesInt[l[1].toInt()] = value;
         valuesString[l[2]] = value;
    }

    file.readLine();

    int nb = 0;
    int nb2 = 0;

    double V1 = 0;
    double A1 = 0;
    double A2 = 0;
    double V2 = 0;
    m_moyenne = 0;

    qDebug() << "t";
    while (!file.atEnd())
    {
        QString data = file.readLine();
        data.remove(0, 1);
        data.remove(data.size() - 1, 1);
        data.remove('\t');
        qDebug() << data;
        //series0->append();
        /*
        if (line.at(0) != '\t' && nb != 0)
        {
            i = 0;
            series1->append(A1, A2 * 1000000000);
            series2->append(A1, V2 * 1000000000);
            //qDebug() << V2 << ":" << A1;
            //qDebug() << V2 << ":" << A2;
            m_moyenne += (A2 - V2);
        }
        line.remove(0, 1);
        line.remove(line.size() - 1, 1);
        i++;
        if (i == 109)
        {
            V1 = line.toDouble();
        }
        if (i == 259)
        {
            V2 = line.toDouble();
            nb++;
        }

        if (i == 249)
        {
            A2 = line.toDouble();
        }
        if (i == 53)
        {
            A1 = line.toDouble();
        }*/
        valuesInt[i]->push_back(data.toDouble());
        i++;
        if (i > nbVar - 1)
            i = 0;
    }
    //m_moyenne /= nb;
    //qDebug() << m_moyenne;

    file.close();
}

QLineSeries *Simulation::getSeries(QString nameX, QString nameY)
{
    QLineSeries *s = new QLineSeries;
    for (int e = 0; e < valuesString[nameX]->size(); e++)
    {
        if (valuesString[nameX]->value(e) < 0.6 && valuesString[nameY]->value(valuesString[nameX]->size() - e) != 0)
            s->append(valuesString[nameX]->value(e), (valuesString[nameY]->value(valuesString[nameX]->size() - e)* 1000000000));
    }
    return s;
}

double Simulation::getMoyenne(QString nameRef, QString nameX, QString nameY)
{
    double value = 0.0;
    for (int e = 0; e < valuesString[nameRef]->size(); e++)
    {
        if (valuesString[nameRef]->value(e) < 0.6 && valuesString[nameY]->value(e) != 0)
            value += valuesString[nameX]->value(e) - valuesString[nameY]->value(e);
    }
    return value;
}
