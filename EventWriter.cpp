#include "EventWriter.h"

EventWriter::EventWriter(std::string file)
{
    file_out = new TFile(file.c_str(), "recreate");
    tree_out = new TTree("tree", "tree"); // tree that will hold the output data
    
    // create a new branch in the output tree
    evt_out = new IngridEventSummary();
    tree_out -> Branch("fDefaultReco.", "IngridEventSummary", &evt_out, 64000, 99);
}

EventWriter::~EventWriter()
{
    file_out -> Write();
    file_out -> Close();
}

void EventWriter::SetEvent(IngridEventSummary* evt)
{
    // need to prepare the event: change from convention cyc = -1 --> cyc = 4 for the MC files
    for(int i = 0; i < evt -> NIngridHits(); i++)
    {
	if(evt -> GetIngridHit(i) -> cyc == -1)
	    evt -> GetIngridHit(i) -> cyc = 4;
    }

    // now the convention should be restored
    evt_out = new IngridEventSummary(*evt);
}

void EventWriter::AddTrack(GEMTrack track, int view, IngridEventSummary* evt)
{
    GEMLine line = track.GetLine();
    std::vector<GEMVector> hits = track.GetHits();

    recon = new PMReconSummary();
    
    recon -> Clear();

    GEMVector start = line.GetRefPoint() - 100. * (line.GetDirVect());
    GEMVector end = line.GetRefPoint() + 100. * (line.GetDirVect());
    
    recon -> view = view;

    recon -> startxy = (start.GetCoord(1) + EV_XY_OFFSET) * EV_XY_SCALE;
    recon -> endxy = (end.GetCoord(1) + EV_XY_OFFSET) * EV_XY_SCALE;
    
    recon -> startz = (start.GetCoord(0) + EV_Z_OFFSET) * EV_Z_SCALE;
    recon -> endz = (end.GetCoord(0) + EV_Z_OFFSET) * EV_Z_SCALE;

    std::cout << "view = " << view << std::endl;
    std::cout << "recon -> startxy = " << recon -> startxy << std::endl;
    std::cout << "recon -> startz = " << recon -> startz << std::endl;
    std::cout << "start.x = " << start.GetCoord(0) << std::endl;
    std::cout << "start.z = " << start.GetCoord(1) << std::endl;

    // ---------------------------------------------
    // these are still dummy values!
    recon -> startpln = 0;
    recon -> endpln = 5;
    recon -> angle = 0.0;
    recon -> slope = 0.0;
    recon -> intcpt = 20.0;
    recon -> vetowtracking = 0.0;
    recon -> edgewtracking = 0.0;
    recon -> modfc = 0.0;
    recon -> vetodist = 0.0;
    // ---------------------------------------------

    // to make it into an INGRID track
    recon -> hitmod = 15;

    /*
      The following code block is meant to add hits to the track they belong to. In this case, studies would be possible that don't only work on the level of a track, but can also take into account the hits that were associated to this track.
      Libgem provides a very natural way to find the hits for a fitted track, however there is a problem with saving the information to the ROOT tree, and so this feature is not available at present.
     */
    
    /*
    std::cout << "trying to refind INGRID hits" << std::endl;
    
    // now add all the associated hits to the PMRecon object
    for(int i = 0; i < hits.size(); i++)
    {
	
	// IngridHitSummary* cur = new IngridHitSummary();

	// again, add only the very basic info at this point, i.e. x / y and z positions
	// cur -> view = view;
	// cur -> xy = hits.at(i).GetCoord(0);
	// cur -> z = hits.at(i).GetCoord(1);
	// cur -> mod = 15;
	// recon -> AddIngridHit(cur);

	// workaround: instead of creating a new IngridHitSummary object that holds this hit (which somehow doesn't work for very weird reasons), search for the corresponding, already existing IngridHitSummary in the event and it it instead
	IngridHitSummary* existing_hit;

	std::cout << "looking for " << hits.at(i).GetCoord(0) << " / " << hits.at(i).GetCoord(1) << " in " << evt -> NIngridHits() << " hits" << std::endl;

	for(int j = 0; j < evt -> NIngridHits(); j++)
	{
	    // load the next hit in the event
	    existing_hit = evt -> GetIngridHit(j);

	    if((existing_hit -> mod == 15) && (existing_hit -> time < 20))
	    {
		std::cout << "currently looking at " << j << " : " << (existing_hit -> xy + GEO_XY_OFFSET) << " / " << (existing_hit -> z + GEO_Z_OFFSET) << std::endl;
	    
		// check if it matches the coordinates of the hit that is meant to be added
		if((existing_hit -> view == view) && (existing_hit -> xy + GEO_XY_OFFSET == hits.at(i).GetCoord(1)) && (existing_hit -> z + GEO_Z_OFFSET == hits.at(i).GetCoord(0)))
		{
		    std::cout << "corresponding hit was recovered!" << std::endl;
		    break;
		}
	    }
	}

	std::cout << "searched all" << std::endl;

	// now add the already existing hit to the object
	recon -> AddIngridHit(existing_hit);
    }
    */
    
    evt_out -> AddPMModRecon(recon, 15, 4, view);    
}

void EventWriter::WriteEvent()
{
    tree_out -> Fill();
}
