#include <iostream>
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"
#include "EventDisplay.h"
#include "DetectorGeometry.h"
#include "EventWriter.h"
#include "IngridUtils.h"
#include "TString.h"

int main(int argc, char** argv)
{
    TApplication app("app", &argc, argv);
	
    TFile* file_a = new TFile("/home/philipp/Private/T2K/GUI_indep/MCFiles/WMMC_Run1_1_wNoise_recon.root");    
    //TFile* file_b = new TFile("/home/philipp/Private/T2K/EM4T2K/testevents.root");
    TFile* file_b = new TFile("/home/philipp/Private/T2K/EM4T2K/out.root");

    TTree* tree_a = (TTree*) file_a -> Get("tree");
    TTree* tree_b = (TTree*) file_b -> Get("tree");

    TBranch* br_a = tree_a -> GetBranch("fDefaultReco.");
    TBranch* br_b = tree_b -> GetBranch("fDefaultReco.");

    IngridEventSummary* evt_a = new IngridEventSummary();    
    IngridEventSummary* evt_b = new IngridEventSummary();

    br_a -> SetAddress(&evt_a);
    br_b -> SetAddress(&evt_b);

    // event displays to compare the two
    EventDisplay* EvdX_a = new EventDisplay("File A: X events", "z [cm]", "x [cm]");
    EventDisplay* EvdY_a = new EventDisplay("File A: Y events", "z [cm]", "y [cm]");

    EventDisplay* EvdX_b = new EventDisplay("File B: X events", "z [cm]", "x [cm]");
    EventDisplay* EvdY_b = new EventDisplay("File B: Y events", "z [cm]", "y [cm]");

    EvdX_a -> Update();
    EvdY_a -> Update();

    EvdX_b -> Update();
    EvdY_b -> Update();

    // listen for events to compare:
    int RequestedEvent = 0;
    
    std::cin >> RequestedEvent;
    std::cout << "got " << RequestedEvent << std::endl;

    while(RequestedEvent != -1)
    {
	tree_a -> GetEntry(RequestedEvent);
	tree_b -> GetEntry(RequestedEvent);

	// get all hits and fitted tracks of both files in terms of the same objects!
	std::vector<GEMVector> hits_x_a = IngridUtils::GetHits(evt_a, EW_VIEW_X);
	std::vector<GEMVector> hits_y_a = IngridUtils::GetHits(evt_a, EW_VIEW_Y);
	std::vector<GEMLine> tracks_x_a = IngridUtils::GetTracks(evt_a, EW_VIEW_X);
	std::vector<GEMLine> tracks_y_a = IngridUtils::GetTracks(evt_a, EW_VIEW_Y);

	std::vector<GEMVector> hits_x_b = IngridUtils::GetHits(evt_b, EW_VIEW_X);
	std::vector<GEMVector> hits_y_b = IngridUtils::GetHits(evt_b, EW_VIEW_Y);
	std::vector<GEMLine> tracks_x_b = IngridUtils::GetTracks(evt_b, EW_VIEW_X);
	std::vector<GEMLine> tracks_y_b = IngridUtils::GetTracks(evt_b, EW_VIEW_Y);

	// for visualization, also draw the events side by side
	EvdX_a -> Clear();
	EvdY_a -> Clear();
	EvdX_a -> PlotHits(hits_x_a);
	EvdX_a -> PlotTracks(tracks_x_a);
	EvdY_a -> PlotHits(hits_y_a);
	EvdY_a -> PlotTracks(tracks_y_a);
	EvdX_a -> Update();
	EvdY_a -> Update();

	EvdX_b -> Clear();
	EvdY_b -> Clear();
	EvdX_b -> PlotHits(hits_x_b);
	EvdX_b -> PlotTracks(tracks_x_b);
	EvdY_b -> PlotHits(hits_y_b);
	EvdY_b -> PlotTracks(tracks_y_b);
	EvdX_b -> Update();
	EvdY_b -> Update();

	// take the next event
	std::cin >> RequestedEvent;
	std::cout << "got " << RequestedEvent << std::endl;
    }
    
    std::cout << "stopped by user" << std::endl;

    app.Run();
    return(0);
}
