// -----------------------------------------------------------------------------
// Copyright (C)  qbrobotics. All rights reserved.
// www.qbrobotics.com
// -----------------------------------------------------------------------------

/**
* \file         utils.h
*
* \brief        Utility functions declaration.
* \date         June 06, 2016
* \author       _qbrobotics_
* \copyright    (C)  qbrobotics. All rights reserved.
*/



#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <globals.h>

//------------------------------------------------------------------     DEFINES

#define TIMER_CLOCK 10000

#define ALPHA 3				///< Voltage and current filters constant
#define BETA  50			///< Emg filters constant
#define GAMMA 32 			///< Velocity filters constant
#define DELTA 32 			///< Acceleration filters constant

#define SIGN(A) (((A) >=0) ? (1) : (-1))	///< Sign calculation function

//-------------------------------------------------------------     DECLARATIONS

/** \name Filters */
/** \{ */

/** Filter on the converted voltage value. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param new_value	New value of the filter.
 *
 * 	\return Returns the filtered voltage value
**/
int32 filter_v(int32 new_value);
/** Filter on the first EMG sensor converted value. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered emg sensor value
**/
int32 filter_ch1(int32 value);
/** Filter on the second EMG sensor converted value. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered emg sensor value
**/
int32 filter_ch2(int32 value);
/** Filter on the motor current converted value. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered current value
**/
int32 filter_i1(int32 value);
/** Filter on first encoder rotational speed. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered first encoder rotational speed value
**/
int32 filter_vel_1(int32 value);
/**	Filter on second encoder rotational speed (if present). The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered second encoder rotational speed value
**/
int32 filter_vel_2(int32 value);
/** Filter on third encoder rotational speed (if present). The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered third encoder rotational speed value
**/
int32 filter_vel_3(int32 value);
/** Filter on first encoder rotational acceleration. The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 * 
 * 	\return Returns the filtered first encoder rotational acceleration value
**/
int32 filter_acc_1(int32 value);
/** Filter on second encoder rotation acceleration (if present). The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered second encoder rotational acceleration value
**/
int32 filter_acc_2(int32 value);
/** Filter on third encoder rotation acceleration (if present). The weighted average between the 
 *	old value and the new one is executed.
 * 	
 *	\param value	New value of the filter. 
 *
 * 	\return Returns the filtered third encoder rotational acceleration value
**/
int32 filter_acc_3(int32 value);
/** \} */ 


/** \name Estimating current and difference */
/** \{ */

/** Function used to obtain current estimation through current lookup table.
 *
 * 	\param pos 			Position of the encoder in ticks.
 *	\param vel 			Speed of the encoder.
 *	\param accel 		Acceleration of the encoder
 *
 * 	\return Returns an estimation of the motor current, depending on its position, velocity
 * 			and acceleration.
**/
int32 curr_estim(int32 pos, int32 vel, int32 accel);
/** Low pass filter on current difference between measured and estimated current 
 *
 *	\param curr_diff 	Difference between the measured current and the estimated one.
 *
 *	\return Returns the filtered current difference value
**/
int32 filter_curr_diff(int32 curr_diff);
/** \} */


/** \name Utility functions */
/** \{ */

/** This functions approximates the value passed to the nearest integer
 *
 *	\param x 	The floating point value that needs to be rounded
**/
int my_round(const double x);
/** This function computes the module function, returning positive values regardless
 * 	of wheter the value passed is negative
 * 
 * 	\param val 		The value of which the module needs to be calculated
 * 	\param divisor	The divisor according to which the module is calculated
**/
uint32 my_mod(int32 val, int32 divisor);
/**	This function controls if the read encoder data is correct or not.
 *
 * \param value 	A pointer to the encoder data read 
 *
 * \return Returns 1 if the read data is correct, 0 otherwise	
**/
CYBIT check_enc_data(const uint32* value);
/**	This function is used at startup to reconstruct the correct turn of the shaft
 *  connected to the motor. It need two encoders to work.
 *
 * \param pos1		First encoder position
 * \param pos2		Second encoder position
 * 
 * \return Returns the number of turns of motor pulley at startup
**/
int calc_turns_fcn(const int32 pos1,const int32 pos2);
/** This function counts a series of hand opening and closing used to execute a 
 * calibration of the device.
**/
void calibration();

#endif

/* [] END OF FILE */
