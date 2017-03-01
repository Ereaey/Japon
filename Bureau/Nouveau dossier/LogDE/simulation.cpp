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

Simulation::~Simulation()
{
    qDeleteAll(valuesInt);
    //qDeleteAll(valuesString);
}

void Simulation::setParam(QString name, int value)
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
    qDebug() << p->name << p->value;

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

    process.start("ngspice -b -r " + m_path.replace(".sp", "_out") + " -o "+ m_path.replace(".sp", "_out")+ ".out " + newpath);
    process.waitForFinished(-1);

    //QString stdout = process.readAllStandardOutput() + process.readAllStandardError();
    //qDebug() << stdout;
}

void Simulation::treatment()
{

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
            //qDebug() << "ok" << (line.split(' ')[2]);
         }
    }

    //qDebug() << "Nombres de variables : " << nbVar;

    for (int e = 0; e < nbVar; e++)
    {
         line = file.readLine();
         QStringList l = QString(line).split('\t');
         //qDebug() << l[0] << ":" << l[1] << ":" << l[2] << ":" << l[3];
         QVector<double> *value = new QVector<double>;
         valuesInt[l[1].toInt()] = value;
         valuesString[l[2]] = value;
    }

    file.readLine();

    m_moyenne = 0;

    while (!file.atEnd())
    {
        QString data = file.readLine();
        data.remove(0, 1);
        data.remove(data.size() - 1, 1);
        data.remove('\t');

        valuesInt[i]->push_back(data.toDouble());
        i++;
        if (i > nbVar - 1)
            i = 0;
    }


    file.close();
}

QLineSeries *Simulation::getSeries(QString nameX, QString nameY)
{
    QLineSeries *s = new QLineSeries;
    for (int e = 0; e < valuesString[nameX]->size(); e++)
    {
        if (valuesString[nameY]->value(valuesString[nameX]->size()-e) != 0)
            s->append(valuesString[nameX]->value(e), (valuesString[nameY]->value(valuesString[nameX]->size()-e)* 1000000000));
    }
    return s;
}

double Simulation::getMoyenne(QString nameRef, QString nameX)
{
    double value = 0;
    int i = 0;
    for (int e = 0; e < valuesString[nameRef]->size(); e++)
    {
        value += valuesString[nameX]->value(e);
        i++;
    }
    value /= i;
    return value;
}

QVector<double> Simulation::getPointAxe(QString nameRef, QString nameX)
{
    QVector<double> points;

    for (int e = 0; e < valuesString[nameRef]->size(); e++)
    {
        points.append(valuesString[nameX]->value(e));
    }

    return points;
}

double Simulation::comparaison(Simulation *s, QString nameRef, QString nameX)
{
    double V1 = 0.0;
    for (int i = 0; i < s->getPointAxe(nameRef, nameX).size(); i++)
    {
        double value = s->getPointAxe(nameRef, nameX)[i] -
                           getPointAxe(nameRef, nameX)[i];
        if (value < 0)
            value *= -1;
        V1 += value;
    }
    V1 /= s->getPointAxe(nameRef, nameX).size();

    return V1;
}
