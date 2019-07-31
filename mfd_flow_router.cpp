#include <cmath>
#include <vector>
#include "raster.h"
#include "mfd_flow_router.h"

MFDFlowRouter::MFDFlowRouter() : size_x(0), size_y(0) {

}


void MFDFlowRouter::initialise(Raster& initial_flow) {
    size_x = initial_flow.get_size_x();
    size_y = initial_flow.get_size_y();

    // flow Rasters
    flow1 = Raster(size_x, size_y);
    flow2 = Raster(size_x, size_y);
    flow3 = Raster(size_x, size_y);
    flow4 = Raster(size_x, size_y);
    flow5 = Raster(size_x, size_y);
    flow6 = Raster(size_x, size_y);
    flow7 = Raster(size_x, size_y);
    flow8 = Raster(size_x, size_y);

    // FA boundary values; zero otherwise
    fa_bounds = Raster(size_x, size_y, 0.0);
	for (int i = 0; i < size_x; i++)
	{
        fa_bounds(i, 0) = initial_flow(i, 0);
        fa_bounds(i, size_y - 1) = initial_flow(i, size_y - 1);
	}
    for (int j = 0; j < size_y; j++)
    {
        fa_bounds(0, j) = initial_flow(0, j);
        fa_bounds(size_x - 1, j) = initial_flow(size_x - 1, j);
    }
}


// interface will change...
void MFDFlowRouter::run(Raster& topo, Raster& flow, std::vector<int>& iup, std::vector<int>& idown,
        std::vector<int>& jup, std::vector<int>& jdown) {
    // assumes topo has been sorted already

    int t = size_x * size_y;
    while (t > 0)
    {
        t--;
        int i, j;
        topo.get_sorted_ij(t, i, j);
        if ( ( i > 3 ) && ( i < (size_x - 3) ) && ( j > 3 ) && ( j < (size_y - 3) ) ) {  // Do not alter boundary elements
            real_type tot;

            // Note that deltax is not used in this computation, so the flow raster represents simply the number of contributing unit cells upstream.
            tot = 0;
            if (topo(i, j) > topo(iup[i], j))
                tot += pow(topo(i, j) - topo(iup[i], j), 1.1);
            if (topo(i, j) > topo(idown[i], j))
                tot += pow(topo(i, j) - topo(idown[i], j), 1.1);
            if (topo(i, j) > topo(i, jup[j]))
                tot += pow(topo(i, j) - topo(i, jup[j]), 1.1);
            if (topo(i, j) > topo(i, jdown[j]))
                tot += pow(topo(i, j) - topo(i, jdown[j]), 1.1);
            if (topo(i, j) > topo(iup[i], jup[j]))
                tot += pow((topo(i, j) - topo(iup[i], jup[j]))*oneoversqrt2, 1.1);
            if (topo(i, j) > topo(iup[i], jdown[j]))
                tot += pow((topo(i, j) - topo(iup[i], jdown[j]))*oneoversqrt2, 1.1);
            if (topo(i, j) > topo(idown[i], jup[j]))
                tot += pow((topo(i, j) - topo(idown[i], jup[j]))*oneoversqrt2, 1.1);
            if (topo(i, j) > topo(idown[i], jdown[j]))
                tot += pow((topo(i, j) - topo(idown[i], jdown[j]))*oneoversqrt2, 1.1);

            if (topo(i, j) > topo(iup[i], j))
                flow1(i, j) = pow(topo(i, j) - topo(iup[i], j), 1.1) / tot;
            else flow1(i, j) = 0;
            if (topo(i, j) > topo(idown[i], j))
                flow2(i, j) = pow(topo(i, j) - topo(idown[i], j), 1.1) / tot;
            else flow2(i, j) = 0;
            if (topo(i, j) > topo(i, jup[j]))
                flow3(i, j) = pow(topo(i, j) - topo(i, jup[j]), 1.1) / tot;
            else flow3(i, j) = 0;
            if (topo(i, j) > topo(i, jdown[j]))
                flow4(i, j) = pow(topo(i, j) - topo(i, jdown[j]), 1.1) / tot;
            else flow4(i, j) = 0;
            if (topo(i, j) > topo(iup[i], jup[j]))
                flow5(i, j) = pow((topo(i, j) - topo(iup[i], jup[j]))*oneoversqrt2, 1.1) / tot;
            else flow5(i, j) = 0;
            if (topo(i, j) > topo(iup[i], jdown[j]))
                flow6(i, j) = pow((topo(i, j) - topo(iup[i], jdown[j]))*oneoversqrt2, 1.1) / tot;
            else flow6(i, j) = 0;
            if (topo(i, j) > topo(idown[i], jup[j]))
                flow7(i, j) = pow((topo(i, j) - topo(idown[i], jup[j]))*oneoversqrt2, 1.1) / tot;
            else flow7(i, j) = 0;
            if (topo(i, j) > topo(idown[i], jdown[j]))
                flow8(i, j) = pow((topo(i, j) - topo(idown[i], jdown[j]))*oneoversqrt2, 1.1) / tot;
            else flow8(i, j) = 0;

            flow(iup[i], j) += flow(i, j) * flow1(i, j) + fa_bounds(i, j);     // final fa_bounds(i, j) applies only to edges; zero otherwise
            flow(idown[i], j) += flow(i, j) * flow2(i, j) + fa_bounds(i, j);
            flow(i, jup[j]) += flow(i, j) * flow3(i, j) + fa_bounds(i, j);
            flow(i, jdown[j]) += flow(i, j) * flow4(i, j) + fa_bounds(i, j);
            flow(iup[i], jup[j]) += flow(i, j) * flow5(i, j) + fa_bounds(i, j);
            flow(iup[i], jdown[j]) += flow(i, j) * flow6(i, j) + fa_bounds(i, j);
            flow(idown[i], jup[j]) += flow(i, j) * flow7(i, j) + fa_bounds(i, j);
            flow(idown[i], jdown[j]) += flow(i, j) * flow8(i, j) + fa_bounds(i, j);
        }
    }
}
