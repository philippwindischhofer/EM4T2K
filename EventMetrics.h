#ifndef __EVENT_METRICS_H
#define __EVENT_METRICS_H

#include "GEMFitter.h"
#include "TMath.h"

class EventMetrics
{
public:
    static double TrackOrientation(GEMTrack a, GEMTrack b);
    static double VertexDistance(std::vector<GEMTrack> a, std::vector<GEMTrack> b);
    static double VertexDistanceToPoint(std::vector<GEMTrack> a, GEMVector b);
};

#endif
