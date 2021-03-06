/*******************************************************************************
* File Name: MOTOR_EN.h  
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

#if !defined(CY_PINS_MOTOR_EN_H) /* Pins MOTOR_EN_H */
#define CY_PINS_MOTOR_EN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MOTOR_EN_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    MOTOR_EN_Write(uint8 value) ;
void    MOTOR_EN_SetDriveMode(uint8 mode) ;
uint8   MOTOR_EN_ReadDataReg(void) ;
uint8   MOTOR_EN_Read(void) ;
uint8   MOTOR_EN_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define MOTOR_EN_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define MOTOR_EN_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define MOTOR_EN_DM_RES_UP          PIN_DM_RES_UP
#define MOTOR_EN_DM_RES_DWN         PIN_DM_RES_DWN
#define MOTOR_EN_DM_OD_LO           PIN_DM_OD_LO
#define MOTOR_EN_DM_OD_HI           PIN_DM_OD_HI
#define MOTOR_EN_DM_STRONG          PIN_DM_STRONG
#define MOTOR_EN_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define MOTOR_EN_MASK               MOTOR_EN__MASK
#define MOTOR_EN_SHIFT              MOTOR_EN__SHIFT
#define MOTOR_EN_WIDTH              2u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MOTOR_EN_PS                     (* (reg8 *) MOTOR_EN__PS)
/* Data Register */
#define MOTOR_EN_DR                     (* (reg8 *) MOTOR_EN__DR)
/* Port Number */
#define MOTOR_EN_PRT_NUM                (* (reg8 *) MOTOR_EN__PRT) 
/* Connect to Analog Globals */                                                  
#define MOTOR_EN_AG                     (* (reg8 *) MOTOR_EN__AG)                       
/* Analog MUX bux enable */
#define MOTOR_EN_AMUX                   (* (reg8 *) MOTOR_EN__AMUX) 
/* Bidirectional Enable */                                                        
#define MOTOR_EN_BIE                    (* (reg8 *) MOTOR_EN__BIE)
/* Bit-mask for Aliased Register Access */
#define MOTOR_EN_BIT_MASK               (* (reg8 *) MOTOR_EN__BIT_MASK)
/* Bypass Enable */
#define MOTOR_EN_BYP                    (* (reg8 *) MOTOR_EN__BYP)
/* Port wide control signals */                                                   
#define MOTOR_EN_CTL                    (* (reg8 *) MOTOR_EN__CTL)
/* Drive Modes */
#define MOTOR_EN_DM0                    (* (reg8 *) MOTOR_EN__DM0) 
#define MOTOR_EN_DM1                    (* (reg8 *) MOTOR_EN__DM1)
#define MOTOR_EN_DM2                    (* (reg8 *) MOTOR_EN__DM2) 
/* Input Buffer Disable Override */
#define MOTOR_EN_INP_DIS                (* (reg8 *) MOTOR_EN__INP_DIS)
/* LCD Common or Segment Drive */
#define MOTOR_EN_LCD_COM_SEG            (* (reg8 *) MOTOR_EN__LCD_COM_SEG)
/* Enable Segment LCD */
#define MOTOR_EN_LCD_EN                 (* (reg8 *) MOTOR_EN__LCD_EN)
/* Slew Rate Control */
#define MOTOR_EN_SLW                    (* (reg8 *) MOTOR_EN__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MOTOR_EN_PRTDSI__CAPS_SEL       (* (reg8 *) MOTOR_EN__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MOTOR_EN_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MOTOR_EN__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MOTOR_EN_PRTDSI__OE_SEL0        (* (reg8 *) MOTOR_EN__PRTDSI__OE_SEL0) 
#define MOTOR_EN_PRTDSI__OE_SEL1        (* (reg8 *) MOTOR_EN__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MOTOR_EN_PRTDSI__OUT_SEL0       (* (reg8 *) MOTOR_EN__PRTDSI__OUT_SEL0) 
#define MOTOR_EN_PRTDSI__OUT_SEL1       (* (reg8 *) MOTOR_EN__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MOTOR_EN_PRTDSI__SYNC_OUT       (* (reg8 *) MOTOR_EN__PRTDSI__SYNC_OUT) 


#if defined(MOTOR_EN__INTSTAT)  /* Interrupt Registers */

    #define MOTOR_EN_INTSTAT                (* (reg8 *) MOTOR_EN__INTSTAT)
    #define MOTOR_EN_SNAP                   (* (reg8 *) MOTOR_EN__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins MOTOR_EN_H */


/* [] END OF FILE */
