#include "radiusscaleengine.h"
#include <iostream>
RadiusScaleEngine::RadiusScaleEngine()
{

}

// 버그 수정을 위한 overriding ( AutoScale 설정시 초기 그래프에 음수 값이 표시되어 대칭이 깨지는 문제 fix )
void RadiusScaleEngine::autoScale(int maxSteps, double &x1, double &x2, double &stepSize) const
{
    x1 = 0.0;
    if(x2 < 1.0)
        x2 = 1.0;
    QwtLinearScaleEngine::autoScale(maxSteps, x1, x2, stepSize);
}
