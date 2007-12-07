/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/*  @(#) PSP/CSL 3.00.01.00[5910PG1_0] (2003-10-15)  */

#ifndef _CSL_VERSION_H_
#define _CSL_VERSION_H_

#define CSL_VERSION_ID          (0x03000100)    /* 0xAABBCCDD -> Arch (AA); API Changes (BB); Major (CC); Minor (DD) */
#define CSL_VERSION_STR         "@# CSL Revision: 3.00.01.00;" 

extern void CSL_version3_00_01_00( );


#define CSL_CHIP_ID             (0x5910)
#define CSL_CHIP_STR            "Chip: TMS320C5910, PG 1.0"

extern void CSL_chip5910PG1_0(void);


static void CSL_versionCheck(void)
{
    CSL_version3_00_00_00();
    CSL_chip5910PG1_0();
}

#endif /* _CSL_VERSION_H_ */

