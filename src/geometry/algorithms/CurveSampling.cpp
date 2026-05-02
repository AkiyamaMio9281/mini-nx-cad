#include "mnx/geometry/algorithms/CurveSampling.h"

#include <stdexcept>

namespace mnx {

std::vector<Point2> sampleCurve(const Curve2D& curve, std::size_t sampleCount) {
    if(sampleCount < 2) {
        throw std::invalid_argument("Curve sampling requires at least two samples");
    }

    std::vector<Point2> samples;
    samples.reserve(sampleCount);
    for(std::size_t index = 0; index < sampleCount; ++index) {
        const double t = static_cast<double>(index) / static_cast<double>(sampleCount - 1);
        samples.push_back(curve.evaluate(t));
    }
    return samples;
}

} // namespace mnx
