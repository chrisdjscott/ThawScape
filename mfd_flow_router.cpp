#include <iostream>
#include <cmath>
#include <vector>
#include "raster.h"
#include "dem.h"
#include "mfd_flow_router.h"


/// References to the input Rasters and vectors are stored since they are expected to be
/// managed elsewhere. One must call the initialise function before running the flow
/// routing.
MFDFlowRouter::MFDFlowRouter(DEM& topo_, Raster& flow_, GridNeighbours& nebs_) :
        topo(topo_), flow(flow_), nebs(nebs_), initialised(false) {
    size_x = topo.get_size_x();
    size_y = topo.get_size_y();
}

void MFDFlowRouter::initialise(const bool no_fa_bounds) {
    size_x = topo.get_size_x();
    size_y = topo.get_size_y();

    std::cout << "no fa_bounds: " << no_fa_bounds << std::endl;

    // FA boundary values; zero otherwise
    flow_incoming = Raster(size_x, size_y, 0.0);
    if (!no_fa_bounds) {
        #pragma omp parallel for
        for (int i = 0; i < size_x; i++)
        {
            flow_incoming(i, 0) = flow(i, 0);
            flow_incoming(i, size_y - 1) = flow(i, size_y - 1);
        }
        #pragma omp parallel for
        for (int j = 0; j < size_y; j++)
        {
            flow_incoming(0, j) = flow(0, j);
            flow_incoming(size_x - 1, j) = flow(size_x - 1, j);
        }
    }

    initialised = true;
}


void MFDFlowRouter::run() {
    // NOTE: assumes topo has been sorted already (i.e. topo.sort_data())

    // make sure initialise was called before proceeding
    if (!initialised) {
        initialise();
    }

    // first apply incoming flow?? (TESTING)
//    #pragma omp parallel for
//    for (int i = 0; i < size_x; i++) {
//        for (int j = 0; j < size_y; j++) {
//            flow(i, j) += flow_incoming(i, j);
//        }
//    }

    // loop over points starting from highest elevation to lowest, calculating the flow from that point to its neighbours
    // note we don't modify the flow at the i,j element that we are processing, only the flow of its lower neighbours gets modified
    int t = size_x * size_y;
    while (t > 0)
    {
        t--;
        int i, j;
        topo.get_sorted_ij(t, i, j);
//        if ( ( i > 3 ) && ( i < (size_x - 3) ) && ( j > 3 ) && ( j < (size_y - 3) ) ) {  // Do not alter boundary elements
            real_type flow1 = 0;
            real_type flow2 = 0;
            real_type flow3 = 0;
            real_type flow4 = 0;
            real_type flow5 = 0;
            real_type flow6 = 0;
            real_type flow7 = 0;
            real_type flow8 = 0;

            // Note that deltax is not used in this computation, so the flow raster represents simply the number of contributing unit cells upstream.
            real_type tot = 0;
            if (topo(i, j) > topo(nebs.iup(i), j)) {
                flow1 = pow(topo(i, j) - topo(nebs.iup(i), j), 1.1);
                tot += flow1;
            }
            if (topo(i, j) > topo(nebs.idown(i), j)) {
                flow2 = pow(topo(i, j) - topo(nebs.idown(i), j), 1.1);
                tot += flow2;
            }
            if (topo(i, j) > topo(i, nebs.jup(j))) {
                flow3 = pow(topo(i, j) - topo(i, nebs.jup(j)), 1.1);
                tot += flow3;
            }
            if (topo(i, j) > topo(i, nebs.jdown(j))) {
                flow4 = pow(topo(i, j) - topo(i, nebs.jdown(j)), 1.1);
                tot += flow4;
            }
            if (topo(i, j) > topo(nebs.iup(i), nebs.jup(j))) {
                flow5 = pow((topo(i, j) - topo(nebs.iup(i), nebs.jup(j)))*oneoversqrt2, 1.1);
                tot += flow5;
            }
            if (topo(i, j) > topo(nebs.iup(i), nebs.jdown(j))) {
                flow6 = pow((topo(i, j) - topo(nebs.iup(i), nebs.jdown(j)))*oneoversqrt2, 1.1);
                tot += flow6;
            }
            if (topo(i, j) > topo(nebs.idown(i), nebs.jup(j))) {
                flow7 = pow((topo(i, j) - topo(nebs.idown(i), nebs.jup(j)))*oneoversqrt2, 1.1);
                tot += flow7;
            }
            if (topo(i, j) > topo(nebs.idown(i), nebs.jdown(j))) {
                flow8 = pow((topo(i, j) - topo(nebs.idown(i), nebs.jdown(j)))*oneoversqrt2, 1.1);
                tot += flow8;
            }

            if (tot != 0) {
                real_type reciptot = 1.0 / tot;
                flow1 *= reciptot;
                flow2 *= reciptot;
                flow3 *= reciptot;
                flow4 *= reciptot;
                flow5 *= reciptot;
                flow6 *= reciptot;
                flow7 *= reciptot;
                flow8 *= reciptot;
            }

//            flow(nebs.iup(i), j) += flow(i, j) * flow1 + fa_bounds(i, j);     // final fa_bounds(i, j) applies only to edges; zero otherwise
//            flow(nebs.idown(i), j) += flow(i, j) * flow2 + fa_bounds(i, j);
//            flow(i, nebs.jup(j)) += flow(i, j) * flow3 + fa_bounds(i, j);
//            flow(i, nebs.jdown(j)) += flow(i, j) * flow4 + fa_bounds(i, j);
//            flow(nebs.iup(i), nebs.jup(j)) += flow(i, j) * flow5 + fa_bounds(i, j);
//            flow(nebs.iup(i), nebs.jdown(j)) += flow(i, j) * flow6 + fa_bounds(i, j);
//            flow(nebs.idown(i), nebs.jup(j)) += flow(i, j) * flow7 + fa_bounds(i, j);
//            flow(nebs.idown(i), nebs.jdown(j)) += flow(i, j) * flow8 + fa_bounds(i, j);
            flow(nebs.iup(i), j) += flow(i, j) * flow1;
            flow(nebs.idown(i), j) += flow(i, j) * flow2;
            flow(i, nebs.jup(j)) += flow(i, j) * flow3;
            flow(i, nebs.jdown(j)) += flow(i, j) * flow4;
            flow(nebs.iup(i), nebs.jup(j)) += flow(i, j) * flow5;
            flow(nebs.iup(i), nebs.jdown(j)) += flow(i, j) * flow6;
            flow(nebs.idown(i), nebs.jup(j)) += flow(i, j) * flow7;
            flow(nebs.idown(i), nebs.jdown(j)) += flow(i, j) * flow8;
//        }
    }
}
