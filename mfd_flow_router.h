#ifndef _MFD_FLOW_ROUTE_
#define _MFD_FLOW_ROUTE_

#include <vector>
#include "raster.h"
#include "grid_neighbours.h"
#include "global_defs.h"

/// \brief Multiple flow direction flow routing
class MFDFlowRouter {
    private:
        int size_x;  ///< Number of cells in the x dimension
        int size_y;  ///< Number of cells in the y dimension
        Raster& topo;  ///< Raster of elevations
        Raster& flow;  ///< Flow accumulation Raster
        GridNeighbours& nebs;  ///< Grid neighbour indexing
        Raster flow1;  // these can probably be removed...
        Raster flow2;
        Raster flow3;
        Raster flow4;
        Raster flow5;
        Raster flow6;
        Raster flow7;
        Raster flow8;
        Raster fa_bounds;  ///< Raster for flow coming in at the boundaries

    public:
        /// \brief Create an MFDFlowRouter object and initialise it (this interface may change)
        /// \param topo_ The Raster of elevations
        /// \param flow_ The flow accumulation Raster that will contain the initial flow values
        /// \param nebs_ GridNeighbours instance for neighbour indexing
        MFDFlowRouter(Raster& topo_, Raster& flow_, GridNeighbours& nebs_);

        /// \brief Initialise the MFDFlowRouter object
        void initialise();
        
        /// \brief Do the flow routing
        void run();
};

#endif
