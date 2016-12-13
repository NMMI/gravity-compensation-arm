/*******************************************************************************
* File Name: UP_DOWN.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "UP_DOWN.h"


/*******************************************************************************
* Function Name: UP_DOWN_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void UP_DOWN_Write(uint8 value) 
{
    uint8 staticBits = (UP_DOWN_DR & (uint8)(~UP_DOWN_MASK));
    UP_DOWN_DR = staticBits | ((uint8)(value << UP_DOWN_SHIFT) & UP_DOWN_MASK);
}


/*******************************************************************************
* Function Name: UP_DOWN_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  UP_DOWN_DM_STRONG     Strong Drive 
*  UP_DOWN_DM_OD_HI      Open Drain, Drives High 
*  UP_DOWN_DM_OD_LO      Open Drain, Drives Low 
*  UP_DOWN_DM_RES_UP     Resistive Pull Up 
*  UP_DOWN_DM_RES_DWN    Resistive Pull Down 
*  UP_DOWN_DM_RES_UPDWN  Resistive Pull Up/Down 
*  UP_DOWN_DM_DIG_HIZ    High Impedance Digital 
*  UP_DOWN_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void UP_DOWN_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(UP_DOWN_0, mode);
}


/*******************************************************************************
* Function Name: UP_DOWN_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro UP_DOWN_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 UP_DOWN_Read(void) 
{
    return (UP_DOWN_PS & UP_DOWN_MASK) >> UP_DOWN_SHIFT;
}


/*******************************************************************************
* Function Name: UP_DOWN_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 UP_DOWN_ReadDataReg(void) 
{
    return (UP_DOWN_DR & UP_DOWN_MASK) >> UP_DOWN_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(UP_DOWN_INTSTAT) 

    /*******************************************************************************
    * Function Name: UP_DOWN_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 UP_DOWN_ClearInterrupt(void) 
    {
        return (UP_DOWN_INTSTAT & UP_DOWN_MASK) >> UP_DOWN_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
