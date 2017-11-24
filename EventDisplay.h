#ifndef __EVENT_DISPLAY_H
#define __EVENT_DISPLAY_H

#include <iostream>
#include <TCanvas.h>
#include <TH2D.h>
#include <TBox.h>
#include <TGraph.h>
#include <TLine.h>
#include "GEMFitter.h"

//#include "IngridConstants.h"
//#include "INGRID_Dimension.hh"

class EventDisplay
{
public:
    EventDisplay(std::string title);
    void PlotHit(GEMVector& hit);
    void PlotHits(std::vector<GEMVector>& hits);
    void PlotTrack(GEMLine& track);
    void PlotTracks(std::vector<GEMLine>& tracks);
    void Update();
    void SaveAs(std::string file);
    void Clear();

private:
    TCanvas canv;
    TGraph g;
};

#endif
