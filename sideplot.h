#ifndef SIDEPLOT_H
#define SIDEPLOT_H

#include <qwt_plot.h>

#include "pointstream.h"

class PointStream;
class SidePlot : public QwtPlot
{
public:
    SidePlot( QWidget* = NULL );
    std::vector<PointStream*> streams;
    virtual ~SidePlot();
    void addStream();
};

#endif // SIDEPLOT_H
