// ----------------------------------------------------------------------------
// BSD 3-Clause License

// Copyright (c) 2016, qbrobotics
// Copyright (c) 2017, Centro "E.Piaggio"
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

/**
* \file         interruptions.c
*
* \brief        Interruption handling and firmware core functions
* \date         June 06, 2016
* \author       _qbrobotics_
* \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
* \copyright    (C) 2017 Centro "E.Piaggio". All rights reserved.
*/


//=================================================================     includes
#include <interruptions.h>
#include <command_processing.h>
#include <globals.h>
#include <utils.h>


//==============================================================================
//                                                            RS485 RX INTERRUPT
//==============================================================================
// Processing RS-485 data frame:
//
// - 0:     Waits for beggining characters
// - 1:     Waits for ID;
// - 2:     Data length;
// - 3:     Receive all bytes;
// - 4:     Wait for another device end of transmission;
//
//==============================================================================

// PWM values needed to obtain 11.5 V given a certain input tension
// Numbers are sperimentally calculated //[index] (millivolts)
static const uint8 pwm_preload_values[29] = {100,    //0 (11500)
                                              83,
                                              78,
                                              76,
                                              74,
                                              72,    //5 (14000)
                                              70,
                                              68,
                                              67,
                                              65,
                                              64,    //10 (16500)
                                              63,
                                              62,
                                              61,
                                              60,
                                              59,    //15 (19000)
                                              58,
                                              57,
                                              56,
                                              56,
                                              55,    //20 (21500)
                                              54,
                                              54,
                                              53,
                                              52,
                                              52,    //25 (24000)
                                              52,
                                              51,
                                              51};   //28 (25500)

//==============================================================================
//                                                            WATCHDOG INTERRUPT
//==============================================================================

CY_ISR(ISR_WATCHDOG_Handler){

    // Set WDT flag
    
    watchdog_flag = TRUE;

}

//==============================================================================
//                                                            RS485 RX INTERRUPT
//==============================================================================

CY_ISR(ISR_RS485_RX_ExInterrupt) {

    // Set RS485 flag
    
    interrupt_flag = TRUE;
     
}

//==============================================================================
//                                                             INTERRUPT MANAGER
//==============================================================================
// Manage interrupt from RS485 
//==============================================================================
// Processing RS-485 data frame:
//
// - WAIT_START:    Waits for beginning characters;
// - WAIT_ID:       Waits for ID;
// - WAIT_LENGTH:   Data length;
// - RECEIVE:       Receive all bytes;
// - UNLOAD:        Wait for another device end of transmission;
//
//==============================================================================

