#include "IngridUtils.h"

std::vector<GEMVector> IngridUtils::GetHits(IngridEventSummary* evt, int view)
{
    std::vector<GEMVector> retval;

    int number_hits = evt -> NIngridHits();
    IngridHitSummary* hit;

    for(int i = 0; i < number_hits; i++)
    {
	hit = evt -> GetIngridHit(i);
	if(hit -> view == view && hit -> mod == 15)
	{
	    // get new 2-dim vector
	    GEMVector cur(2);
	    cur.SetCoord(1, hit -> xy + GEO_XY_OFFSET);
	    cur.SetCoord(0, hit -> z + GEO_Z_OFFSET);

	    retval.push_back(cur);
	}
    }
    
    return retval;
}

std::vector<GEMLine> IngridUtils::GetTracks(IngridEventSummary* evt, int view)
{
    std::vector<GEMLine> retval;

    // look only at 2D tracks
    int number_tracks = evt -> NPMRecons();

    for(int i = 0; i < number_tracks; i++)
    {
	PMReconSummary* recon = evt -> GetPMRecon(i);
	if(recon -> view == view && recon -> hitmod == 15)
	{
	    GEMVector start(2);
	    GEMVector end(2);

	    start.SetCoord(1, (recon -> startxy) / EV_XY_SCALE - EV_XY_OFFSET);
	    end.SetCoord(1, (recon -> endxy) / EV_XY_SCALE - EV_XY_OFFSET);

	    start.SetCoord(0, (recon -> startz) / EV_Z_SCALE - EV_Z_OFFSET);
	    end.SetCoord(0, (recon -> endz) / EV_Z_SCALE - EV_Z_OFFSET);

	    std::cout << "view = " << view << std::endl;
	    std::cout << "recon -> startxy = " << recon -> startxy << std::endl;
	    std::cout << "recon -> startz = " << recon -> startz << std::endl;
	    std::cout << "start.x = " << start.GetCoord(0) << std::endl;
	    std::cout << "start.z = " << start.GetCoord(1) << std::endl;

	    GEMLine cur(start, end - start);

	    retval.push_back(cur);
	}
    }
    
    return retval;
}