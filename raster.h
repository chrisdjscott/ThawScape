#ifndef _RASTER_HPP_
#define _RASTER_HPP_

#include <vector>
#include <string>
#include "global_defs.h"

class Raster {
    private:
        int size_x;  ///< x dimension of the raster
        int size_y;  ///< y dimension of the raster
        std::vector<real_type> data;  ///< Underlying data of the raster
        real_type xllcorner;  ///< x coordinate of lower left corner
        real_type yllcorner;  ///< y coordinate of lower left corner
        real_type deltax;  ///< Grid resolution
        real_type nodata;  ///< The value that represents nodata
        std::vector<int> idx;  ///< Vector of indexes, used for sorting raster data by value

    public:
        /// \brief Create an empty Raster object
        Raster();

        /// \brief Create an empty Raster object with the given dimensions
        /// \param size_x_ x dimension of the raster
        /// \param size_y_ y dimension of the raster
        Raster(int size_x_, int size_y_);

        /// \brief Create a Raster object with the given dimensions and all elements initialised to the same value
        /// \param size_x_ x dimension of the raster
        /// \param size_y_ y dimension of the raster
        /// \param value The value to initialise the raster with
        Raster(int size_x_, int size_y_, real_type value);

        /// \brief Create a Raster object by loading it from a file
        /// \param filename The name of the file to load the raster from
        Raster(const std::string &filename);

        /// \brief Access the underlying data using (i, j) notation (const)
        /// \param i First index of data element to access
        /// \param j Second index of data element to access
        const real_type& operator()(int i, int j) const;

        /// \brief Access the underlying data using (i, j) notation
        /// \param i First index of data element to access
        /// \param j Second index of data element to access
        real_type& operator()(int i, int j);

        /// \brief Destructively resize the raster
        /// \param size_x_ New x dimension
        /// \param size_y_ New y dimension
        void resize(int size_x_, int size_y_);

        /// \brief Destructively load raster from file
        /// \param filename The name of the file to load the raster from
        void load(const std::string &filename);

        /// \brief Save the raster to file
        /// \param filename The name of the file to save the raster to
        void save(const std::string &filename);

        /// \brief Set all elements of the raster to the given value
        /// \param value Set all elements of the raster to this value
        void set_data(real_type value);

        /// \brief Check if an element in the raster is set to nodata
        /// \param i First index of the element to check
        /// \param j Second index of the element to check
        bool is_nodata(int i, int j);

        /// \brief Set the given element to nodata
        /// \param i First index of the element to set
        /// \param j Second index of the element to set
        void set_nodata(int i, int j);

        /// \brief Create a list of indexes of the data sorted by value (low to high)
        void sort_data();

        /// \brief Find the indices of the specified element from the ordered data values
        /// \param t Find the t'th element from the ordered data values
        /// \param i Set to the first index of the t'th element
        /// \param j Set to the second index of the t'th element
        void get_sorted_ij(int t, int &i, int &j);

        /// \brief Get the size of the raster in the x dimension
        real_type get_size_x() { return size_x; }

        /// \brief Get the size of the raster in the y dimension
        real_type get_size_y() { return size_y; }

        /// \brief Get the x coordinate of the lower left corner
        real_type get_xllcorner() { return xllcorner; }

        /// \brief Get the y coordinate of the lower left corner
        real_type get_yllcorner() { return yllcorner; }

        /// \brief Get the grid resolution
        real_type get_deltax() { return deltax; }

        /// \brief Get the nodata value
        real_type get_nodata() { return nodata; }
};

#endif
