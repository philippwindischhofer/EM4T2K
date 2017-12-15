#ifndef __EVENT_METRICS_H
#define __EVENT_METRICS_H

#include "GEMFitter.h"
#include "TMath.h"

class EventMetrics
{
public:
    static double TrackOrientation(GEMTrack a, GEMTrack b);
};

#endif
