#include "boardplot.h"
#include <qwt_polar_grid.h>
#include "pointpolarstream.h"
#include "pointpolardegreeseries.h"
#include <qwt_symbol.h>
#include <QStaticText>
#include <qwt_polar_canvas.h>
#include "radiusscaleengine.h"

void BoardPlot::drawCanvas(QPainter *p, const QRectF &rect) const
{
    QwtPolarPlot::drawCanvas( p, rect );

    if(centerText != "")
    {
        QPen penHText(Qt::red);
        p->setPen(penHText);
        p->setFont(QFont("Arial", 16, QFont::Bold));
        p->drawText( canvas()->width() / 2, canvas()->height() / 2, centerText);
    }
}

BoardPlot::BoardPlot( QWidget* parent )
    : QwtPolarPlot( QwtText( "" ), parent ), marker( new QwtPolarMarker() )
{
    const QwtInterval radialInterval( 0.0, 8.0 );
    const QwtInterval azimuthInterval( 0.0, 360.0 );

    setAutoReplot( true );
    setPlotBackground( QBrush( QColor(43, 30, 30) ) );
    setTitle("");

    // scales

    setScale( QwtPolar::Azimuth,
        azimuthInterval.minValue(), azimuthInterval.maxValue(),
        azimuthInterval.width() / 12 );

    setScaleMaxMinor( QwtPolar::Azimuth, 0 );
    setScaleMaxMinor( QwtPolar::Radius, 2 );
    setAutoScale( QwtPolar:: Radius );
    setScaleEngine( QwtPolar::Radius, new RadiusScaleEngine);

    // grids, axes

    auto d_grid = new QwtPolarGrid();
    d_grid->setPen( QPen( Qt::white ) );
    for ( int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++ )
    {
        d_grid->showGrid( scaleId );
        d_grid->showMinorGrid( scaleId );

        QPen minorPen( Qt::gray );
        minorPen.setStyle( Qt::DotLine );
        d_grid->setMinorGridPen( scaleId, minorPen );
    }
    d_grid->setAxisPen( QwtPolar::AxisAzimuth, QPen( Qt::white ) );

    d_grid->showAxis( QwtPolar::AxisAzimuth, true );
    d_grid->showAxis( QwtPolar::AxisLeft, false );
    d_grid->showAxis( QwtPolar::AxisRight, false );
    d_grid->showAxis( QwtPolar::AxisTop, true );
    d_grid->showAxis( QwtPolar::AxisBottom, false );
    d_grid->showGrid( QwtPolar::Azimuth, true );
    d_grid->showGrid( QwtPolar::Radius, true );
    d_grid->attach( this );

    //d_grid->setRenderHint( QwtPolarItem::RenderAntialiased, false );
    // marker

    marker->setPosition( QwtPointPolar( 0, 0 ) );
    marker->setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::blue ), QPen( Qt::transparent ), QSize( 40, 40 ) ) );
    marker->setLabelAlignment( Qt::AlignHCenter | Qt::AlignTop );
    marker->hide();
    marker->attach( this );

    addStream();
}

BoardPlot::~BoardPlot()
{
    for(size_t i = 0; i < streams.size(); i++)
        delete streams[i];
}

void BoardPlot::addStream()
{
    PointPolarStream* stream = new PointPolarStream(this);
    streams.push_back(stream);
}

void BoardPlot::setMarkerPosition(QwtPointPolar pp)
{
    double distance = sqrt( pp.toPoint().x() * pp.toPoint().x() + pp.toPoint().y() * pp.toPoint().y() );
    QColor color;
    int cval = distance * 50;
    if(cval > 255)
        cval = 255;
    color = QColor(cval, 0, 255 - cval);
    marker->setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( color ), QPen( Qt::transparent ), QSize( 40, 40 ) ) );
    marker->setPosition( PointPolarDegreeSeries::fromRadian( pp ) );
}

void BoardPlot::showMarker()
{
    marker->show();
}

void BoardPlot::hideMarker()
{
    marker->hide();
}

void BoardPlot::setCenterText(QString text)
{
    centerText = text;
    replot();
}
