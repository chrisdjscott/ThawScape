#ifndef _STREAMPOWER_H_
#define _STREAMPOWER_H_

#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include "global_defs.h"
#include "Array2D.hpp"
#include "model_time.h"
#include "raster.h"

#define NR_END 1
#define FREE_ARG char*

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

#define SWAP(a,b) itemp=(a);(a)=(b);(b)=itemp;
//#define M 7
#define NSTACK 100000

#define sqrt2 1.414213562373f
#define oneoversqrt2 0.707106781186f
#define degrad 0.01745329251994330   // Convert degrees to radians; e.g. 180 * degrad = 3.14159..
#define PI 3.14159265358979
#define HALFPI = PI/2
#define fillincrement 0.01f


class solar_geom {
public:
	real_type lattitude;
	real_type longitude;
	real_type stdmed;          // LSTM = (UTC - 7H * 15 deg)
	real_type declination;     // Declination of sun from equatorial plane
	real_type altitude;        // Sun altitude in the sky
	real_type azimuth;         // Compass angle of sun
	real_type incidence;       // Angle of sun's incidence
	real_type SHA;             // Solar Hour Angle is 0� at solar noon.  Since the Earth rotates 15� per hour,
		            // each hour away from solar noon corresponds to an angular motion of the sun in the sky of 15�.
		            // In the morning the hour angle is negative, in the afternoon the hour angle is positive.
};

class StreamPower
{
public:

	int lattice_size_x, lattice_size_y, duration, printinterval, printstep;
	real_type U, K, D, melt, timestep, ann_timestep, deltax, deltax2, thresh, thresh_diag, thresholdarea;
	real_type init_exposure_age, init_sed_track, init_veg;

	// new vars
	real_type xllcorner, yllcorner, nodata;
	std::vector<int> iup, idown, jup, jdown;
	std::vector<real_type> ax, ay, bx, by, cx, cy, ux, uy, rx, ry;
    Raster topo, topoold, slope, aspect;
    Raster flow, flow1, flow2, flow3, flow4, flow5, flow6, flow7, flow8, FA_Bounds;
	Raster veg, veg_old, Sed_Track, ExposureAge, ExposureAge_old;
	Raster solar_raster, shade_raster, I_D, I_R, I_P, N_Ip, E_Ip, S_Ip, W_Ip, NE_Ip, SE_Ip, SW_Ip, NW_Ip;
	Raster Ip_D8;     // Map of incoming solar flux, 8 directions
	Array2D<real_type> elevation;

	ModelTime ct;             // Current model time
	solar_geom r;

	static std::vector<real_type> Vector(int nl, int nh);
	static std::vector<int> IVector(int nl, int nh);
	static std::vector<std::vector<real_type>> Matrix(int nrl, int nrh, int ncl, int nch);			
	static std::vector<std::vector<int>> IMatrix(int nrl, int nrh, int ncl, int nch);	

    bool fix_random_seed;
	static real_type Ran3(std::default_random_engine& generator, std::uniform_real_distribution<real_type>& distribution);
	static real_type Gasdev(std::default_random_engine& generator, std::normal_distribution<real_type>& distribution);

	static void Tridag(real_type a[], real_type b[], real_type c[], real_type r[], real_type u[], unsigned long n); // interface from old to new implementation
	static void Tridag(std::vector<real_type>& a, std::vector<real_type>& b, std::vector<real_type>& c, std::vector<real_type>& r, std::vector<real_type>& u, int n); // new implementation

	StreamPower(int nx, int ny);
	~StreamPower();

	std::vector<std::vector<real_type>> CreateRandomField();
	Raster GetTopo();


	void SetupGridNeighbors();
	void SetTopo();
	void SetFA();
	void Flood(); // Barnes pit filling
	void MFDFlowRoute(int i, int j); //new implementation
	void InitDiffusion();
	void HillSlopeDiffusion();
	void Avalanche(int i, int j);
	void SlopeAspect(int i, int j);
	void SunPosition();
	void SolarInflux();
	void MeltExposedIce(int i, int j);

	void Init(std::string parameter_file); // using new vars
    void LoadInputs();
	void Start();

    std::string topo_file, fa_file, sed_file;
};

template <typename T> std::vector<T> ArrayToVector(T* a, int size)
{
	std::vector<T> v = std::vector<T>(size);
	for (int i = 0; i < size; i++)
	{
		v[i] = a[i];
	}
	return v;
}

template <typename T> std::vector<T> ArrayToVector(T* a, int size, bool fortranIndexing)
{
	std::vector<T> v;
	if (fortranIndexing)
	{
		v = std::vector<T>(size + 1);
	}
	else 
	{
		v = std::vector<T>(size);
	}

	for (int i = 0; i < size; i++)
	{
		v[i] = a[i];
	}
	return v;
}

template <typename T> void VectorToArray(std::vector<T>& v, T* a)
{
	for (int i = 0; i < v.size(); i++)
	{
		a[i] = v[i];
	}
}

// http://stackover_flow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes
template <typename T> std::vector<int> SortIndices(const std::vector<T>& v)
{

	// initialize original index locations
	std::vector<int> idx(v.size());
	std::iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

	return idx;
}

template <typename T> std::vector<int> SortFortranIndices(const std::vector<T>& v)
{

	// initialize original index locations
	std::vector<int> idx(v.size());
	std::iota(idx.begin()+1, idx.end(), 1);

	// sort indexes based on comparing values in v
	std::sort(idx.begin()+1, idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

	return idx;
}

#endif
