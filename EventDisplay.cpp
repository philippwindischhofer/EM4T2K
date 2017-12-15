#include "EventDisplay.h"

EventDisplay::EventDisplay(std::string title, std::string xtitle, std::string ytitle)
{
    std::vector<double> XBins = GetConfigFromFile(PATH_X_BINS);
    std::vector<double> ZBins = GetConfigFromFile(PATH_Z_BINS);
    
    canv = new TCanvas(title.c_str(), title.c_str());
    hist = new TH2D(title.c_str(), title.c_str(), ZBins.size() - 1, &ZBins[0], XBins.size() - 1, &XBins[0]);

    hist -> GetXaxis() -> SetTitle(xtitle.c_str());
    hist -> GetYaxis() -> SetTitle(ytitle.c_str());

    hist -> SetStats(kFALSE);
}

void EventDisplay::PlotHit(GEMVector& hit)
{
    canv -> cd();
    hist -> SetBinContent(hist -> GetXaxis() -> FindBin(hit.GetCoord(0)), hist -> GetYaxis() -> FindBin(hit.GetCoord(1)), 10000);
}

void EventDisplay::PlotHits(std::vector<GEMVector>& hits)
{
    for(std::vector<GEMVector>::iterator it = hits.begin(); it != hits.end(); it++)
    {
	PlotHit(*it);
    }

    hist -> Draw();
}

void EventDisplay::PlotTrack(GEMLine& track)
{
    // TODO: find scale factor for line automatically
    GEMVector start = track.GetRefPoint() - 100. * (track.GetDirVect());
    GEMVector end = track.GetRefPoint() + 100. * (track.GetDirVect());
    TLine* line = new TLine(start.GetCoord(0), start.GetCoord(1), end.GetCoord(0), end.GetCoord(1));

    line->SetLineStyle(track.GetStyle());
    line->SetLineWidth(2);
    line->SetLineColor(track.GetColor());  
    
    canv -> cd();
    line -> Draw("same");
}

void EventDisplay::PlotTracks(std::vector<GEMLine>& tracks)
{
    for(std::vector<GEMLine>::iterator it = tracks.begin(); it != tracks.end(); it++)
    {
	PlotTrack(*it);
    }
}

void EventDisplay::PlotTracks(std::vector<GEMTrack>& tracks)
{
    for(int i = 0; i < tracks.size(); i++)
    {
	GEMLine cur = tracks.at(i).GetLine();
	PlotTrack(cur);
    }
}

void EventDisplay::Update()
{
    canv -> cd();
    PrintGeometry();
    canv -> Update();
}

void EventDisplay::Clear()
{
    canv -> cd();
    hist -> Reset();
    canv -> Clear();
    PrintGeometry();
}

void EventDisplay::SaveAs(std::string file)
{
    canv -> cd();
    canv -> SaveAs(file.c_str());
}
    
void EventDisplay::PrintGeometry()
{
    std::string paths[] = {PATH_BOXGRID, PATH_X_SCINT, PATH_Y_SCINT};

    for(int i = 0; i < sizeof(paths) / sizeof(paths[0]); i++)
    {    
	// need to read the datafile and print all the boxes
	std::ifstream in;
	in.open(paths[i].c_str());

	double Xs, Xe, Ys, Ye;
    
	while(in >> Xs >> Ys >> Xe >> Ye)
	{
	    if(Xs > 0)
	    {
		TBox* box = new TBox(Xs, Ys, Xe, Ye);
		box -> SetLineColor(kBlue - 9);
		box -> SetFillStyle(0);
		canv -> cd();
		box -> Draw("same");
	    }
	}

	in.close();
    }
}

std::vector<double> EventDisplay::GetConfigFromFile(std::string path)
{
    std::vector<double> retval;

    std::ifstream in;
    in.open(path.c_str());

    double tmp;
    
    while(in >> tmp)
    {
	retval.push_back(tmp);
    }

    return retval;
}
