#ifndef __INGRID_UTILS_H
#define __INGRID_UTILS_H

#include "IngridIncludes.h"
#include "EventWriter.h"
#include "GEMFitter.h"
#include "DetectorGeometry.h"

class IngridUtils
{
public:
    static std::vector<GEMVector> GetHits(IngridEventSummary* evt, int view);
    static std::vector<GEMLine> GetTracks(IngridEventSummary* evt, int view);
};

#endif
