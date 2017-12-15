#include "EventReferee.h"

EventReferee::EventReferee()
{
    single_track_orientation = new TH1D("single_track_orientation", "track orientation", 20, 0, 1);
    single_track_orientation -> SetStats(kFALSE);

    vertex_distance = new TH1D("vertex_distance", "vertex distance", 20, 0, 1);
    vertex_distance -> SetStats(kFALSE);
}

void EventReferee::CompareEvents(std::vector<GEMTrack> ref, std::vector<GEMTrack> aux)
{
    // sort the events according to their type:

    // for single-track events
    if(ref.size() == 1 && aux.size() == 1)
    {
	// ... compare them according to the relative orientation of the fitted tracks
	single_track_orientation -> Fill(EventMetrics::TrackOrientation(ref.at(0), aux.at(0)));
    }

    // for double-track events
    if(ref.size() == 2 && aux.size() == 2)
    {
	// ... compare them according to the positions of the two vertices (as the intersection points of the two lines)
    }
}

void EventReferee::Report()
{
    TCanvas* track_orientation_canv = new TCanvas("track_orientation_canv", "Single track events: track orientation", 800, 600);
    track_orientation_canv -> cd();
    single_track_orientation -> Draw();

    TCanvas* vertex_distance_canv = new TCanvas("vertex_distance_canv", "Vertex distance", 800, 600);
    vertex_distance_canv -> cd();
    vertex_distance -> Draw();
}
