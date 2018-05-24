#include "histogramcreator.h"

int HistogramCreator::cut(int value, int min, int max)
{
    if(value < min)
        value = min;
    if(value > max)
        value = max;
    return value;
}

HistogramCreator::HistogramCreator(double min, double max, double divides)
    : min(min), max(max), divides(divides)
{

}

TBLRHistograms HistogramCreator::createHistograms(QVector<QwtPointPolar> data)
{
    std::vector<int> hist_lr(divides + 1);
    double length = max - min;
    double offset = -min;
    for(int i = 0; i < data.size(); i++)
    {
        auto point = data[i].toPoint();
        if(point.x() == 0.0 && point.y() == 0.0)
            continue;
        int index = ( divides / length ) * ( point.x() + offset );
        index = cut(index, 0, divides);
        hist_lr[index]++;
    }

    std::vector<int> hist_tb(divides + 1);
    length = max - min;
    offset = -min;
    for(int i = 0; i < data.size(); i++)
    {
        auto point = data[i].toPoint();
        if(point.x() == 0.0 && point.y() == 0.0)
            continue;
        int index = ( divides / length ) * ( point.y() + offset );
        index = cut(index, 0, divides);
        hist_tb[index]++;
    }

    TBLRHistograms hists;
    hists.hist_lr = hist_lr;
    hists.hist_tb = hist_tb;
    hists.min = min;
    hists.max = max;
    hists.divides = divides;

    return hists;
}
