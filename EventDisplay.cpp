#include "EventDisplay.h"

EventDisplay::EventDisplay(std::string title) : canv(title.c_str(), title.c_str()), g()
{
    canv.cd();
    g.Draw();
}

void EventDisplay::PlotHit(GEMVector& hit)
{
    canv.cd();
    g.SetPoint(g.GetN(), hit.GetCoord(0), hit.GetCoord(1));
    g.Draw("A*");
}

void EventDisplay::PlotHits(std::vector<GEMVector>& hits)
{
    for(std::vector<GEMVector>::iterator it = hits.begin(); it != hits.end(); it++)
    {
	PlotHit(*it);
    }
}

void EventDisplay::PlotTrack(GEMLine& track)
{
    canv.cd();
    // TODO: find scale factor for line automatically
    GEMVector start = track.GetRefPoint() - 100. * (track.GetDirVect());
    GEMVector end = track.GetRefPoint() + 100. * (track.GetDirVect());
    TLine* line = new TLine(start.GetCoord(0), start.GetCoord(1), end.GetCoord(0), end.GetCoord(1));
    line -> Draw("same");
}

void EventDisplay::PlotTracks(std::vector<GEMLine>& tracks)
{
    for(std::vector<GEMLine>::iterator it = tracks.begin(); it != tracks.end(); it++)
    {
	PlotTrack(*it);
    }
}

void EventDisplay::Update()
{
    canv.cd();
    canv.Update();
}

void EventDisplay::Clear()
{
    g.Set(0);
    canv.Clear();
}

void EventDisplay::SaveAs(std::string file)
{
    canv.cd();
    canv.SaveAs(file.c_str());
}
    
