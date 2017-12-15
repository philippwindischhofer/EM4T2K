#include "EventMetrics.h"

double EventMetrics::TrackOrientation(GEMTrack a, GEMTrack b)
{
    GEMVector dir_a = a.GetLine().GetDirVect();
    GEMVector dir_b = b.GetLine().GetDirVect();

    // return the normalized scalar product of the two direction vectors
    return TMath::Abs(dir_a * dir_b / (TMath::Sqrt(dir_a * dir_a) * TMath::Sqrt(dir_b * dir_b)));
}
