#ifndef __EVENT_WRITER_H
#define __EVENT_WRITER_H

#include <iostream>
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"
#include "DetectorGeometry.h"

#define EW_VIEW_X 1
#define EW_VIEW_Y 0

#define EV_Z_SCALE 10.0
#define EV_Z_OFFSET 18.1

#define EV_XY_SCALE 10.0
#define EV_XY_OFFSET 10.0

class EventWriter
{
public:
    EventWriter(std::string file);
    ~EventWriter();
    void SetEvent(IngridEventSummary* evt);
    void AddTrack(GEMLine track, int view);
    void AddTrack(GEMTrack track, int view, IngridEventSummary* evt);
    void WriteEvent();
    
private:
    TFile* file_out;
    TTree* tree_out;

    IngridEventSummary* evt_out;
    PMReconSummary* recon;
};

#endif
