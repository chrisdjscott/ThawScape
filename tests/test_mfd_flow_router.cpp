#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "catch2/catch.hpp"
#include "mfd_flow_router.h"
#include "raster.h"
#include "grid_neighbours.h"



TEST_CASE("Flow accumulation simple", "[mfd_flow_router]") {
    std::cout << "Running MFD flow routing" << std::endl;

    std::vector<bool> disable_fa_bounds_options = { true, false };
    for (auto disable_fa_bounds : disable_fa_bounds_options) {
        DYNAMIC_SECTION("with fa bounds disabled: " << disable_fa_bounds) {
            std::vector<std::string> names = { "north", "east", "south", "west" };
            for (auto name : names) {
                DYNAMIC_SECTION("flow to: " << name) {
                    std::cout << "Testing flow: " << name << std::endl;

                    // load topo
                    std::string topo_file = "test_flow_" + name + ".asc";
                    std::cout << "Topo file = " << topo_file << std::endl;
                    Raster topo(topo_file);

                    // initial flow set to 1 everywhere
                    Raster flow;
                    std::cout << "Setting initial flow accumulation to 1" << std::endl;
                    flow = Raster(topo.get_size_x(), topo.get_size_y(), 1);

                    // setup grid neighbour indexing
                    GridNeighbours nebs;
                    nebs.setup(topo.get_size_x(), topo.get_size_y());

                    // flow routing object
                    MFDFlowRouter flow_router(topo, flow, nebs);
                    flow_router.initialise(disable_fa_bounds);

                    // topo must be sorted before calling mfd and, since it won't change, we sort it once now
                    topo.sort_data();

                    // do the flow routing once
                    flow_router.run();

                    // save the flow
                    std::string save_file;
                    if (disable_fa_bounds) {
                        save_file = topo_file + ".faboundsoff.out";
                    }
                    else {
                        save_file = topo_file + ".faboundson.out";
                    }
                    flow.save(save_file);

                    // TODO compare against known good output
                }
            }
        }
    }
}
