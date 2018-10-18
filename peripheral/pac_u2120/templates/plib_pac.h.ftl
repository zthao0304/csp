/*******************************************************************************
  Peripheral Access Controller(PAC) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_${PAC_INSTANCE_NAME?lower_case}.h

  Summary
    PAC PLIB Header File.

  Description
    This file defines the interface to the PAC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/* Guards against multiple inclusion */
#ifndef PLIB_${PAC_INSTANCE_NAME}_H
#define PLIB_${PAC_INSTANCE_NAME}_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "device.h"
#include "plib_pac_common.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* The following data type definitions are used by the functions in this
    interface and should be considered part it.
*/

// *****************************************************************************
/* Peripheral module Identifier enumeration

  Summary:
    List of available Peripheral module on which errors will be detected.

  Description:
    This enumeration identifies all the Peripheral modules used on which access
    errors will be detected.

  Remarks:
    None.
*/

typedef enum
{
<#list 0..PAC_BRIDGE_COUNT as i>
    <#assign PAC_BRIDGE = "PAC_" + i + "_BRIDGE">
    <#assign PAC_BRIDGE_PERI_COUNT = "PAC_BRIDGE_" + i + "_PERI_COUNT">
        <#if .vars[PAC_BRIDGE_PERI_COUNT]?has_content>
            <#assign PAC_PERI_COUNT = .vars[PAC_BRIDGE_PERI_COUNT]>
            <#list 0..PAC_PERI_COUNT as j>
                <#assign PAC_BRIDGE_PERI_NAME = "PAC_BRIDGE_" + i + "_PERI_" + j + "_NAME">
                    <#if .vars[PAC_BRIDGE_PERI_NAME]?has_content>
    <#lt>    /* Interrupt flag for Peripheral bridge ${.vars[PAC_BRIDGE]} - ${.vars[PAC_BRIDGE_PERI_NAME]} */
    <#lt>    PAC_PERIPHERAL_${.vars[PAC_BRIDGE]}_${.vars[PAC_BRIDGE_PERI_NAME]} = ${((i * 32 ) + j)},

                    </#if>
            </#list>
        </#if>
</#list>
    /* Interrupt flag for all Peripheral bridges */
    PAC_PERIPHERAL_ANY = 0xFFFFFFFF

} PAC_PERIPHERAL;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/* The following functions make up the methods (set of possible operations) of
    this interface.
*/

void ${PAC_INSTANCE_NAME}_Initialize( void );

void ${PAC_INSTANCE_NAME}_PeripheralProtectSetup( PAC_PERIPHERAL peripheral, PAC_PROTECTION operation );

<#if PAC_INTERRRUPT_MODE = true>
void ${PAC_INSTANCE_NAME}_CallbackRegister( PAC_CALLBACK callback, uintptr_t context );
</#if>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

#endif /* PLIB_${PAC_INSTANCE_NAME}_H */
