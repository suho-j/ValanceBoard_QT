#ifndef RADIUSSCALEENGINE_H
#define RADIUSSCALEENGINE_H
#include <qwt_scale_engine.h>

class RadiusScaleEngine : public QwtLinearScaleEngine
{
public:
    RadiusScaleEngine();

    void autoScale(int maxSteps, double &x1, double &x2, double &stepSize) const;
};

#endif // RADIUSSCALEENGINE_H
