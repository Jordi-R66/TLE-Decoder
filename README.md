# Two-Line Elements Set Decoder

This decoder parses from a given object's TLE all of the data needed to perform orbital calculations.

The only compatible format is the 3LE format.

## Useful links

[TLEs Wikipedia article](https://en.wikipedia.org/wiki/Two-line_element_set#Format) | [Celestrak (to get the sets)](https://celestrak.org/NORAD/elements/)



## Data representation

```c
typedef struct TLE {
	string name[NAME_LENGTH];

	// First line

	uint16_t NORAD_ID;
	char Classification;

	uint8_t COSPAR_YR: 7; // Max value : 99
	uint16_t COSPAR_LN: 10; // Max value : 999
	string COSPAR_OBJ_ID[LAUNCH_PART_LENGTH];

	uint8_t EPOCH_YR: 7; // Max value : 99
	double EPOCH;

	double FIRST_DERIV_MEAN_MOTION;
	double SECOND_DERIV_MEAN_MOTION;

	double B_STAR;

	// Second line

	float Inclination;
	float AscNodeLong;
	float Eccentricity;
	float PeriArgument;
	float MeanAnomaly;

	double MeanMotion;
} TLE;
```



`NORAD_ID`unsigned 16 bits integer, max value of 65535, the latest is 59681



## Capacities :



- Opening a file containing several entries

- Exporting several entries to a binary file containing the representation of the structs