#ifndef __EVENT_REFEREE_H
#define __EVENT_REFEREE_H

#include "EventMetrics.h"
#include "GEMFitter.h"
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "IngridUtils.h"

class EventReferee
{
public:
    EventReferee();
    void CompareEvents(IngridEventSummary* ref, IngridEventSummary* aux, int view);
    void Report();
    
private:
    TH1D* single_track_orientation;
    TH1D* vertex_distance;
    TH1D* vertex_MC_distance_ref;
    TH1D* vertex_MC_distance_aux;
    TH2D* number_tracks;
};

#endif
