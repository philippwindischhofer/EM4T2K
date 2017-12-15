#ifndef __EVENT_REFEREE_H
#define __EVENT_REFEREE_H

#include "EventMetrics.h"
#include "GEMFitter.h"
#include "RootIncludes.h"

class EventReferee
{
public:
    EventReferee();
    void CompareEvents(std::vector<GEMTrack> ref, std::vector<GEMTrack> aux);
    void Report();
    
private:
    TH1D* single_track_orientation;
    TH1D* vertex_distance;
};

#endif