void interrupt_manager(){

    
    //===========================================     local variables definition

    static uint8 CYDATA state = WAIT_START;                      // state
    
    //------------------------------------------------- local data packet
    static uint8 CYDATA data_packet_index;
    static uint8 CYDATA data_packet_length;
    static uint8 data_packet_buffer[128];                     
    static uint8 CYDATA rx_queue[3];                    // last 2 bytes received
    //-------------------------------------------------

    uint8 CYDATA    rx_data;                            // RS485 UART rx data
    CYBIT           rx_data_type;                       // my id?
    uint8 CYDATA    package_count = 0;                     

    //======================================================     receive routine
    
    // Get data until buffer is not empty 
    
    while(UART_RS485_GetRxBufferSize() && (package_count < 6)){  
        // 6 stima di numero massimo di pacchetti che riesco a leggere senza bloccare l'esecuzione del firmware
        
        // Get next char
        rx_data = UART_RS485_GetChar();
        
        switch (state) {
            //-----     wait for frame start     -------------------------------
            case WAIT_START:
            
                rx_queue[0] = rx_queue[1];
                rx_queue[1] = rx_queue[2];
                rx_queue[2] = rx_data;
                
                // Check for header configuration package
                if ((rx_queue[1] == 58) && (rx_queue[2] == 58)) {
                    rx_queue[0] = 0;
                    rx_queue[1] = 0;
                    rx_queue[2] = 0;
                    state       = WAIT_ID;                    
                }else
                    if ((rx_queue[0] == 63) &&      //ASCII - ?
                        (rx_queue[1] == 13) &&      //ASCII - CR
                        (rx_queue[2] == 10))        //ASCII - LF)
                        infoGet(INFO_ALL);
                break;

            //-----     wait for id     ----------------------------------------
            case  WAIT_ID:

                // packet is for my ID or is broadcast
                if (rx_data == c_mem.id || rx_data == 0)
                    rx_data_type = FALSE;
                else                //packet is for others
                    rx_data_type = TRUE;
                
                data_packet_length = 0;
                state = WAIT_LENGTH;
                break;

            //-----     wait for length     ------------------------------------
            case  WAIT_LENGTH:

 
                data_packet_length = rx_data;
                // check validity of pack length
                if (data_packet_length <= 1) {
                    data_packet_length = 0;
                    state = WAIT_START;
                } else if (data_packet_length > 128) {
                    data_packet_length = 0;
                    state = WAIT_START;
                } else {
                    data_packet_index = 0;
                    
                    if(rx_data_type == FALSE)
                        state = RECEIVE;          // packet for me or broadcast
                    else
                        state = UNLOAD;           // packet for others
                }
                break;

            //-----     receiving     -------------------------------------------
            case RECEIVE:

                data_packet_buffer[data_packet_index] = rx_data;
                data_packet_index++;
                
                // check end of transmission
                if (data_packet_index >= data_packet_length) {
                    // verify if frame ID corresponded to the device ID
                    if (rx_data_type == FALSE) {
                        // copying data from buffer to global packet
                        memcpy(g_rx.buffer, data_packet_buffer, data_packet_length);
                        g_rx.length = data_packet_length;
                        g_rx.ready  = 1;
                        commProcess();
                    }
                    
                    data_packet_index  = 0;
                    data_packet_length = 0;
                    state              = WAIT_START;
                    package_count++;
                
                }
                break;

            //-----     other device is receving     ---------------------------
            case UNLOAD:
                if (!(--data_packet_length)) {
                    data_packet_index  = 0;
                    data_packet_length = 0;
                    RS485_CTS_Write(1);
                    RS485_CTS_Write(0);
                    state              = WAIT_START;
                    package_count++;
                }
                break;
        }
    }
}
//==============================================================================
//                                                            FUNCTION SCHEDULER
//==============================================================================
// Call all the function with the right frequency
//==============================================================================
// Base frequency 1000 Hz
//==============================================================================

void function_scheduler(void) {
 
    static uint16 counter_calibration = DIV_INIT_VALUE;

    timer_value0 = (uint32)MY_TIMER_ReadCounter();

    // Start ADC Conversion, SOC = 1

    ADC_SOC_Write(0x01); 
    
    // Check Interrupt 

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
  
    //---------------------------------- Get Encoders

    encoder_reading(0); 
    
    // Check Interrupt     
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }   
    
    encoder_reading(1);
    
    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    
    encoder_reading(2);
    
    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    //---------------------------------- Control Motor
   
    motor_control();

    // Check Interrupt 

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    //---------------------------------- Control Overcurrent

    overcurrent_control();
    
    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    //---------------------------------- Read conversion buffer - LOCK function

    analog_read_end();

    //---------------------------------- Calibration 

    // Divider 10, freq = 500 Hz
    if (calib.enabled == TRUE) {
        if (counter_calibration == CALIBRATION_DIV) {
            calibration();
            counter_calibration = 0;
        }
        counter_calibration++;
    }

    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
   
    //---------------------------------- Update States
    
    // Load k-1 state
    memcpy( &g_measOld, &g_meas, sizeof(g_meas) );
    memcpy( &g_refOld, &g_ref, sizeof(g_ref) );

    // Load k+1 state
    memcpy( &g_ref, &g_refNew, sizeof(g_ref) );

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    timer_value = (uint32)MY_TIMER_ReadCounter();
    MY_TIMER_WriteCounter(5000000);

}

//==============================================================================
//                                                                MOTORS CONTROL
//==============================================================================

