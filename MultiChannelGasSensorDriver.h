/*MiCS-6184 Conversion : ADC to Engineering Values
*
* Header file
 */


#define	NH3_SENSOR	0
#define	CO_SENSOR 	1
#define	NO2_SENSOR 	2

#define	NH3			0
#define	CO		 	1
#define	NO2		 	2

// Base resistance (R0) Value in k
#define R0_NH3		69.9
#define R0_CO		88.66
#define R0_NO2		6.2

/* return the resistance value for the current ADC value for sensor*/
float getSensorRs(unsigned char gasCh);

/* calculate the current gas concentration for a specific gas in PPM */
float calcGasP(int gas);

/* calculate the current gas concentration for a specific gas in EU acceptable units */
float calcGas(int gas);