/* MiCS-6184 MiCS-6184 Conversion : ADC to Engineering Values
 *  
 */

/*********************************************************************************************************
** Function name:           getSensorRs
** Descriptions:            Converts ADC value to current sensor resistance
**
** @param gasCh : ID of gas to get
**
** return code
** 0 = Error
**
**********************************************************************************************************/

#include "MICS6814.h"

//get current sensor resistance
float getSensorRs(unsigned char gasCh)
{
    int a = 0;
   //first get sensor resistance ADC value
    switch(gasCh)
    {
        case NH3_SENSOR:         // NH3
        a = getAdcValue(VALUE_NH3); //getAdcValue is whatever function that returns current ADC value of gas
        break;
        
        case CO_SENSOR:         // CO
        a = getAdcValue(VALUE_CO);
        break;
        
        case NO2_SENSOR:         // NO2
        a = getAdcValue(VALUE_NO2);
        break;
        
        default:
		break;
    }
    
/* then calculate the resistance value using derived equation 4.3 in section 4.1.1,

R_s=  (56k * ADC_Current_Reading )  /(ADC_Max_Reading -ADC_(Current_Reading ) 
Where; 
Rs= Sensor resistance

*/	 
	float res = 56.0*(float)a/(1023.0-(float)a);
    
	return res;
}



/*********************************************************************************************************
** Function name:           calcGasP
** Description:            calculates gas concentration of CO (in PPM), NH3 (in PPM) and NO2 (in PPM)
**                         See Chapter 4 for calculation basis
** Parameters:
**                          gas - gas name CO or NO2 or NH3
** Returns:
**                          float - gas concentration in PPM
**							
**
** 
*********************************************************************************************************/
float calcGasP(int gasName)
{

	float Rs_NH3, Rs_CO, Rs_NO2; 
	float ratio_NH3, ratio_CO, ratio_NO2; 
   

	
	
	// get sensor current resistance (Rs) value
	Rs_NH3 = getSensorRs(NH3_SENSOR);
	Rs_CO = getSensorRs(CO_SENSOR);
	Rs_NO2 = getSensorRs(NO2_SENSOR);
	
	//calculate resistance ratio (Rs/Ro)
	ratio_NH3 = Rs_NH3/R0_NH3;
	ratio_CO = Rs_CO/R0_CO;
	ratio_NO2 = Rs_NO2/R0_NO2;

	
    float c = 0;

    switch(gasName)
    {
        case CO:	// Calculate Carbon Monoxide from equation 4.4, and check if result is within limits
			
            	c = pow(ratio_CO, -1.182)*4.4922;  
				if (c < 0.1 || c > 1000) {
					c=0;
				}       
				break;
        }
        case NO2:	// Calculate Nitrogen Dioxide from equation 4.5, and check if result is within limits
        {
			 
				c = (ratio_NO2 * 0.1499) + 0.0042; 
                if (c < 0.01 || c > 7) {
					c=0;
				}
            	break;
        }
        case NH3:	// Calculate Ammonia from equation 4.6, and check if result is within limits
        {
			
				c = pow(ratio_NH3, -1.918)*0.5908; 
                if (c < 1 || c > 160) {
					c=0;
				}
				break;
        }
        default:
				break;
    }
    
    //verify result is floating point and return 
    return isnan(c)?-1:c;
}

/*********************************************************************************************************
** Function name:           calcGas
** Description:            calculates gas concentration of CO (in mg/m3), NH3 (in ug/m3) and NO2 (in ug/m3)
**                         See Chapter 4 for calculation basis
** Parameters:
**                          gas - gas name CO or NO2 or NH3
** Returns:
**                          float - gas concentration in EU accepted units CO (mg/m3) or NO2 (ug/m3) or NH3 (mg/m3)
**
**
**
*********************************************************************************************************/
float calcGas(int gasName)
{

	float Rs_NH3, Rs_CO, Rs_NO2;
	float ratio_NH3, ratio_CO, ratio_NO2;
	

	
	
	// get sensor current resistance (Rs) value
	Rs_NH3 = getSensorRs(NH3_SENSOR);
	Rs_CO = getSensorRs(CO_SENSOR);
	Rs_NO2 = getSensorRs(NO2_SENSOR);
	
	//calculate resistance ratio (Rs/Ro)
	ratio_NH3 = Rs_NH3/R0_NH3;
	ratio_CO = Rs_CO/R0_CO;
	ratio_NO2 = Rs_NO2/R0_NO2;

	
	float c = 0;

	switch(gasName)
	{
		case CO:	// Calculate Carbon Monoxide from equation 4.4, and convert to mg/m3
		
		c = pow(ratio_CO, -1.182)*4.4922;
		if (c < 0.1 || c > 1000) { //is gas within limit?
			c=0;
			break;
		}
		c=(c*28.01)/24.45; //convert to mg/m3, where 28.01 is molecular weight of CO in g/mol 
		break;
	}
	case NO2:	// Calculate Nitrogen Dioxide from equation 4.5, and convert to ug/m3
	{
		
		c = (ratio_NO2 * 0.1499) + 0.0042;
		if (c < 0.01 || c > 7) {
			c=0;
			break;
		}
		c = c * 1000; //convert to ppb
		c=(c*46.0055)/24.45; //convert to ug/m3, where 46.0055 is molecular weight of NO2 in g/mol
		break;
	}
	case NH3:	// Calculate Ammonia from equation 4.6, and convert to ug/m3
	{
		
		c = pow(ratio_NH3, -1.918)*0.5908;
		if (c < 1 || c > 160) {
			c=0;
			break;
		}
		c = c * 1000; //convert to ppb
		c=(c*17.031)/24.45; //convert to ug/m3, where 17.031 is molecular weight of NH3 in g/mol
		break;
	}
	default:
	break;
}

//verify result is floating point and return
return isnan(c)?-1:c;
}