void motor_control() {

    int32 CYDATA pwm_input = 0;

    int32 CYDATA pos_error;         // position error
    int32 CYDATA curr_error;        // current error

    int32 CYDATA i_ref;             // current reference
    int32 CYDATA handle_value;

    int32 CYDATA err_joy_1, err_joy_2;
    int32 CYDATA pos_diff, pos_sum;
    
    int32 CYDATA k_p = c_mem.k_p;  
    int32 CYDATA k_i = c_mem.k_i; 
    int32 CYDATA k_d = c_mem.k_d; 

    int32 CYDATA k_p_c = c_mem.k_p_c;  
    int32 CYDATA k_i_c = c_mem.k_i_c; 
    int32 CYDATA k_d_c = c_mem.k_d_c;   

    int32 CYDATA k_p_dl = c_mem.k_p_dl;  
    int32 CYDATA k_i_dl = c_mem.k_i_dl; 
    int32 CYDATA k_d_dl = c_mem.k_d_dl;
    
    int32 CYDATA k_p_c_dl = c_mem.k_p_c_dl;  
    int32 CYDATA k_i_c_dl = c_mem.k_i_c_dl; 
    int32 CYDATA k_d_c_dl = c_mem.k_d_c_dl;   

    // Static Variables

    static int32 pos_error_sum;     // position error sum for integral
    static int32 curr_error_sum;    // current error sum for integral
    
    static int32 prev_pos_err;      // previous position error for deriv. control
    static int32 prev_curr_err;     // previous current error for deriv. control

    static CYBIT motor_dir = FALSE;

    // =========================== POSITION INPUT ==============================
    switch(c_mem.input_mode) {

        case INPUT_MODE_ENCODER3:

            // Calculate handle value based on position of handle in the
            // sensor chain and multiplication factor between handle and motor position
            if (c_mem.double_encoder_on_off) 
                handle_value = g_meas.pos[2] * c_mem.motor_handle_ratio;
            else
                handle_value = g_meas.pos[1] * c_mem.motor_handle_ratio;
            

            // Read handle and use it as reference for the motor
            if (((handle_value - g_ref.pos[0]) > c_mem.max_step_pos) && (c_mem.max_step_pos != 0))
                g_ref.pos[0] += c_mem.max_step_pos;
            else {
                if (((handle_value - g_ref.pos[0]) < c_mem.max_step_neg) && (c_mem.max_step_neg != 0))
                    g_ref.pos[0] += c_mem.max_step_neg;
                else
                g_ref.pos[0] = handle_value;
            }
            break;

        case INPUT_MODE_JOYSTICK:
            switch(c_mem.control_mode) {
                // ======================= POSITION CONTROL =======================
                case CONTROL_ANGLE:
                    err_joy_1 = 0;
                    err_joy_2 = 0;
                    if(!(g_meas.joystick[0] > 900)) {
                        err_joy_1 = g_meas.joystick[0] - g_mem.joystick_threshold;

                        //if (err_joy_1 > 0) {   
                        if(g_meas.joystick[0] > g_mem.joystick_threshold) {     //both motors wind the wire around the pulley
                            g_ref.pos[0] += ((int32) err_joy_1 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);
                            g_ref.pos[1] += ((int32) err_joy_1 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);

                        }
                        else {
                            err_joy_1 = g_meas.joystick[0] + g_mem.joystick_threshold;
                            //if (err_joy_1 < 0) {
                            if(g_meas.joystick[0] < -g_mem.joystick_threshold) {  //both motors unroll the wire around the pulley
                                g_ref.pos[0] += ((int32) err_joy_1 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);
                                g_ref.pos[1] += ((int32) err_joy_1 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);

                            }
                        }

                        err_joy_2 = g_meas.joystick[1] - g_mem.joystick_threshold;
                        //if(err_joy_2 > 0) {
                        if(g_meas.joystick[1] > g_mem.joystick_threshold) {    //The wire is winded around the first pulley and unwinded from the second one
                            g_ref.pos[0] += ((int32) err_joy_2 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);
                            g_ref.pos[1] -= ((int32) err_joy_2 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);
                        
                        }
                        else {
                            err_joy_2 = g_meas.joystick[1] + g_mem.joystick_threshold;
                            //if(err_joy_2 < 0) {
                            if(g_meas.joystick[1] < -g_mem.joystick_threshold) {  //The wire is unwinded from the first pulley and winded on the second one
                                g_ref.pos[0] += ((int32) err_joy_2 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);
                                g_ref.pos[1] -= ((int32) err_joy_2 * g_mem.closure_speed) / (1024 - g_mem.joystick_threshold);

                            }
                        }
                    }

                    else {  //The button is pressed and the hand is opened firstly making the position difference
                            //equal to zero, then the position sum is gradually set to zero 
                        pos_diff = (g_meas.pos[0] - g_meas.pos[1]) >> c_mem.res[0];
                        pos_sum = (g_meas.pos[0] + g_meas.pos[1]) >> c_mem.res[0];

                        if(abs(pos_diff) > 500) {
                            if(SIGN(pos_diff) == 1) {
                                g_ref.pos[1] += g_mem.closure_speed;
                                g_ref.pos[0] -= g_mem.closure_speed / 2;
                            }
                            else {
                                g_ref.pos[0] += g_mem.closure_speed;
                                g_ref.pos[1] -= g_mem.closure_speed / 2;
                            }
                           
                        }
                        else {
                            if(pos_sum > 0) {
                                g_ref.pos[0] -= g_mem.closure_speed * 2;
                                g_ref.pos[1] -= g_mem.closure_speed * 2;
                                if(g_ref.pos[0] < 0)
                                    g_ref.pos[0] = 0;
                                if(g_ref.pos[1] < 0);
                                    g_ref.pos[1] = 0;
                            }
                        }
                    }
                    break;
                // ======================= PWM CONTROL =======================
                // (Gravity Compensation Arm)
                case CONTROL_PWM:
                    
                    // g_meas.joystick[1] (UP/DOWN DIRECTION) has to be rescaled to range [-100, 100] 
                    // to feed motor with coherent pwm input
                    err_joy_2 = (int32) (-(100.0/512.0) * g_meas.joystick[1]);
                    
                    // Compensation Arm is going up (PWM < 0)
                    if (g_meas.curr[0] < -g_mem.current_limit && g_meas.joystick[1] > 0)
                        err_joy_2 = 0;
                    
                    // Compensation Arm is going up (PWM < 0)
                    if (g_meas.curr[0] > g_mem.current_limit && g_meas.joystick[1] < 0)
                        err_joy_2 = 0;
                        
                    g_ref.pwm[0] = (int32) err_joy_2;
                    
                    break;
                    
                default:
                    break;
            }
      
        break;
            
        default:
            break;
    }

    // Position limit saturation
    if (c_mem.pos_lim_flag) {
        if (g_ref.pos[0] < c_mem.pos_lim_inf[0]) 
            g_ref.pos[0] = c_mem.pos_lim_inf[0];
        if (g_ref.pos[0] > c_mem.pos_lim_sup[0]) 
            g_ref.pos[0] = c_mem.pos_lim_sup[0];
    }

    switch(c_mem.control_mode) {
        // ======================= CURRENT AND POSITION CONTROL =======================
        case CURR_AND_POS_CONTROL:
            pos_error = g_ref.pos[0] - g_meas.pos[0];

            pos_error_sum += pos_error;

            // error_sum saturation
            if (pos_error_sum > POS_INTEGRAL_SAT_LIMIT)
                pos_error_sum = POS_INTEGRAL_SAT_LIMIT;
            else {
                if (pos_error_sum < -POS_INTEGRAL_SAT_LIMIT) 
                    pos_error_sum = -POS_INTEGRAL_SAT_LIMIT;
            }
            
            // ------ position PID control ------

            i_ref = 0;
            
            // Proportional
            if (k_p_dl != 0)
                i_ref += (int32)(k_p_dl * pos_error) >> 16;

            // Integral
            if (k_i_dl != 0)
                i_ref += (int32)(k_i_dl * (pos_error_sum >> 6)) >> 10;

            // Derivative
            if (k_d_dl != 0)
                i_ref += (int32)(k_d_dl * (pos_error - prev_pos_err)) >> 16;
                        
            // Update previous position
            prev_pos_err = pos_error;

            // // current set through position reference
            // i_ref = g_ref.pos[0] >> g_mem.res[0];

            // motor direction depends on i_ref
            if (i_ref >= 0)
                motor_dir = TRUE;
            else
                motor_dir = FALSE;

            // current ref must be positive
            //i_ref = abs(i_ref);

            // saturate max current
            if (i_ref > c_mem.current_limit)
                i_ref = c_mem.current_limit;
            else 
                if (i_ref < -c_mem.current_limit)
                    i_ref = -c_mem.current_limit;
            

            // saturate min current
            /*if (i_ref < MIN_CURR_SAT_LIMIT && i_ref > 0) {
                i_ref = MIN_CURR_SAT_LIMIT;
            }*/

            // // write i_ref on meas curr 2 for DEBUG
            //g_meas.curr[1] = i_ref;

            // current error and curr error sum
            curr_error = i_ref - g_meas.curr[0];
            curr_error_sum += curr_error;
            
            if (curr_error_sum > CURR_INTEGRAL_SAT_LIMIT)
                curr_error_sum = CURR_INTEGRAL_SAT_LIMIT;
            else {
                if (curr_error_sum < -CURR_INTEGRAL_SAT_LIMIT)
                    curr_error_sum = -CURR_INTEGRAL_SAT_LIMIT;
            }

            // ----- current PID control -----

            pwm_input = 0;

            // Proportional
            if (k_p_c_dl != 0)
                pwm_input += (int32)(k_p_c_dl * curr_error) >> 16;

            // Integral
            if (k_i_c_dl != 0)
                pwm_input += (int32)(k_i_c_dl * (curr_error_sum >> 6)) >> 10;

            // Derivative
            if (k_d_c_dl != 0)
                pwm_input += (int32)(k_d_c_dl * (curr_error - prev_curr_err)) >> 16;

            // pwm_input saturation
            if (pwm_input < -PWM_MAX_VALUE) 
                pwm_input = -PWM_MAX_VALUE;
            else {
                if (pwm_input > PWM_MAX_VALUE)
                    pwm_input = PWM_MAX_VALUE;
            }

            // Update previous current
            prev_curr_err = curr_error;

        break;

        // ============================== POSITION CONTROL =====================
        case CONTROL_ANGLE:
            pos_error = g_ref.pos[0] - g_meas.pos[0];

            pos_error_sum += pos_error;

            // anti-windup (for integral control)
            if (pos_error_sum > ANTI_WINDUP) {
                pos_error_sum = ANTI_WINDUP;
            } else if (pos_error_sum < -ANTI_WINDUP) {
                pos_error_sum = -ANTI_WINDUP;
            }

            // Proportional
            if (k_p != 0) 
                pwm_input = (int32)(k_p * pos_error) >> 16;
            

            // Integral
            if (k_i != 0) 
                pwm_input += (int32)(k_i * (pos_error_sum >> 6)) >> 10;
            

            // Derivative
            if (k_d != 0) 
                pwm_input += (int32)(k_d * (pos_error - prev_pos_err)) >> 16;
            

            // Update measure
            prev_pos_err = pos_error;

            if (pwm_input > 0)
                motor_dir = TRUE;
            else
                motor_dir = FALSE;

        break;

        // ========================== CURRENT CONTROL ==========================
        case CONTROL_CURRENT:
            if(g_ref.onoff && tension_valid) {
                
                i_ref = g_ref.curr[0];

                if (i_ref > c_mem.current_limit) 
                    i_ref = c_mem.current_limit;
                else {
                    if (i_ref < -c_mem.current_limit)
                        i_ref = -c_mem.current_limit;
                }
                
                // current error
                curr_error = i_ref - g_meas.curr[0];            
                curr_error_sum += curr_error;
                
                if (curr_error_sum > CURR_INTEGRAL_SAT_LIMIT)
                    curr_error_sum = CURR_INTEGRAL_SAT_LIMIT;
                else {
                    if (curr_error_sum < -CURR_INTEGRAL_SAT_LIMIT) 
                        curr_error_sum = -CURR_INTEGRAL_SAT_LIMIT;
                }

                pwm_input = 0;

                // Proportional
                if (k_p_c != 0)
                    pwm_input += (int32)(k_p_c * curr_error) >> 16;

                // Integral
                if (k_i_c != 0)
                    pwm_input += (int32)(k_i_c * (curr_error_sum >> 6)) >> 10;

                // Derivative
                if (k_d_c != 0)
                    pwm_input += (int32)(k_d_c * (curr_error - prev_curr_err)) >> 16;
                
                prev_curr_err = curr_error;
                
                if (pwm_input >= 0) 
                    motor_dir = TRUE;
                else
                    motor_dir = FALSE;
            }
        break;
        
        // ================= DIRECT PWM CONTROL ================================
        case CONTROL_PWM:

            pwm_input = g_ref.pwm[0];

            if (pwm_input > 0) 
                motor_dir = TRUE;
            else 
                motor_dir = FALSE;
            

            // pwm_input saturation
            if (pwm_input < -PWM_MAX_VALUE) 
                pwm_input = -PWM_MAX_VALUE;
             else if (pwm_input > PWM_MAX_VALUE) 
                pwm_input = PWM_MAX_VALUE;
            

        break;
    }

    ////////////////////////////////////////////////////////////////////////////

    if(pwm_input >  PWM_MAX_VALUE) 
        pwm_input =  PWM_MAX_VALUE;
    if(pwm_input < -PWM_MAX_VALUE) 
        pwm_input = -PWM_MAX_VALUE;


    if (c_mem.control_mode != CONTROL_PWM) 
        pwm_input = (((pwm_input << 10) / PWM_MAX_VALUE) * dev_pwm_limit) >> 10;
    

    pwm_sign = SIGN(pwm_input);

    if (motor_dir)
        MOTOR_DIR_Write(0x01);
    else
        MOTOR_DIR_Write(0x00);

    PWM_MOTORS_WriteCompare1(abs(pwm_input));
}

