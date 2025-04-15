#include "libs/TleParser.h"
#include "libs/OrbMaths.h"
#include "libs/TleFiles.h"
#include "libs/TermFuncs.h"
#include "libs/Algos.h"
#include "libs/Kepler.h"
#include "libs/Ellipses.h"

#include "libs/Common.h"

typedef struct Params {
	double MeanMotion;
	double MeanAnomaly;
	double ArgPeri;
	double AscNodeLong;
	double Inclination;
	double Eccentricity;

	double SMA;
	double OrbPeriod;
	double Ap;
	double Pe;
	double longPeri;
	double EccentricAnomaly;
	double TrueAnomaly;
	double distanceToEarth;

	KeplerCoords2D_t coords2D_Bari;
	KeplerCoords2D_t coords2D_Earth;
} OrbParams_t;

void computeParams(OrbParams_t* params) {
	params->OrbPeriod = OrbitalPeriod(params->MeanMotion);
	params->SMA = SemiMajorAxis(params->OrbPeriod);
	params->Ap = Apoapsis(params->Eccentricity, params->SMA);
	params->Pe = Periapsis(params->Eccentricity, params->SMA);
	params->longPeri = (params->AscNodeLong + params->ArgPeri);
	params->longPeri *= RADS2DEGS;
	params->longPeri -= (double)((uint32_t)(params->longPeri / 360.0) * 360);
	params->longPeri *= DEGS2RADS;

	params->EccentricAnomaly = params->MeanAnomaly + params->Eccentricity * sin(params->MeanAnomaly);
	params->EccentricAnomaly = NewtonRaphson(params->MeanAnomaly, params->Eccentricity, *KeplerEquation, *KeplerPrime, params->EccentricAnomaly, 1E-5 * DEGS2RADS, 100000);

	params->TrueAnomaly = TrueAnomaly(params->Eccentricity, params->EccentricAnomaly);
	params->distanceToEarth = OrbAltTA(params->Eccentricity, params->SMA, params->TrueAnomaly);

	params->coords2D_Earth = coordsFromTA(params->distanceToEarth, params->TrueAnomaly);
	KeplerCoords2D_t bari = baricenterRelativeToFocal(params->SMA, params->Eccentricity);
	params->coords2D_Bari = PointRelativeToBaricenter(bari, params->coords2D_Earth);
}

void printParams(OrbParams_t params) {
	printf("Orbital Period : %.4lf secs (%s)\n", params.OrbPeriod, secstohms(params.OrbPeriod));
	printf("Semi Major Axis : %.0lf m\n", params.SMA);
	printf("Apoapsis : %.0lf m | Periapsis : %.0lf m\n", params.Ap - (double)EARTH_RADIUS, params.Pe - (double)EARTH_RADIUS);
	printf("Eccentric Anomaly : %.4lf degs\n", params.EccentricAnomaly * RADS2DEGS);
	printf("True Anomaly : %.4lf degs\n", params.TrueAnomaly * RADS2DEGS);
	printf("Distance to Earth core : %.0lf m\n", params.distanceToEarth);
	printf("2D rel. to Earth core : X = %.4lf | Y = %.4lf\n", params.coords2D_Earth.x, params.coords2D_Earth.y);
	printf("2D rel. to Baricenter : X = %.4lf | Y = %.4lf\n", params.coords2D_Bari.x, params.coords2D_Bari.y);
	printf("Longitude of Periapsis : %.4f degs\n", params.longPeri * RADS2DEGS);
	printf("Mean Anomaly : %.4lf degs\n", params.MeanAnomaly);
	printf("Mean Motion : %.8lf rev/(sid. day)\n", params.MeanMotion);
	printf("Inclination : %.4lf degs\n", params.Inclination);
	printf("Longitude of Asc. Node : %.4f degs\n", params.AscNodeLong);
	printf("Eccentricity : %.7lf\n", params.Eccentricity);
	printf("Arg. of Periapsis : %.4lf degs\n", params.ArgPeri);
}

void main(int argc, char** argv) {
	double MeanMotion = 15.5;
	double MeanAnomaly = 2.47413;
	double ArgPeri = 0.440942;
	double AscNodeLong = 0.248486;
	double Inclination = 0.934062;
	double Eccentricity = 0.02134;

	OrbParams_t params = {
		.MeanMotion = MeanMotion,
		.MeanAnomaly = MeanAnomaly,
		.ArgPeri = ArgPeri,
		.AscNodeLong = AscNodeLong,
		.Inclination = Inclination,
		.Eccentricity = Eccentricity
	};
	computeParams(&params);
	printParams(params);
}