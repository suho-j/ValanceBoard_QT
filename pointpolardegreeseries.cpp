#include "pointpolardegreeseries.h"

QwtPointPolar PointPolarDegreeSeries::fromRadian(QwtPointPolar pp)
{
    return QwtPointPolar(pp.azimuth() * 180 / 3.141592653589793238462643383279502884197169399375105820974944, pp.radius());
}

QwtPointPolar PointPolarDegreeSeries::toRadian(QwtPointPolar pp)
{
    return QwtPointPolar(pp.azimuth() * 3.141592653589793238462643383279502884197169399375105820974944 / 180, pp.radius());
}

PointPolarDegreeSeries::PointPolarDegreeSeries(QVector<QwtPointPolar> samples)
{
    setSamples(samples);
}

void PointPolarDegreeSeries::setSamples(QVector<QwtPointPolar> samples)
{
    d_samples.clear();
    d_boundingRect = QRectF();
    if (samples.count())
    {
        QwtPointPolar degreepp0 = fromRadian(samples.at(0));
        QwtInterval xInterval = QwtInterval(degreepp0.azimuth(), degreepp0.azimuth());
        QwtInterval yInterval = QwtInterval(degreepp0.radius(), degreepp0.radius());
        foreach (QwtPointPolar sample, samples)
        {
            QwtPointPolar degreepp = fromRadian(sample);
            d_samples << degreepp;
            xInterval |= degreepp.azimuth();
            yInterval |= degreepp.radius();
        }
        d_boundingRect = QRectF(xInterval.minValue(), yInterval.minValue(), xInterval.width(), yInterval.width());
    }
}

void PointPolarDegreeSeries::append(QwtPointPolar sample)
{
    QwtPointPolar degreepp = fromRadian(sample);
    d_samples << degreepp;
    QwtInterval xInterval(d_boundingRect.x(), d_boundingRect.x() + d_boundingRect.width());
    QwtInterval yInterval(d_boundingRect.y(), d_boundingRect.y() + d_boundingRect.height());
    xInterval |= degreepp.azimuth();
    yInterval |= degreepp.radius();
    d_boundingRect = QRectF(xInterval.minValue(), yInterval.minValue(), xInterval.width(), yInterval.width());
}

const QVector<QwtPointPolar> PointPolarDegreeSeries::samples()
{
    QVector<QwtPointPolar> copy = d_samples;
    for(int i = 0; i < copy.size(); i++)
        copy[i] = toRadian(copy[i]);
    return copy;
}

QRectF PointPolarDegreeSeries::boundingRect() const
{
    return d_boundingRect;
}
