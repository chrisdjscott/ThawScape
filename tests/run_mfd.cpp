#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "raster.h"
#include "mfd_flow_router.h"
#include "grid_neighbours.h"


int main(int argc, char** argv) {
    std::cout << "Running MFD flow routing" << std::endl;

    // arg parsing, could be done better, may change
    // usage: run_mfd <num_steps> [<topo_file> [<flow_file]] 
    int num_steps;
    if (argc < 2) {
        std::cerr << "Must at least specify the number of steps" << std::endl;
        return 1;
    }
    else {
        num_steps = std::stoi(argv[1]);
        std::cout << "Running for " << num_steps << " steps" << std::endl;
    }
    std::string topo_file("topo.asc");
    std::string flow_file;
    bool have_flow = false;
    if (argc > 2) {
        topo_file = std::string(argv[2]);
    }
    if (argc > 3) {
        flow_file = std::string(argv[3]);
        have_flow = true;
    }

    // load topo
    std::cout << "Topo file = " << topo_file << std::endl;
    Raster topo(topo_file);

    // initial flow
    Raster flow;
    if (have_flow) {
        // load from file
        std::cout << "Flow accumulation file = " << flow_file << std::endl;
        flow = Raster(flow_file);
    }
    else {
        // or set to 1 everywhere
        std::cout << "Setting initial flow accumulation to 1" << std::endl;
        flow = Raster(topo.get_size_x(), topo.get_size_y(), 1);
    }

    // setup grid neighbour indexing
    GridNeighbours nebs;
    nebs.setup(topo.get_size_x(), topo.get_size_y());

    // flow routing object
    MFDFlowRouter flow_router(topo, flow, nebs);
    flow_router.initialise(true);

    // save initial flow
    flow.save("flow000.asc");

    // topo must be sorted before calling mfd and, since it won't change, we sort it once now
    topo.sort_data();

    // main loop
    for (int step = 1; step <= num_steps; step++) {
        std::cout << "Step " << step << std::endl;

        // do the flow routing (TODO: can it just be run by itself with nothing else?)
        flow_router.run();

        // save the flow
        std::ostringstream fname;
        fname << "flow" << std::setfill('0') << std::setw(3) << step << ".asc";
        flow.save(fname.str());
    }

    return 0;
}
