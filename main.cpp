#include <iostream>
#include "RootIncludes.h"
#include "IngridIncludes.h"
#include "GEMFitter.h"

int main(int argc, char** argv)
{
    TApplication app("app", &argc, argv);
    
    // make fitter object
    int dim = 2;
    double accuracy = 0.00001;
    double scale = 50;
    
    GEMFitter* gf = new GEMFitter(dim);

    std::vector<GEMVector> hits;
    
    // now, need to read the data files and then perform the fitting (do a plot in between)
    TFile* file = new TFile("/home/philipp/Private/T2K/GUI_indep/MCFiles/WMMC_Run1_1_wNoise_ana.root");
    TTree* tree = (TTree*) file -> Get("tree");
    TBranch* br = tree -> GetBranch("fDefaultReco.");

    // now, read the hits coming from a single event
    IngridEventSummary* evt = new IngridEventSummary();    
    br -> SetAddress(&evt);

    // get the first event here
    tree -> GetEntry(206);

    int nHits = evt -> NIngridHits();
    std::cout << "have " << nHits << " hits" << std::endl;
    IngridHitSummary* Hit = new IngridHitSummary();
  
    for(int i = 0; i < nHits; i++)
    {
	Hit = evt -> GetIngridHit(i);
	
	// get the hit coordinates (use only one view for the moment)
	if(Hit -> mod == 15)
	{
	    if(Hit -> view == 0)
	    {
		hits.push_back(GEMVector(dim));

		hits.back().SetCoord(0, Hit -> z);
		hits.back().SetCoord(1, Hit -> xy);
	    }
	}
    }

    std::cout << "retained " << hits.size() << " hits" << std::endl;
    
    // add all the points to the fitting object
    for(std::vector<GEMVector>::iterator it = hits.begin(); it != hits.end(); it++)
    {
	gf -> AddPoint(*it);
    }

    gf -> PerformFit(accuracy, scale);

    std::vector<GEMLine> tracks;
    for(int i = 0; i < gf -> GetNumberLines(); i++)
    {
	tracks.push_back(gf -> GetLine(i));
    }

    // now have all the points and all extracted tracks in a nice way, plot them together into a TGraph
    TCanvas* canv = new TCanvas("canv", "canv", 800, 600);
    TGraph* g = new TGraph();
    
    for(std::vector<GEMVector>::iterator it = hits.begin(); it != hits.end(); it++)
    {
	g -> SetPoint(g -> GetN(), (*it).GetCoord(0), (*it).GetCoord(1));
    }
    
    canv -> cd();
    g -> Draw("A*");    
    canv -> Update();
    
    for(std::vector<GEMLine>::iterator it = tracks.begin(); it != tracks.end(); it++)
    {
	// TODO: find scale factor for line automatically
	GEMVector start = (*it).GetRefPoint() - 100. * ((*it).GetDirVect());
	GEMVector end = (*it).GetRefPoint() + 100. * ((*it).GetDirVect());
	TLine* line = new TLine(start.GetCoord(0), start.GetCoord(1), end.GetCoord(0), end.GetCoord(1));
	line -> Draw();
    }

    canv -> SaveAs("out.pdf");
    app.Run();
    
    return(0);
}
