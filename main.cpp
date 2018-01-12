#include <iostream>
#include "Constants.h"
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"
#include "EventDisplay.h"
#include "DetectorGeometry.h"
#include "EventWriter.h"
#include "TString.h"

int main(int argc, char** argv)
{
    TApplication app("app", &argc, argv);
    
    // make fitter object
    int dim = 2;
    double accuracy = 0.00001;
    double scale = 30;

    /*
    EventDisplay* EvdX = new EventDisplay("X events", "z [cm]", "x [cm]");
    EventDisplay* EvdY = new EventDisplay("Y events", "z [cm]", "y [cm]");
    EvdX -> Update();
    EvdY -> Update();
    */
    
    // now, need to read the data files and then perform the fitting (do a plot in between)
    TFile* file_in = new TFile("/home/philipp/Private/T2K/GUI_indep/MCFiles/WMMC_Run1_1_wNoise.root", "open");    
    TTree* tree_in = (TTree*) file_in -> Get("tree");
    TBranch* br = tree_in -> GetBranch("fDefaultReco.");

    int number_events = tree_in -> GetEntries();

    // prepare the object that is going to write the processed events back to disk
    EventWriter* ew = new EventWriter("./out.root");
    
    // now, read the hits coming from a single event
    IngridEventSummary* evt_in = new IngridEventSummary();    
    br -> SetAddress(&evt_in);

    // get the first event here
    int RequestedEvent = 0;
    
    //std::cin >> RequestedEvent;
    //std::cout << "got " << RequestedEvent << std::endl;

    //while(RequestedEvent != -1)
    while(RequestedEvent < number_events)
    {
	tree_in -> GetEntry(RequestedEvent);
	IngridHitSummary* Hit = new IngridHitSummary();

	std::vector<GEMVector> HitsY;
	std::vector<GEMVector> HitsX;

	// forward this event also to the output writer
	ew -> SetEvent(evt_in);

	for(int i = 0; i < evt_in -> NIngridHits(); i++)
	{
	    Hit = evt_in -> GetIngridHit(i);
	
	    // get the hit coordinates (use only one view for the moment)
	    if((Hit -> mod == 15) && (Hit -> time < 20))
	    {
		if(Hit -> view == 0)
		{
		    // the Y-plane hits
		    HitsY.push_back(GEMVector(dim));

		    HitsY.back().SetCoord(0, Hit -> z + GEO_Z_OFFSET);
		    HitsY.back().SetCoord(1, Hit -> xy + GEO_XY_OFFSET);
		}
		else if(Hit -> view == 1)
		{
		    // the X-plane hits
		    HitsX.push_back(GEMVector(dim));

		    HitsX.back().SetCoord(0, Hit -> z + GEO_Z_OFFSET);
		    HitsX.back().SetCoord(1, Hit -> xy + GEO_XY_OFFSET);	
		}
	    }
	}

	std::cout << "retained " << HitsX.size() << " hits in x" << std::endl;
	std::cout << "retained " << HitsY.size() << " hits in y" << std::endl;

	// perform the X fitting
	GEMFitter* gf = new GEMFitter(dim);
	
	// now set the screen size manually for testing
	GEMVector min(2);
	GEMVector max(2);
	min.SetCoord(0, 0.0); min.SetCoord(1, 0.0);
	max.SetCoord(0, 45.0); max.SetCoord(1, 100.0);
	gf -> SetScreenSize(min, max);

	// add all the points to the fitting object and perform the actual fit
	for(std::vector<GEMVector>::iterator it = HitsX.begin(); it != HitsX.end(); it++)
	{
	    gf -> AddPoint(*it);
	}
	
	gf -> PerformFit(accuracy, scale);
	
	std::cout << "this is the X dataset for event " << RequestedEvent << std::endl;
	gf -> PrintDataset();

	// read back the fitted tracks
	std::vector<GEMTrack> TracksX = gf -> GetTracks();
	
	for(int i = 0; i < TracksX.size(); i++)
	{
	    ew -> AddTrack(TracksX.at(i), EW_VIEW_X, evt_in);
	}

	// perform the Y fitting
	gf -> Reset();

	// add all the points to the fitting object and perform the actual fit
	for(std::vector<GEMVector>::iterator it = HitsY.begin(); it != HitsY.end(); it++)
	{
	    gf -> AddPoint(*it);
	}

	gf -> PerformFit(accuracy, scale);
	
	std::cout << "this is the Y dataset for event " << RequestedEvent << std::endl;
	gf -> PrintDataset();

	// read back the fitted tracks
	std::vector<GEMTrack> TracksY = gf -> GetTracks();
	
	for(int i = 0; i < TracksY.size(); i++)
	{
	    ew -> AddTrack(TracksY.at(i), EW_VIEW_Y, evt_in);
	}

	// at this point, have all found tracks in the X- and Y-planes available
	// now, just need to save them back to a ROOT tree

	// TODO: call here the event writer again to add the found tracks
	ew -> WriteEvent();

	/*
	// now plot everything
	EvdX -> Clear();
	EvdY -> Clear();
    
	EvdX -> PlotHits(HitsX);
	EvdX -> PlotTracks(TracksX);
	EvdX -> Update();    
	
	EvdY -> PlotHits(HitsY);
	EvdY -> PlotTracks(TracksY);
	EvdY -> Update();
	*/

	// save the event displays
	//EvdX -> SaveAs(Form("./output/outX-%d.pdf", RequestedEvent));
	//EvdY -> SaveAs(Form("./output/outY-%d.pdf", RequestedEvent));

	// take the next event
	//std::cin >> RequestedEvent;

	RequestedEvent++;
    }

    std::cout << "stopped by user" << std::endl;
    delete ew;

    app.Run();
    return(0);
}
