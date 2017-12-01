#include <iostream>
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"
#include "EventDisplay.h"
#include "DetectorGeometry.h"
#include "TString.h"

int main(int argc, char** argv)
{
    TApplication app("app", &argc, argv);
    
    // make fitter object
    int dim = 2;
    double accuracy = 0.00001;
    double scale = 10;

    EventDisplay* EvdX = new EventDisplay("X events", "z [cm]", "x [cm]");
    EventDisplay* EvdY = new EventDisplay("Y events", "z [cm]", "y [cm]");
    EvdX -> Update();
    EvdY -> Update();
    
    // now, need to read the data files and then perform the fitting (do a plot in between)
    TFile* file = new TFile("/home/philipp/Private/T2K/GUI_indep/MCFiles/WMMC_Run1_1_wNoise_ana.root");
    TTree* tree = (TTree*) file -> Get("tree");
    TBranch* br = tree -> GetBranch("fDefaultReco.");

    // now, read the hits coming from a single event
    IngridEventSummary* evt = new IngridEventSummary();    
    br -> SetAddress(&evt);

    // get the first event here

    int RequestedEvent = 0;
    std::cin >> RequestedEvent;

    std::cout << "got " << RequestedEvent << std::endl;

    while(RequestedEvent != -1)
    {
	tree -> GetEntry(RequestedEvent);
	IngridHitSummary* Hit = new IngridHitSummary();

	std::vector<GEMVector> HitsY;
	std::vector<GEMVector> HitsX;

	for(int i = 0; i < evt -> NIngridHits(); i++)
	{
	    Hit = evt -> GetIngridHit(i);
	
	    // get the hit coordinates (use only one view for the moment)
	    if(Hit -> mod == 15)
	    {
		if(Hit -> view == 0)
		{
		    HitsY.push_back(GEMVector(dim));

		    HitsY.back().SetCoord(0, Hit -> z + GEO_Z_OFFSET);
		    HitsY.back().SetCoord(1, Hit -> xy + GEO_XY_OFFSET);
		}
		else if(Hit -> view == 1)
		{
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
	std::vector<GEMLine> TracksX;
	for(int i = 0; i < gf -> GetNumberLines(); i++)
	{
	    TracksX.push_back(gf -> GetLine(i));
	}

	// perform the Y fitting
	gf -> Reset();

	// add all the points to the fitting object and perform the actual fit
	for(std::vector<GEMVector>::iterator it = HitsY.begin(); it != HitsY.end(); it++)
	{
	    gf -> AddPoint(*it);
	}

	gf -> PerformFit(accuracy, scale);
	
	std::cout << "this is the X dataset for event " << RequestedEvent << std::endl;
	gf -> PrintDataset();

	// read back the fitted tracks
	std::vector<GEMLine> TracksY;
	for(int i = 0; i < gf -> GetNumberLines(); i++)
	{
	    TracksY.push_back(gf -> GetLine(i));
	}

	// now plot everything
	EvdX -> Clear();
	EvdY -> Clear();
    
	EvdX -> PlotHits(HitsX);
	EvdX -> PlotTracks(TracksX);
	EvdX -> Update();    
	
	EvdY -> PlotHits(HitsY);
	EvdY -> PlotTracks(TracksY);
	EvdY -> Update();
	    
	EvdX -> SaveAs(Form("./output/outX-%d.pdf", RequestedEvent));
	EvdY -> SaveAs(Form("./output/outY-%d.pdf", RequestedEvent));

	std::cin >> RequestedEvent;
    }

    std::cout << "stopped by user" << std::endl;

    app.Run();
    return(0);
}
