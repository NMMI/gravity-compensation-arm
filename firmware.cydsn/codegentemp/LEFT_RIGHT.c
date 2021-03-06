/*******************************************************************************
* File Name: LEFT_RIGHT.c  
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
#include "LEFT_RIGHT.h"


/*******************************************************************************
* Function Name: LEFT_RIGHT_Write
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
void LEFT_RIGHT_Write(uint8 value) 
{
    uint8 staticBits = (LEFT_RIGHT_DR & (uint8)(~LEFT_RIGHT_MASK));
    LEFT_RIGHT_DR = staticBits | ((uint8)(value << LEFT_RIGHT_SHIFT) & LEFT_RIGHT_MASK);
}


/*******************************************************************************
* Function Name: LEFT_RIGHT_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  LEFT_RIGHT_DM_STRONG     Strong Drive 
*  LEFT_RIGHT_DM_OD_HI      Open Drain, Drives High 
*  LEFT_RIGHT_DM_OD_LO      Open Drain, Drives Low 
*  LEFT_RIGHT_DM_RES_UP     Resistive Pull Up 
*  LEFT_RIGHT_DM_RES_DWN    Resistive Pull Down 
*  LEFT_RIGHT_DM_RES_UPDWN  Resistive Pull Up/Down 
*  LEFT_RIGHT_DM_DIG_HIZ    High Impedance Digital 
*  LEFT_RIGHT_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void LEFT_RIGHT_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(LEFT_RIGHT_0, mode);
}


/*******************************************************************************
* Function Name: LEFT_RIGHT_Read
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
*  Macro LEFT_RIGHT_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 LEFT_RIGHT_Read(void) 
{
    return (LEFT_RIGHT_PS & LEFT_RIGHT_MASK) >> LEFT_RIGHT_SHIFT;
}


/*******************************************************************************
* Function Name: LEFT_RIGHT_ReadDataReg
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
uint8 LEFT_RIGHT_ReadDataReg(void) 
{
    return (LEFT_RIGHT_DR & LEFT_RIGHT_MASK) >> LEFT_RIGHT_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(LEFT_RIGHT_INTSTAT) 

    /*******************************************************************************
    * Function Name: LEFT_RIGHT_ClearInterrupt
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
    uint8 LEFT_RIGHT_ClearInterrupt(void) 
    {
        return (LEFT_RIGHT_INTSTAT & LEFT_RIGHT_MASK) >> LEFT_RIGHT_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
