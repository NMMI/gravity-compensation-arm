/*******************************************************************************
* File Name: UP_DOWN.h  
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

#if !defined(CY_PINS_UP_DOWN_H) /* Pins UP_DOWN_H */
#define CY_PINS_UP_DOWN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UP_DOWN_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    UP_DOWN_Write(uint8 value) ;
void    UP_DOWN_SetDriveMode(uint8 mode) ;
uint8   UP_DOWN_ReadDataReg(void) ;
uint8   UP_DOWN_Read(void) ;
uint8   UP_DOWN_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define UP_DOWN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define UP_DOWN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define UP_DOWN_DM_RES_UP          PIN_DM_RES_UP
#define UP_DOWN_DM_RES_DWN         PIN_DM_RES_DWN
#define UP_DOWN_DM_OD_LO           PIN_DM_OD_LO
#define UP_DOWN_DM_OD_HI           PIN_DM_OD_HI
#define UP_DOWN_DM_STRONG          PIN_DM_STRONG
#define UP_DOWN_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define UP_DOWN_MASK               UP_DOWN__MASK
#define UP_DOWN_SHIFT              UP_DOWN__SHIFT
#define UP_DOWN_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UP_DOWN_PS                     (* (reg8 *) UP_DOWN__PS)
/* Data Register */
#define UP_DOWN_DR                     (* (reg8 *) UP_DOWN__DR)
/* Port Number */
#define UP_DOWN_PRT_NUM                (* (reg8 *) UP_DOWN__PRT) 
/* Connect to Analog Globals */                                                  
#define UP_DOWN_AG                     (* (reg8 *) UP_DOWN__AG)                       
/* Analog MUX bux enable */
#define UP_DOWN_AMUX                   (* (reg8 *) UP_DOWN__AMUX) 
/* Bidirectional Enable */                                                        
#define UP_DOWN_BIE                    (* (reg8 *) UP_DOWN__BIE)
/* Bit-mask for Aliased Register Access */
#define UP_DOWN_BIT_MASK               (* (reg8 *) UP_DOWN__BIT_MASK)
/* Bypass Enable */
#define UP_DOWN_BYP                    (* (reg8 *) UP_DOWN__BYP)
/* Port wide control signals */                                                   
#define UP_DOWN_CTL                    (* (reg8 *) UP_DOWN__CTL)
/* Drive Modes */
#define UP_DOWN_DM0                    (* (reg8 *) UP_DOWN__DM0) 
#define UP_DOWN_DM1                    (* (reg8 *) UP_DOWN__DM1)
#define UP_DOWN_DM2                    (* (reg8 *) UP_DOWN__DM2) 
/* Input Buffer Disable Override */
#define UP_DOWN_INP_DIS                (* (reg8 *) UP_DOWN__INP_DIS)
/* LCD Common or Segment Drive */
#define UP_DOWN_LCD_COM_SEG            (* (reg8 *) UP_DOWN__LCD_COM_SEG)
/* Enable Segment LCD */
#define UP_DOWN_LCD_EN                 (* (reg8 *) UP_DOWN__LCD_EN)
/* Slew Rate Control */
#define UP_DOWN_SLW                    (* (reg8 *) UP_DOWN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UP_DOWN_PRTDSI__CAPS_SEL       (* (reg8 *) UP_DOWN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UP_DOWN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UP_DOWN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UP_DOWN_PRTDSI__OE_SEL0        (* (reg8 *) UP_DOWN__PRTDSI__OE_SEL0) 
#define UP_DOWN_PRTDSI__OE_SEL1        (* (reg8 *) UP_DOWN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UP_DOWN_PRTDSI__OUT_SEL0       (* (reg8 *) UP_DOWN__PRTDSI__OUT_SEL0) 
#define UP_DOWN_PRTDSI__OUT_SEL1       (* (reg8 *) UP_DOWN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UP_DOWN_PRTDSI__SYNC_OUT       (* (reg8 *) UP_DOWN__PRTDSI__SYNC_OUT) 


#if defined(UP_DOWN__INTSTAT)  /* Interrupt Registers */

    #define UP_DOWN_INTSTAT                (* (reg8 *) UP_DOWN__INTSTAT)
    #define UP_DOWN_SNAP                   (* (reg8 *) UP_DOWN__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins UP_DOWN_H */


/* [] END OF FILE */
