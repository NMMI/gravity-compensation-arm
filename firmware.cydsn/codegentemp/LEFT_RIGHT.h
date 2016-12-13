/*******************************************************************************
* File Name: LEFT_RIGHT.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_LEFT_RIGHT_H) /* Pins LEFT_RIGHT_H */
#define CY_PINS_LEFT_RIGHT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LEFT_RIGHT_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LEFT_RIGHT_Write(uint8 value) ;
void    LEFT_RIGHT_SetDriveMode(uint8 mode) ;
uint8   LEFT_RIGHT_ReadDataReg(void) ;
uint8   LEFT_RIGHT_Read(void) ;
uint8   LEFT_RIGHT_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LEFT_RIGHT_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define LEFT_RIGHT_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define LEFT_RIGHT_DM_RES_UP          PIN_DM_RES_UP
#define LEFT_RIGHT_DM_RES_DWN         PIN_DM_RES_DWN
#define LEFT_RIGHT_DM_OD_LO           PIN_DM_OD_LO
#define LEFT_RIGHT_DM_OD_HI           PIN_DM_OD_HI
#define LEFT_RIGHT_DM_STRONG          PIN_DM_STRONG
#define LEFT_RIGHT_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define LEFT_RIGHT_MASK               LEFT_RIGHT__MASK
#define LEFT_RIGHT_SHIFT              LEFT_RIGHT__SHIFT
#define LEFT_RIGHT_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LEFT_RIGHT_PS                     (* (reg8 *) LEFT_RIGHT__PS)
/* Data Register */
#define LEFT_RIGHT_DR                     (* (reg8 *) LEFT_RIGHT__DR)
/* Port Number */
#define LEFT_RIGHT_PRT_NUM                (* (reg8 *) LEFT_RIGHT__PRT) 
/* Connect to Analog Globals */                                                  
#define LEFT_RIGHT_AG                     (* (reg8 *) LEFT_RIGHT__AG)                       
/* Analog MUX bux enable */
#define LEFT_RIGHT_AMUX                   (* (reg8 *) LEFT_RIGHT__AMUX) 
/* Bidirectional Enable */                                                        
#define LEFT_RIGHT_BIE                    (* (reg8 *) LEFT_RIGHT__BIE)
/* Bit-mask for Aliased Register Access */
#define LEFT_RIGHT_BIT_MASK               (* (reg8 *) LEFT_RIGHT__BIT_MASK)
/* Bypass Enable */
#define LEFT_RIGHT_BYP                    (* (reg8 *) LEFT_RIGHT__BYP)
/* Port wide control signals */                                                   
#define LEFT_RIGHT_CTL                    (* (reg8 *) LEFT_RIGHT__CTL)
/* Drive Modes */
#define LEFT_RIGHT_DM0                    (* (reg8 *) LEFT_RIGHT__DM0) 
#define LEFT_RIGHT_DM1                    (* (reg8 *) LEFT_RIGHT__DM1)
#define LEFT_RIGHT_DM2                    (* (reg8 *) LEFT_RIGHT__DM2) 
/* Input Buffer Disable Override */
#define LEFT_RIGHT_INP_DIS                (* (reg8 *) LEFT_RIGHT__INP_DIS)
/* LCD Common or Segment Drive */
#define LEFT_RIGHT_LCD_COM_SEG            (* (reg8 *) LEFT_RIGHT__LCD_COM_SEG)
/* Enable Segment LCD */
#define LEFT_RIGHT_LCD_EN                 (* (reg8 *) LEFT_RIGHT__LCD_EN)
/* Slew Rate Control */
#define LEFT_RIGHT_SLW                    (* (reg8 *) LEFT_RIGHT__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LEFT_RIGHT_PRTDSI__CAPS_SEL       (* (reg8 *) LEFT_RIGHT__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LEFT_RIGHT_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LEFT_RIGHT__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LEFT_RIGHT_PRTDSI__OE_SEL0        (* (reg8 *) LEFT_RIGHT__PRTDSI__OE_SEL0) 
#define LEFT_RIGHT_PRTDSI__OE_SEL1        (* (reg8 *) LEFT_RIGHT__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LEFT_RIGHT_PRTDSI__OUT_SEL0       (* (reg8 *) LEFT_RIGHT__PRTDSI__OUT_SEL0) 
#define LEFT_RIGHT_PRTDSI__OUT_SEL1       (* (reg8 *) LEFT_RIGHT__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LEFT_RIGHT_PRTDSI__SYNC_OUT       (* (reg8 *) LEFT_RIGHT__PRTDSI__SYNC_OUT) 


#if defined(LEFT_RIGHT__INTSTAT)  /* Interrupt Registers */

    #define LEFT_RIGHT_INTSTAT                (* (reg8 *) LEFT_RIGHT__INTSTAT)
    #define LEFT_RIGHT_SNAP                   (* (reg8 *) LEFT_RIGHT__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins LEFT_RIGHT_H */


/* [] END OF FILE */
