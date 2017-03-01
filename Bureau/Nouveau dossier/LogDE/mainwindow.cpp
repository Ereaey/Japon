#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_chart = new QChart();
    m_chart->legend()->hide();
    m_chart->createDefaultAxes();

    QChartView *chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand);

    ui->gridLayout->addWidget(chartView,0, 0);

    ui->listParams->clear();

    m_simulate = new SimulationDE();
    connect(m_simulate, SIGNAL(outputGraph(QVector<QList<QPointF>>,int*,double)), this, SLOT(inputGraph(QVector<QList<QPointF>>,int*,double)),
            Qt::DirectConnection);
    connect(this, SIGNAL(drawCourbe()), this, SLOT(draw()), Qt::AutoConnection);

    m_simulate->getReferenceSTEP23();
}

void MainWindow::inputGraph(QVector<QList<QPointF>> lines, int *trial, double fitness)
{
    li = lines;


    emit drawCourbe();
}

void MainWindow::draw()
{
    qDebug() << "oki";
    m_lines.clear();

    m_chart->removeAllSeries();

    for (int i = 0; i < li.size(); i++)
    {
        QLineSeries *n = new QLineSeries(this);
        n->append(li[i]);
        m_lines.push_back(n);
        m_chart->addSeries(m_lines[i]);
    }

    m_chart->createDefaultAxes();
    /*
    m_chart->axisX()->setRange(0.240, 0.510);
    m_chart->axisY()->setRange(0, 0.5);
    m_chart->axisY()->setTitleText("I(nA)");
    m_chart->axisX()->setTitleText("V(V)");
    */
    qDebug() << "oki2";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_5_clicked()
{
    m_simulate->startAlgo();
}

