#include <iostream>
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"
#include "EventDisplay.h"
#include "DetectorGeometry.h"
#include "EventWriter.h"
#include "IngridUtils.h"
#include "EventMetrics.h"
#include "EventReferee.h"
#include "TString.h"
#include "TMath.h"

int main(int argc, char** argv)
{  
    std::cout << argc << std::endl;
    if(argc != 3)
    {
	std::cerr << "Error: exactly 2 arguments are required. Call it like ./comp FILE_A FILE_B" << std::endl;
	return(0);
    }

    char* path_a = argv[1];
    char* path_b = argv[2];
    
    TApplication app("app", &argc, argv);

    EventReferee er;

    TFile* file_a = new TFile(path_a, "open");
    TFile* file_b = new TFile(path_b, "open");

    TTree* tree_a = (TTree*) file_a -> Get("tree");
    TTree* tree_b = (TTree*) file_b -> Get("tree");

    int number_events_a = tree_a -> GetEntries();
    int number_events_b = tree_b -> GetEntries();

    std::cout << "found " << number_events_a << " events (A) and " << number_events_b << " events (B)" << std::endl;

    TBranch* br_a = tree_a -> GetBranch("fDefaultReco.");
    TBranch* br_b = tree_b -> GetBranch("fDefaultReco.");

    IngridEventSummary* evt_a = new IngridEventSummary();    
    IngridEventSummary* evt_b = new IngridEventSummary();

    br_a -> SetAddress(&evt_a);
    br_b -> SetAddress(&evt_b);

    // listen for events to compare:
    int RequestedEvent = 0;
    
    // iterate over all events
    while(RequestedEvent < TMath::Min(number_events_a, number_events_b))
    {
	tree_a -> GetEntry(RequestedEvent);
	tree_b -> GetEntry(RequestedEvent);
	
	// compare the two events
	er.CompareEvents(evt_a, evt_b, EW_VIEW_X);
	er.CompareEvents(evt_a, evt_b, EW_VIEW_Y);

	RequestedEvent++;
    }

    er.Report();
    std::cout << "Stopped. Press Ctrl+C to exit." << std::endl;

    app.Run();
    return(0);
}
