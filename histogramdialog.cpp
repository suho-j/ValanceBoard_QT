#include "histogramdialog.h"
#include "ui_histogramdialog.h"

HistogramDialog::HistogramDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::HistogramDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    ui->lr_histogram->setTitle("LEFT & RIGHT");
    ui->lr_histogram->histogram->setBrush(QBrush(Qt::red));
    ui->lr_histogram->histogram->setTitle("x");

    ui->tb_histogram->setTitle("TOP & BOTTOM");
    ui->tb_histogram->histogram->setBrush(QBrush(Qt::blue));
    ui->tb_histogram->histogram->setTitle("y");
}

HistogramDialog::~HistogramDialog()
{
    delete ui;
}

void HistogramDialog::setHistogramData(TBLRHistograms hists)
{
    ui->lr_histogram->setAxisScale(QwtPlot::xBottom, hists.min, hists.max);
    ui->tb_histogram->setAxisScale(QwtPlot::xBottom, hists.min, hists.max);
    double length = hists.max - hists.min;
    double ratio = length / hists.divides;
    QVector<QwtIntervalSample> samps_tb;
    for(int i = 0; i < hists.hist_tb.size(); i++)
    {
        double start = i * ratio + hists.min;
        double end = start + 1 * ratio;
        samps_tb.append(QwtIntervalSample(hists.hist_tb[i], start, end));
    }
    ui->tb_histogram->data->setSamples(samps_tb);

    QVector<QwtIntervalSample> samps_lr;
    for(int i = 0; i < hists.hist_lr.size(); i++)
    {
        double start = i * ratio + hists.min;
        double end = start + 1 * ratio;
        samps_lr.append(QwtIntervalSample(hists.hist_lr[i], start, end));
    }
    ui->lr_histogram->data->setSamples(samps_lr);
    ui->tb_histogram->replot();
    ui->lr_histogram->replot();
}
