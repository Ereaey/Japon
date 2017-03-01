#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include "simulationde.h"

QT_CHARTS_USE_NAMESPACE

struct Courbe
{
    QString nameX;
    QString nameY;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void finishSim();
    void drawCourbe();

private slots:
    void on_pushButton_5_clicked();
    void draw();

public slots:
        void inputGraph(QVector<QList<QPointF>> lines, int *trial, double fitness);

private:
    Ui::MainWindow *ui;

    QVector<Courbe*> a_courbes;
    QVector<QLineSeries*> m_lines;
    QVector<QList<QPointF>> li;
    QChart *m_chart;

    SimulationDE *m_simulate;

};

#endif // MAINWINDOW_H
