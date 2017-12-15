#ifndef __DETECTOR_GEOMETRY_H
#define __DETECTOR_GEOMETRY_H

#define GEO_XY_OFFSET 50.
#define GEO_Z_OFFSET 22.8
#define VERTEX_XY_OFFSET GEO_XY_OFFSET
#define VERTEX_Z_OFFSET (120 + GEO_Z_OFFSET)

// holds the positions of the box grid
#define PATH_BOXGRID "./GeomBoxgrid.dat"
#define PATH_X_SCINT "./GeomXScint.dat"
#define PATH_Y_SCINT "./GeomYScint.dat"

#define PATH_X_BINS "./GeomXBins.dat"
#define PATH_Z_BINS "./GeomZBins.dat"

#endif
