#include "EventMetrics.h"

double EventMetrics::TrackOrientation(GEMTrack a, GEMTrack b)
{
    GEMVector dir_a = a.GetLine().GetDirVect();
    GEMVector dir_b = b.GetLine().GetDirVect();

    // return the normalized scalar product of the two direction vectors
    return TMath::Abs(dir_a * dir_b / (TMath::Sqrt(dir_a * dir_a) * TMath::Sqrt(dir_b * dir_b)));
}

double EventMetrics::VertexDistance(std::vector<GEMTrack> a, std::vector<GEMTrack> b)
{
    // compute first the intersection points of the two tracks contained in a and b, separately
    GEMVector ip_a = GEMLine::IntersectionPoint2D(a.at(0).GetLine(), a.at(1).GetLine());
    GEMVector ip_b = GEMLine::IntersectionPoint2D(b.at(0).GetLine(), b.at(1).GetLine());

    a.at(0).GetLine().Print();
    b.at(0).GetLine().Print();

    ip_a.Print();
    ip_b.Print();
    
    double retval = GEMVector::EuclideanDistance(ip_a, ip_b);

    std::cout << "vertex distance = " << retval << std::endl;
    
    return retval;
}
