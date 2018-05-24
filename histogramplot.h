#ifndef HISTOGRAMPLOT_H
#define HISTOGRAMPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_histogram.h>
class HistogramPlot : public QwtPlot
{
public:
    HistogramPlot( QWidget* = NULL );
    QwtPlotHistogram *histogram;
    QwtIntervalSeriesData* data;
};

#endif // HISTOGRAMPLOT_H
