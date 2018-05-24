#include "histogramplot.h"
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_histogram.h>
#include <qwt_column_symbol.h>
#include <qwt_series_data.h>

HistogramPlot::HistogramPlot( QWidget* parent )
    : QwtPlot( QwtText( "" ), parent )
{
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::gray );
    setCanvas( canvas );

    QwtLegend *legend = new QwtLegend;
    insertLegend( legend, QwtPlot::RightLegend );

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX( false );
    grid->enableY( true );
    grid->enableXMin( false );
    grid->enableYMin( false );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach( this );

    histogram = new QwtPlotHistogram();
    data = new QwtIntervalSeriesData(QVector<QwtIntervalSample>());
    histogram->setData(data);
    histogram->attach( this );
}
