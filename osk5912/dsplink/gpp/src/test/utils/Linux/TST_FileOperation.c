/** ============================================================================
 *  @file   TST_FileOperation.c
 *
 *  @path   $(DSPLINK)\gpp\src\test\utils
 *
 *  @desc   Implementation of file operation functions.
 *
 *  @ver    01.10.01
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2004
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */


/*  ----------------------------------- OS Specific Headers           */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*  ----------------------------------- DSP/BIOS Link                 */
#include <gpptypes.h>
#include <errbase.h>

/*  ----------------------------------- Trace & Debug                 */
#include <TST_Dbc.h>

/*  ----------------------------------- Test framework                */
#include <TST_Framework.h>
#include <TST_PrintFuncs.h>
#include <TST_FileOperation.h>
#include <TST_Helper.h>
#include <TST_Mem.h>


#if defined (__cplusplus)
EXTERN "C" {
#endif /* defined (__cplusplus) */


/** ============================================================================
 *  @name   INSIDE
 *
 *  @desc   A constant defining if the current position is INSIDE the word.
 *  ============================================================================
 */
#define INSIDE  0

/** ============================================================================
 *  @name   OUTSIDE
 *
 *  @desc   A constant defining if the current position is OUTSIDE the word.
 *  ============================================================================
 */
#define OUTSIDE 1

/** ============================================================================
 *  @name   COMMENT_CHAR
 *
 *  @desc   A constant defining the character to be used for comments in data
 *          files.
 *  ============================================================================
 */
#define COMMENT_CHAR  '!'

/** ============================================================================
 *  @name   COMMENT_TEST_CHAR
 *
 *  @desc   A constant defining the character to be used for commenting tests
 *          in data files.
 *  ============================================================================
 */
#define COMMENT_TEST_CHAR  '#'


/** ============================================================================
 *  @func   TST_ReadLine
 *
 *  @desc   It  parses the data file to reads one line.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_ReadLine (IN Void *     filePtr,
              OUT Uint32 *  argc,
              OUT Char8     argv [] [MAX_BUFSIZE],
              OUT Bool *    isTestCommented)
{
    DSP_STATUS status    = DSP_SOK ;
    Char8      buf       = ' '     ;
    Int32      index     = 0       ;
    Bool       flag      = TRUE    ; /* For detecting EOF condition */
    Int32      where     = OUTSIDE ; /* For keping track of the posiiton
                                        of current position*/
    Bool       isComment = FALSE   ; /* For keeping track if the current
                                        line is a comment */
    Bool       isFirst   = TRUE    ; /* For Dealing with blank lines */
    int osStatus         = 0          ;

    TST_PrnDebug ("Entered TST_ReadLine ()") ;

    DBC_Require (filePtr != NULL) ;
    DBC_Require (argc != NULL) ;
    DBC_Require (argv != NULL) ;

    if ((filePtr == NULL)
        || (argc == NULL)
        || (argv == NULL)) {
        status = DSP_EINVALIDARG ;
    }

    if (DSP_SUCCEEDED (status)) {
        *argc = 0 ;
        *isTestCommented = FALSE ;
        while ((((buf != '\n')
               || (isFirst == TRUE)
               || (isComment == TRUE))
               && (osStatus >= 0)
               && (flag != FALSE))) {
            osStatus = read (*((int*)filePtr), (void *) &buf, 1) ;
            if (osStatus == 0) {
                flag = FALSE ;
                status = DSP_SOK ;
                if (isComment == FALSE) {
                    if (where == INSIDE) {
                        argv [*argc] [index] = '\0' ;
                        (*argc)++ ;
                    }
                }  /* isComment == FALSE */
            }      /* status == DSP_ERANGE */
            else {
                if (buf == COMMENT_CHAR) {
                    isComment = TRUE ;
                    *isTestCommented = FALSE ;
                }
                else {
                    if (ISSPACE (buf)) {
                        if ((where == INSIDE) || (isComment == TRUE)) {
                            if (isComment == FALSE) {
                                argv [*argc] [index] = '\0' ;
                                (*argc)++ ;
                                where  = OUTSIDE ;
                                index  = 0 ;
                            }
                            if (buf == '\n') {
                                if (isComment == TRUE) {
                                    isComment = FALSE ;
                                    buf = ' ' ;
                                }
                            }
                        }
                    }
                    else if (   (isFirst == TRUE) 
                             && (buf == COMMENT_TEST_CHAR)
                             && (isComment == FALSE)) {
                        *isTestCommented = TRUE ;
                    }
                    else {
                        if (isComment == FALSE) {
                            isFirst = FALSE ;
                            argv [*argc] [index] = buf ;
                            where = INSIDE ;
                            index ++ ;
                        }
                    }
                }
            }
        }
    }
    TST_PrnDebug ("Leaving TST_ReadLine ()") ;
    return status ;
}


/** ============================================================================
 *  @func   TST_OpenFile
 *
 *  @desc   This function opens a given file and returns a file pointer.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_OpenFile (IN Char8 * fileName, IN Void ** filePtr)
{
    DSP_STATUS status = DSP_SOK ;
    int fd = 0 ;
    int * ptr ;

    TST_PrnDebug ("Entered TST_OpenFile ()") ;

    DBC_Require (fileName != NULL) ;
    DBC_Require (filePtr != NULL) ;

    if (fileName == NULL) {
        status = DSP_EINVALIDARG ;
    }

    if (DSP_SUCCEEDED (status)) {
        fd = open (fileName, O_RDONLY) ;
        if (fd < 0) {
            status = DSP_EFAIL ;
            *filePtr = NULL ;
        }
        else {
            status = TST_Alloc ((Void *)&ptr, 1) ;
            if (DSP_SUCCEEDED (status)) {
                *ptr = fd ;
                *filePtr = ptr ;
            }
        }
    }

    TST_PrnDebug ("Leaving TST_OpenFile ()") ;

    return status ;
}


/** ============================================================================
 *  @func   TST_CloseFile
 *
 *  @desc   This function accepts a filehandle and closes that file.
 *
 *  @modif  None
 *  ============================================================================
 */
EXPORT_API
DSP_STATUS
TST_CloseFile (IN Void ** filePtr)
{
    DSP_STATUS status = DSP_SOK ;
    int osStatus ;
    int fd ;

    TST_PrnDebug ("Entered TST_CloseFile ()") ;

    fd = **((int**)filePtr) ;
    osStatus = close (fd) ;

    if (fd < 0) {
        status = DSP_EFAIL ;
    }

    TST_Free (filePtr) ;

    TST_PrnDebug ("Leaving TST_CloseFile ()") ;

    return status ;
}


#if defined (__cplusplus)
}
#endif /* defined (__cplusplus) */
