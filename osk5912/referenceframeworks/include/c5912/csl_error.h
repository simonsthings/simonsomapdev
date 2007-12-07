/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
 \*****************************************************/

#ifndef _CSL_ERROR_H_
#define _CSL_ERROR_H_

#include <csl_resId.h>

/* Below Error codes are Global across all CSL Modules. */
#define CSL_SOK                         (1)             /* Success */
#define CSL_ESYS_FAIL               (-1)                /* Generic failure */
#define CSL_ESYS_INUSE              (-2)                /* Peripheral resource is already in use */
#define CSL_ESYS_XIO                (-3)                /* Encountered a shared I/O(XIO) pin conflict */
#define CSL_ESYS_OVFL               (-4)                /* Encoutered CSL system resource overflow */
#define CSL_ESYS_BADHANDLE          (-5)                /* Handle passed to CSL was invalid */
#define CSL_ESYS_INVPARAMS      (-6)        /* invalid parameters */
#define CSL_ESYS_INVCMD         (-7)        /* invalid command */
#define CSL_ESYS_INVQUERY       (-8)        /* invalid query */
#define CSL_ESYS_NOTSUPPORTED   (-9)        /* action not supported */


/* Error codes individual to various modules. */

/* Error code for UART, individual error would be assigned as
 * eg: #define CSL_E<Peripheral name>i_<error code>     CSL_EUART_FIRST - 0
 */
#define CSL_EDMA_FIRST          -( ((CSL_DMA_ID + 1) << 5 ) + 1 )
#define CSL_EDMA_LAST           -( (CSL_DMA_ID + 1) << 6 )

#define CSL_ETIMER_FIRST        -( ((CSL_TIMER_ID + 1) << 5 ) + 1 )
#define CSL_ETIMER_LAST         -( (CSL_TIMER_ID + 1) << 6 )

#define CSL_EMCBSP_FIRST        -( ((CSL_MCBSP_ID + 1) << 5 ) + 1 )
#define CSL_EMCBSP_LAST         -( (CSL_MCBSP_ID + 1) << 6 )

#define CSL_EINTC_FIRST         -( ((CSL_INTC_ID + 1) << 5 ) + 1 )
#define CSL_EINTC_LAST          -( (CSL_INTC_ID + 1) << 6 )

#define CSL_EUART_FIRST         -( ((CSL_UART_ID + 1) << 5 ) + 1 )
#define CSL_EUART_LAST          -( (CSL_UART_ID + 1) << 6 )

#define CSL_EIRDA_FIRST         -( ((CSL_IRDA_ID + 1) << 5 ) + 1 )
#define CSL_EIRDA_LAST          -( (CSL_IRDA_ID + 1) << 6 )

#define CSL_EI2C_FIRST          -( ((CSL_I2C_ID + 1) << 5 ) + 1 )
#define CSL_EI2C_LAST           -( (CSL_I2C_ID + 1) << 6 )

#define CSL_ECLKRST_FIRST       -( ((CSL_CLKRST_ID + 1) << 5 ) + 1 )
#define CSL_ECLKRST_LAST        -( (CSL_CLKRST_ID + 1) << 6 )

#endif /* _CSL_ERROR_H_ */

