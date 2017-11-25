#ifndef __EVENT_DISPLAY_H
#define __EVENT_DISPLAY_H

#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH2D.h>
#include <TBox.h>
#include <TGraph.h>
#include <TLine.h>

#include "DetectorGeometry.h"
#include "GEMFitter.h"

class EventDisplay
{
public:
    EventDisplay(std::string title, std::string xtitle, std::string ytitle);
    void PlotHit(GEMVector& hit);
    void PlotHits(std::vector<GEMVector>& hits);
    void PlotTrack(GEMLine& track);
    void PlotTracks(std::vector<GEMLine>& tracks);
    void Update();
    void SaveAs(std::string file);
    void Clear();

private:
    void PrintGeometry();
    std::vector<double> GetConfigFromFile(std::string path);
    TCanvas* canv;
    TH2D* hist;
};

#endif
