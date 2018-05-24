#ifndef HISTOGRAMCREATOR_H
#define HISTOGRAMCREATOR_H
#include <qwt_point_polar.h>
#include <vector>

struct TBLRHistograms
{
    std::vector<int> hist_lr;
    std::vector<int> hist_tb;
    double min, max, divides;
};

class HistogramCreator
{
private:
    int cut(int value, int min, int max);
public:
    double min, max;
    double divides;
    HistogramCreator(double min, double max, double divides);

    TBLRHistograms createHistograms(QVector<QwtPointPolar> data);
};
#endif // HISTOGRAMCREATOR_H
