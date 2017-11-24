#include <iostream>
#include <TApplication.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLine.h>
#include <TRandom1.h>
#include "GEMFitter.h"

int main(int argc, char** argv)
{
    TApplication app("app", &argc, argv);
    
    // make fitter object
    int dim = 2;
    double accuracy = 0.00001;
    double scale = 13;
    
    GEMFitter* gf = new GEMFitter(dim);

    int num_points = 25;
    std::vector<GEMVector> hits;

    TRandom1* gen = new TRandom1();
    
    // prepare a couple of points
    for(int i = 0; i < num_points; i++)
    {
	hits.push_back(GEMVector(dim));

	double mu = 0;
	double x = 1.0;
		
	if(gen -> Rndm() < 0.5)
	{
	    mu = 0.25;
	}
	else
	{
	    mu = 0.75;
	}

	if(gen -> Rndm() < 0.5)
	{
	    x = 1.0;
	}
	else
	{
	    x = -1.0;
	}

	double ov =  gen -> Gaus(mu, 0.005);
	hits[i].SetCoord(0, ov);
	hits[i].SetCoord(1, gen -> Rndm());
	//hits[i].SetCoord(1, x * ov);
    }
    
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
    g -> GetXaxis() -> SetRangeUser(-1., 1.0);
    g -> GetYaxis() -> SetRangeUser(-1., 1.0);
    g -> Draw("A*");
    canv -> Update();

    
    for(std::vector<GEMLine>::iterator it = tracks.begin(); it != tracks.end(); it++)
    {
	GEMVector start = (*it).GetRefPoint() - (*it).GetDirVect();
	GEMVector end = (*it).GetRefPoint() + (*it).GetDirVect();
	TLine* line = new TLine(start.GetCoord(0), start.GetCoord(1), end.GetCoord(0), end.GetCoord(1));
	line -> Draw();
    }

    canv -> SaveAs("out.pdf");
    app.Run();
    
    return(0);
}
