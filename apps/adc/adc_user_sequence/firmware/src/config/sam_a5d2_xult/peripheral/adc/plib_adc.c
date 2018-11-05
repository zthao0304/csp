/*******************************************************************************
  ADC Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_adc.c

  Summary
    ADC peripheral library source.

  Description
    This file implements the ADC peripheral library.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END
#include "device.h"
#include "plib_adc.h"

#define ADC_SEQ1_CHANNEL_NUM (8U)


// *****************************************************************************
// *****************************************************************************
// Section: ADC Implementation
// *****************************************************************************
// *****************************************************************************
/* Object to hold callback function and context */
ADC_CALLBACK_OBJECT ADC_CallbackObj;

/* Initialize ADC peripheral */
void ADC_Initialize()
{
    /* Software reset */
    ADC_REGS->ADC_CR = ADC_CR_SWRST_Msk;

    /* Prescaler and different time settings as per CLOCK section  */
    ADC_REGS->ADC_MR = ADC_MR_PRESCAL(9U) | ADC_MR_TRACKTIM(15U) | ADC_MR_STARTUP_SUT512 |
        ADC_MR_TRANSFER(2U) | ADC_MR_ANACH_ALLOWED;

    /* resolution and sign mode of result */
    ADC_REGS->ADC_EMR = ADC_EMR_OSR_NO_AVERAGE 
         | ADC_EMR_SIGNMODE_SE_UNSG_DF_SIGN | ADC_EMR_TAG_Msk;

    /* Trigger mode */
    ADC_REGS->ADC_TRGR = ADC_TRGR_TRGMOD_NO_TRIGGER;

    /* User defined channel conversion sequence */
    ADC_REGS->ADC_MR |= ADC_MR_USEQ_Msk;
    ADC_REGS->ADC_SEQR1 = ADC_SEQR1_USCH1(ADC_CH5) | 
		ADC_SEQR1_USCH2(ADC_CH6) | 
		ADC_SEQR1_USCH3(ADC_CH0);
    /* Enable interrupt */
    ADC_REGS->ADC_IER = ADC_IER_EOC0_Msk;
    ADC_CallbackObj.callback_fn = NULL;
    /* Enable channel */
    ADC_REGS->ADC_CHER = ADC_CHER_CH0_Msk | ADC_CHER_CH1_Msk | ADC_CHER_CH2_Msk;
}

/* Enable ADC channels */
void ADC_ChannelsEnable (ADC_CHANNEL_MASK channelsMask)
{
    ADC_REGS->ADC_CHER = channelsMask;
}

/* Disable ADC channels */
void ADC_ChannelsDisable (ADC_CHANNEL_MASK channelsMask)
{
    ADC_REGS->ADC_CHDR = channelsMask;
}

/* Enable channel end of conversion or Comparison event interrupt */
void ADC_ChannelsInterruptEnable (ADC_INTERRUPT_MASK channelsInterruptMask)
{
    ADC_REGS->ADC_IER = channelsInterruptMask;
}

/* Disable channel end of conversion or Comparison event interrupt */
void ADC_ChannelsInterruptDisable (ADC_INTERRUPT_MASK channelsInterruptMask)
{
    ADC_REGS->ADC_IDR = channelsInterruptMask;
}

/* Start the conversion with software trigger */
void ADC_ConversionStart(void)
{
    ADC_REGS->ADC_CR = ADC_CR_START_Msk;
}

/* Check if conversion result is available */
bool ADC_ChannelResultIsReady(ADC_CHANNEL_NUM channel)
{
    return (ADC_REGS->ADC_ISR >> channel) & 0x1U;
}

/* Read the conversion result */
uint16_t ADC_ChannelResultGet(ADC_CHANNEL_NUM channel)
{
    return ADC_REGS->ADC_CDR[channel];
}

/* Configure the user defined conversion sequence */
void ADC_ConversionSequenceSet(ADC_CHANNEL_NUM *channelList, uint8_t numChannel)
{
    uint8_t channelIndex;
    ADC_REGS->ADC_SEQR1 = 0U;
    ADC_REGS->ADC_SEQR2 = 0U;

    for (channelIndex = 0U; channelIndex < ADC_SEQ1_CHANNEL_NUM; channelIndex++)
    {
        if (channelIndex >= numChannel)
            break;
        ADC_REGS->ADC_SEQR1 |= channelList[channelIndex] << (channelIndex * 4U);
    }
    if (numChannel > ADC_SEQ1_CHANNEL_NUM)
    {
        for (channelIndex = 0U; channelIndex < (numChannel - ADC_SEQ1_CHANNEL_NUM); channelIndex++)
        {
            if (channelIndex >= 3)
                break;
            ADC_REGS->ADC_SEQR2 |= channelList[channelIndex + ADC_SEQ1_CHANNEL_NUM] << (channelIndex * 4U);
        }
    }
}

/* Sets Low threshold and High threshold in comparison window */
void ADC_ComparisonWindowSet(uint16_t lowThreshold, uint16_t highThreshold)
{
    ADC_REGS->ADC_CWR = ADC_CWR_LOWTHRES(lowThreshold) | ADC_CWR_HIGHTHRES(highThreshold);
}

/* Check if Comparison event result is available */
bool ADC_ComparisonEventResultIsReady(void)
{
    return (ADC_REGS->ADC_ISR >> ADC_ISR_COMPE_Pos) & 0x1U;
}

/* Restart the comparison function */
void ADC_ComparisonRestart(void)
{
    ADC_REGS->ADC_CR = ADC_CR_CMPRST_Msk;
}

/* Register the callback function */
void ADC_CallbackRegister(ADC_CALLBACK callback, uintptr_t context)
{
    ADC_CallbackObj.callback_fn = callback;
    ADC_CallbackObj.context = context;
}
/* Interrupt Handler */
void ADC_InterruptHandler(void)
{
    if (ADC_CallbackObj.callback_fn != NULL)
    {
        ADC_CallbackObj.callback_fn(ADC_CallbackObj.context);
    }
}
