#include "EventReferee.h"

EventReferee::EventReferee()
{
    number_tracks = new TH2D("number_tracks", "number of tracks", 6, -0.5, 5.5, 6, -0.5, 5.5);
    number_tracks -> GetXaxis() -> SetTitle("tracks from cellular automaton");
    number_tracks -> GetYaxis() -> SetTitle("tracks from libgem");
    number_tracks -> SetStats(kFALSE);
    
    single_track_orientation = new TH1D("single_track_orientation", "track orientation", 20, 0, 1);
    single_track_orientation -> GetXaxis() -> SetTitle("cos(d_{1}, d_{2})");
    single_track_orientation -> SetStats(kFALSE);

    vertex_distance = new TH1D("vertex_distance", "vertex distance", 20, 0, 100);
    vertex_distance -> GetXaxis() -> SetTitle("| IP_{1} - IP_{2} |");
    vertex_distance -> SetStats(kFALSE);

    vertex_MC_distance_aux = new TH1D("vertex_MC_distance_aux", "vertex-MC distance", 20, 0, 100);
    vertex_MC_distance_aux -> GetXaxis() -> SetTitle("| IP_{1} - IP_{MC} |");
    vertex_MC_distance_aux -> SetStats(kFALSE);
    vertex_MC_distance_aux -> SetLineColor(kRed);

    vertex_MC_distance_ref = new TH1D("vertex_MC_distance_ref", "vertex-MC distance", 20, 0, 100);
    vertex_MC_distance_ref -> GetXaxis() -> SetTitle("| IP_{1} - IP_{MC} |");
    vertex_MC_distance_ref -> SetStats(kFALSE);
}

void EventReferee::CompareEvents(IngridEventSummary* ref, IngridEventSummary* aux, int view)
{
    // first extract the tracks
    // get all hits and fitted tracks of both files in terms of the same objects!
    std::vector<GEMVector> hits_ref = IngridUtils::GetHits(ref, view);
    std::vector<GEMTrack> tracks_ref = IngridUtils::GetTracks(ref, view);
	
    std::vector<GEMVector> hits_aux = IngridUtils::GetHits(aux, view);
    std::vector<GEMTrack> tracks_aux = IngridUtils::GetTracks(aux, view);

    GEMVector true_IP = IngridUtils::GetTrueVertex(ref, view);
    std::cout << "true IP" << std::endl;
    true_IP.Print();

    number_tracks -> Fill(tracks_ref.size(), tracks_aux.size());

    // for single-track events
    if(tracks_ref.size() == 1 && tracks_aux.size() == 1)
    {
	// ... compare them according to the relative orientation of the fitted tracks
	single_track_orientation -> Fill(EventMetrics::TrackOrientation(tracks_ref.at(0), tracks_aux.at(0)));
    }

    // for double-track events
    if(tracks_ref.size() == 2 && tracks_aux.size() == 2)
    {
	// ... compare them according to the positions of the two vertices (as the intersection points of the two lines)
	vertex_distance -> Fill(EventMetrics::VertexDistance(tracks_ref, tracks_aux));

	// compare the position of the libgem vertex to the true vertex
	vertex_MC_distance_aux -> Fill(EventMetrics::VertexDistanceToPoint(tracks_aux, true_IP));
	vertex_MC_distance_ref -> Fill(EventMetrics::VertexDistanceToPoint(tracks_ref, true_IP));
    }

    /*
      Add your own tests and comparisons here! It is suggested to add new lower-level functions to the EventMetrics class, to keep the code tidy.
     */
}

void EventReferee::Report()
{
    TCanvas* track_orientation_canv = new TCanvas("track_orientation_canv", "Single track events: track orientation", 800, 600);
    track_orientation_canv -> cd();
    single_track_orientation -> Draw();

    TCanvas* vertex_distance_canv = new TCanvas("vertex_distance_canv", "Vertex distance", 800, 600);
    vertex_distance_canv -> cd();
    vertex_distance -> Draw();

    TCanvas* vertex_MC_distance_canv = new TCanvas("vertex_MC_canv", "Vertex-MC distance", 800, 600);
    vertex_MC_distance_canv -> cd();
    vertex_MC_distance_aux -> Draw();
    vertex_MC_distance_ref -> Draw("same");

    TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
    leg -> AddEntry("vertex_MC_distance_aux", "MC - libgem", "l");
    leg -> AddEntry("vertex_MC_distance_ref", "MC - cellular automaton", "l");
    leg -> Draw();

    TCanvas* number_tracks_canv = new TCanvas("number_tracks_canv", "number of tracks", 800, 600);
    number_tracks_canv -> cd();
    number_tracks -> Draw("colz");

    vertex_MC_distance_canv -> cd();
    vertex_MC_distance_canv -> SaveAs("vertex_MC_distance.pdf");

    track_orientation_canv -> cd();
    track_orientation_canv -> SaveAs("track_orientation.pdf");

    vertex_distance_canv -> cd();
    vertex_distance_canv -> SaveAs("vertex_distance.pdf");

    number_tracks_canv -> cd();
    number_tracks_canv -> SaveAs("number_tracks.pdf");

    std::cout << "done reporting" << std::endl;
}
