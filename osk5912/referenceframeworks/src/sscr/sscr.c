/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== sscr.c ========
 *  Globals for the SSCR module
 */

#include <std.h>

#include <sscr.h>

/* Heap Id used to allocate scratch buffers for the XDAIS algorithms */
Int _SSCR_heapId = _SSCR_INVALIDHEAP;

/* Number of buckets created */
Int _SSCR_bucketCnt = 0;

/* Table to hold scratch buckets */
SSCR_Bucket *_SSCR_bucketList = NULL;