//==============================================================================
//                                                                   ENCODER READING
//==============================================================================

void encoder_reading(const uint8 idx) {

    uint8 CYDATA index = idx;
    
    uint8 jj;
    
    uint32 data_encoder;
    int32 value_encoder;
    int32 aux;

    static int32 last_value_encoder[NUM_OF_SENSORS];

    static uint8 error[NUM_OF_SENSORS];
    
    static CYBIT only_first_time = TRUE;

    static uint8 one_time_execute = 0;
    static CYBIT pos_reconstruct = FALSE;

    // static int32 l_value[NUM_OF_SENSORS];   //last value for vel
    // static int32 ll_value[NUM_OF_SENSORS];  //last last value for vel
    // static int32 lll_value[NUM_OF_SENSORS];  //last last last value for vel

    if (index >= NUM_OF_SENSORS)
        return;
    
    if (reset_last_value_flag) {
        for (jj = NUM_OF_SENSORS; jj--;) 
            last_value_encoder[jj] = 0;
        
        reset_last_value_flag = 0;
    }

    //======================================================     reading sensors
        // Shift 1 right to erase Dummy bit of chain
    if (index == 0)
            data_encoder = (SHIFTREG_ENC_1_ReadData() >> 1) & 0x3FFFF;  //0x0003FFFF reset first 14 bits
    else {
        if (index == 1)
            data_encoder = (SHIFTREG_ENC_2_ReadData() >> 1) & 0x3FFFF;  //0x0003FFFF reset first 14 bits
        else // index == 2
            data_encoder = (SHIFTREG_ENC_3_ReadData() >> 1) & 0x3FFFF;  //0x0003FFFF reset first 14 bits
    }
       

    if (check_enc_data(&data_encoder)) {

        value_encoder = (int16) (((data_encoder & 0x3FFC0) - 0x20000) >> 2 ) + g_mem.m_off[index];
                                                    // reset last 6 bit 
                                                    // -> |:|:|id|dim|CMD|CHK|(data---
                                                    // subtract half of max value
                                                    // and shift to have 16 bit val

        // Initialize last_value_encoder
        if (only_first_time) {
            last_value_encoder[index] = value_encoder;
            if (index == 2)
                only_first_time = 0;
        }

        // Take care of rotations
        aux = value_encoder - last_value_encoder[index];

        // ====================== 1 TURN ======================
        // -32768                    0                    32767 -32768                   0                     32767
        // |-------------------------|-------------------------|-------------------------|-------------------------|
        //              |                         |      |           |      |                         |
        //           -16384                     16383    |           |   -16384                     16383
        //                                               |           |
        //                                           24575           -24576
        //                                               |___________|
        //                                                   49152

        // if we are in the right interval, take care of rotation
        // and update the variable only if the difference between
        // one measure and another is less than 1/4 of turn

        // Considering we are sampling at 1kHz, this means that our shaft needs
        // to go slower than 1/4 turn every ms -> 1 turn every 4ms
        // equal to 250 turn/s -> 15000 RPM

        if (aux > 49152)
            g_meas.rot[index]--;
        else{ 
            if (aux < -49152)
                g_meas.rot[index]++;
            else{
                if (abs(aux) > 16384) { // if two measure are too far
                    error[index]++;
                    if (error[index] < 10)
                        // Discard
                        return;
                }
            }
        }

        error[index] = 0;

        last_value_encoder[index] = value_encoder;

        value_encoder += (int32)g_meas.rot[index] << 16;

        if (c_mem.m_mult[index] != 1.0) {
            value_encoder *= c_mem.m_mult[index];
        }

        g_meas.pos[index] = value_encoder;
    }

    // wait at least 3 * max_num_of_error (10) + 5 = 35 cycles to reconstruct the right turn
    if (pos_reconstruct == FALSE){
        if (one_time_execute < 34) 
            one_time_execute++;
        else {
            //Double encoder translation
            if (c_mem.double_encoder_on_off)
                g_meas.rot[0] = calc_turns_fcn(g_meas.pos[0], g_meas.pos[1]);

            g_meas.pos[0] += (int32) g_meas.rot[0] << 16;

            // If necessary activate motors
            g_refNew.pos[0] = g_meas.pos[0];

            MOTOR_ON_OFF_Write(g_ref.onoff);
            
            pos_reconstruct = TRUE;
        }
    }
}

