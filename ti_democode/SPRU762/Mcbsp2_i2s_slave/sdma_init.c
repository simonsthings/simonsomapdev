#include <csl_dma.h>
#include <soc.h>

extern CSL_DmaObj sdmaObjXmit, sdmaObjRcv;

/** This function obtains a handle for the DMA Channel0 Object
 *
 *  The function does the following steps :-
 *  -# Obtains a handle to Channel0 DMA object.
 *  -# Checks if Open call to get the handle for Channel0 DMA object
 *     succeeded.  On success returns 1 else 0
 *
 *  @return returns an int: 1 on success and 0 on failure
 */
int sdmaInit(CSL_DmaHandle *sdmaHndlXmitptr, CSL_DmaHandle *sdmaHndlRcvptr)
{
   Bool openFailure,validSdmaHndl;

   CSL_Status sdmaStatusXmit;
   CSL_DmaParam  dmaParamXmit;

   CSL_Status sdmaStatusRcv;
   CSL_DmaParam  dmaParamRcv;

	dmaParamXmit.chaNum = CSL_DMA_CHA0;
	dmaParamRcv.chaNum = CSL_DMA_CHA1;

	/* DMA Initialization */
	CSL_dmaInit(NULL);

	/* Open DMA handles */
   *sdmaHndlXmitptr = CSL_dmaOpen(&sdmaObjXmit, CSL_DMA, &dmaParamXmit, &sdmaStatusXmit);
   *sdmaHndlRcvptr = CSL_dmaOpen(&sdmaObjRcv, CSL_DMA, &dmaParamRcv, &sdmaStatusRcv);

   if (sdmaStatusXmit ) { // DMA Open failed
       openFailure = TRUE;
   }
   else {
       openFailure = FALSE;
   }

   if (*sdmaHndlXmitptr != NULL) {
      validSdmaHndl = TRUE;
   }
   else {
      validSdmaHndl = FALSE;
   }

   if ( (openFailure == FALSE) || (validSdmaHndl == FALSE) ) {
      return(0);
   }


   if (sdmaStatusRcv ) { // DMA Open failed
       openFailure = TRUE;
   }
   else {
       openFailure = FALSE;
   }

   if (*sdmaHndlRcvptr != NULL) {
      validSdmaHndl = TRUE;
   }
   else {
      validSdmaHndl = FALSE;
   }

   if ( (openFailure == FALSE) || (validSdmaHndl == FALSE) ) {
      return(0);
   }

   return 1;
}
