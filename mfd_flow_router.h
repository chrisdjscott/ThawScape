#ifndef _MFD_FLOW_ROUTE_
#define _MFD_FLOW_ROUTE_

#include <vector>
#include "raster.h"
#include "grid_neighbours.h"
#include "dem.h"
#include "global_defs.h"

/// \brief Multiple flow direction flow routing
class MFDFlowRouter {
    private:
        int size_x;  ///< Number of cells in the x dimension
        int size_y;  ///< Number of cells in the y dimension
        DEM& topo;  ///< Raster of elevations
        Raster& flow;  ///< Flow accumulation Raster
        GridNeighbours& nebs;  ///< Grid neighbour indexing
        Raster flow_incoming;  ///< Raster for flow coming in at the boundaries
        bool initialised;

    public:
        /// \brief Create an MFDFlowRouter object and initialise it (this interface may change)
        /// \param topo_ The Raster of elevations
        /// \param flow_ The flow accumulation Raster that will contain the initial flow values
        /// \param nebs_ GridNeighbours instance for neighbour indexing
        MFDFlowRouter(DEM& topo_, Raster& flow_, GridNeighbours& nebs_);

        /// \brief Initialise the MFDFlowRouter object
        /// \param no_incoming_flow If set to true then there will be no incoming flow at the boundaries
        void initialise(const bool no_incoming_flow = false);
        
        /// \brief Do the flow routing
        void run();
};

#endif
