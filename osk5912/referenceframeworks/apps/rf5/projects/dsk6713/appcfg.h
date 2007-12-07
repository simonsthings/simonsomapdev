/*   Do *not* directly modify this file.  It was    */
/*   generated by the Configuration Tool; any  */
/*   changes risk being overwritten.                */

/* INPUT app.cdb */

#define CHIP_6713 1

/*  Include Header Files  */
#include <std.h>
#include <hst.h>
#include <swi.h>
#include <tsk.h>
#include <log.h>
#include <sts.h>

#ifdef __cplusplus
extern "C" {
#endif

extern far HST_Obj RTA_fromHost;
extern far HST_Obj RTA_toHost;
extern far SWI_Obj KNL_swi;
extern far TSK_Obj TSK_idle;
extern far TSK_Obj tskRxSplit;
extern far TSK_Obj tskTxJoin;
extern far TSK_Obj tskProcess;
extern far TSK_Obj tskControl;
extern far LOG_Obj LOG_system;
extern far LOG_Obj logTrace;
extern far STS_Obj IDL_busyObj;
extern far STS_Obj stsTime0;
extern far STS_Obj stsTime1;
extern far STS_Obj stsTime2;
extern far STS_Obj stsTime3;
extern far STS_Obj stsTime4;
extern far STS_Obj stsTime5;
extern far STS_Obj stsTime6;
extern far STS_Obj stsTime7;
extern far STS_Obj stsTime8;
extern far STS_Obj stsTime9;
extern far void CSL_cfgInit();

#ifdef __cplusplus
}
#endif /* extern "C" */