//==============================================================================
//                                                           ANALOG MEASUREMENTS
//==============================================================================

void analog_read_end() {

    /* =========================================================================
    /   Ideal formulation to calculate tension and current
    /
    /   tension = ((read_value_mV - offset) * 101) / gain -> [mV]
    /   current = ((read_value_mV - offset) * 375) / (gain * resistor) -> [mA]
    /
    /   Definition:
    /   read_value_mV = counts_read / 0.819 -> conversion from counts to mV
    /   offset = 2000 -> hardware amplifier bias in mV
    /   gain = 8.086 -> amplifier gain
    /   resistor = 18 -> resistor of voltage divider in KOhm 
    /
    /   Real formulation: tradeoff in good performance and accurancy, ADC_buf[] 
    /   and offset unit of measurment is counts, instead dev_tension and
    /   g_meas.curr[] are converted in properly unit.
    /  =========================================================================
    */

    int16 CYDATA i_aux;
    
    static joystick_status CYDATA UD_status = RESET;
    static joystick_status CYDATA LR_status = RESET;  
    
    static uint16 UD_counter = 0;
    static uint16 LR_counter = 0;
    
    static int32 UD_mean_value;
    static int32 LR_mean_value;

    // Wait for conversion end
    
    while(!ADC_STATUS_Read()){
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
    
    // Convert tension read
    dev_tension = ((int32)(ADC_buf[0] - 1638) * 1952) >> 7;
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    // Until there is no valid input tension repeat this measurement
    
    if (dev_tension > 0){
        // Set tension valid bit to TRUE

        tension_valid = TRUE;

        if(g_mem.activate_pwm_rescaling)        //pwm rescaling is activated
            pwm_limit_search();                 //only for 12V motors

        // Filter and Set currents
        g_meas.curr[0] = filter_i1((int16) (((int32)(ADC_buf[1] - 1638) * 25771) >> 13) * pwm_sign);
        
        // Check Interrupt 
    
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }

        switch (UD_status) {
            case NORMAL:
                i_aux = ADC_buf[2];
                // Remap the analog reading from -1024 to 1024.  
                i_aux = (int32) (((float) (i_aux - UD_mean_value) / UD_mean_value) * g_mem.joystick_gains[1]); 
                
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                //Saturation
                if (i_aux < -1024) 
                    i_aux = -1024;
                if (i_aux > 1024)
                    i_aux = 1024;

                g_meas.joystick[1] = (int16) i_aux;

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

            break;

            case RESET: // reset variables
                UD_counter = 0;
                UD_mean_value = 0;
                UD_status = WAIT; // go to waiting status

            break;

            case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples
                UD_counter++;
                if (UD_counter == JOYSTICK_SAMPLE_TO_DISCARD) {
                    UD_counter = 0;                     // reset counter

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    UD_status = SUM_AND_MEAN;           // sum and mean status
                }

            break;

            case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
                // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
                UD_counter++;
                UD_mean_value += ADC_buf[2];

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                if (UD_counter == SAMPLES_FOR_JOYSTICK_MEAN) {
                    UD_mean_value = UD_mean_value / SAMPLES_FOR_JOYSTICK_MEAN; // calc mean

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    UD_counter = 0;          // reset counter
                    UD_status = NORMAL;           // goto normal execution
                }
            break;

            case WAIT:
                UD_status = DISCARD;           // goto discard sample
            break;
        }

        switch (LR_status) {
            case NORMAL:
                i_aux = ADC_buf[3];

                i_aux = (int32) (((float) (i_aux - LR_mean_value) / LR_mean_value) * g_mem.joystick_gains[0]); 
                
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                //Saturation
                if (i_aux < -1024)
                    i_aux = -1024;
                if (i_aux > 1024)
                    i_aux = 1024;

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                g_meas.joystick[0] = (int16) i_aux;

            break;

            case RESET: // reset variables
                LR_counter = 0;
                LR_mean_value = 0;
                LR_status = WAIT; // goes waiting for all conversions to be done
            break;

            case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples8
                LR_counter++;
                if (LR_counter == JOYSTICK_SAMPLE_TO_DISCARD) {
                    LR_counter = 0;                     // reset counter

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    LR_status = SUM_AND_MEAN;           // sum and mean status
                }
            break;

            case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
                // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
                LR_counter++;
                LR_mean_value += ADC_buf[3];
                if (LR_counter == SAMPLES_FOR_JOYSTICK_MEAN) {
                    LR_mean_value = LR_mean_value / SAMPLES_FOR_JOYSTICK_MEAN; // calc mean
                    
                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    LR_counter = 0;               // reset counter
                    LR_status = NORMAL;           // goto normal execution
                }
            break;

            case WAIT:
                if(UD_status == NORMAL)
                    LR_status = DISCARD;
            break;
        }

        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
    else{
        UD_status = RESET;
        LR_status = RESET;
        
        tension_valid = FALSE;
            
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
        
        // Reset current
        g_meas.curr[0] = 0;

        //Reset Joystick
        g_meas.joystick[0] = 0;
        g_meas.joystick[1] = 0;

    }
        
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
}

//==============================================================================
//                                                           OVERCURRENT CONTROL
//==============================================================================

void overcurrent_control() {
    if (c_mem.current_limit != 0) {
        // if the current is over the limit
        if (g_meas.curr[0] > c_mem.current_limit) {
            //decrese pwm_limit
            dev_pwm_limit--;
        // if the current is in the safe zone
        } else if (g_meas.curr[0] < (c_mem.current_limit - CURRENT_HYSTERESIS)) {
            //increase pwm_limit
            dev_pwm_limit++;
        }

        // Bound pwm_limit
        if (dev_pwm_limit < 0) 
            dev_pwm_limit = 0;
        else 
            if (dev_pwm_limit > 100) 
                dev_pwm_limit = 100;
    }
}

//==============================================================================
//                                                              PWM_LIMIT_SEARCH
//==============================================================================

void pwm_limit_search() {

    uint8 CYDATA index;

    if (dev_tension > 25500) {
        dev_pwm_limit = 0;
    } else if (dev_tension < 11500) {
        dev_pwm_limit = 100;
    } else {
        index = (uint8)((dev_tension - 11500) >> 9);
        dev_pwm_limit = pwm_preload_values[index];
    }
}
/* [] END OF FILE */