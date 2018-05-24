#include "sideplot.h"

#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot_canvas.h>
#include "pointstream.h"

SidePlot::SidePlot( QWidget* parent )
    : QwtPlot( QwtText( "" ), parent )
{
    setAutoReplot( true );
    setTitle("");

    setAxisScale( QwtPlot::yLeft, 1.0, 10.0 );

    enableAxis( QwtPlot::xBottom, true );

    setAxisAutoScale( QwtPlot::yLeft, true );

    QwtLegend *legend = new QwtLegend;
    insertLegend( legend, QwtPlot::RightLegend );

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setLineWidth( 1 );
    canvas->setPalette( QColor(43, 30, 30) );

    setCanvas( canvas );

    addStream();
}

SidePlot::~SidePlot()
{
    for(size_t i = 0; i < streams.size(); i++)
        delete streams[i];
}

void SidePlot::addStream()
{
    PointStream* stream = new PointStream(this);
    streams.push_back(stream);
}
