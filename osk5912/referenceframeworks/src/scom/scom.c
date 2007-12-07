/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.10.00.05 04-29-04 (swat-g05)" */
/*
 *  ======== scom.c ========
 *
 *  Synchronized COMmunication module
 *
 *  Refer to the header file for general information about the module.
 */

/* separate section placement of SCOM functions */
#pragma CODE_SECTION( SCOM_init,     ".text:init"          )
#pragma CODE_SECTION( SCOM_exit,     ".text:exit"          )
#pragma CODE_SECTION( SCOM_create,   ".text:create"        )
#pragma CODE_SECTION( SCOM_delete,   ".text:delete"        )
#pragma CODE_SECTION( SCOM_open,     ".text:SCOM_open"     )
#pragma CODE_SECTION( SCOM_putMsg,   ".text:SCOM_putMsg"   )
#pragma CODE_SECTION( SCOM_getMsg,   ".text:SCOM_getMsg"   )

#include <std.h>

#include <mem.h>
#include <que.h>
#include <sem.h>
#include <utl.h>

#include <string.h>

#include "scom.h"

/* default SCOM creation attributes */
SCOM_Attrs SCOM_ATTRS = { 
    0                      /* dummy */
};

/* definition of an SCOM queue */
struct SCOM_Obj {
    QUE_Elem   elem;         /* mandatory first element */
    String     name;         /* name of the queue */
    QUE_Obj    que;          /* queue on which messages are placed */
    SEM_Obj    sem;          /* semaphore for the queue */
};

/* internal list of SCOM queues */
static QUE_Obj queScomQueues;

/*
 *  ======== SCOM_init ========
 *  
 *  Initializes the master queue.
 */
Void SCOM_init( Void )
{
    QUE_new( &queScomQueues );
}

/*
 *  ======== SCOM_exit ========
 */
Void SCOM_exit( Void )
{
}

/*
 *  ======== SCOM_create ========
 *  
 *  Create a new SCOM queue under given name 
 */
SCOM_Handle SCOM_create( 
    String       queueName,  /* ID (name) of the SCOM queue to be created */
    SCOM_Attrs * attrs       /* SCOM object attributes */
    )
{
    SCOM_Handle  scomQueue;

    /* verify parameters */
    UTL_assert( queueName != NULL );
    
    /* allocate the SCOM queue object */
    scomQueue = (SCOM_Handle)MEM_alloc( 0, sizeof( SCOM_Obj ), 0 );
    if (scomQueue == MEM_ILLEGAL) {
        return NULL;
    }
    
    /* initialize the object */
    QUE_new( &scomQueue->que );
    SEM_new( &scomQueue->sem, 0 );
    scomQueue->name = queueName;

    /* and place it on the master list of SCOM queues */
    QUE_put( &queScomQueues, scomQueue );
    
    return scomQueue;
}

/*
 *  ======== SCOM_open ========
 *  
 *  Get a reference to an existing SCOM queue by its name
 */
SCOM_Handle SCOM_open(
    String       queueName    /* ID (name) of the SCOM queue to be found */
    )
{
    SCOM_Handle  scomQueue;
    
    /* verify parameters */
    UTL_assert( queueName != NULL );

    /* search the master list for a queue whose name is == queueName */
    scomQueue = QUE_head( &queScomQueues );
    while ((QUE_Obj *)scomQueue != &queScomQueues) {
        if (!strcmp( scomQueue->name, queueName )) {
            return scomQueue;    
        }
        scomQueue = QUE_next( scomQueue );
    }
    
    /* not found */
    return NULL;
}

/*
 *  ======== SCOM_delete ========
 *  
 *  Delete the SCOM queue
 */
Bool SCOM_delete( 
    SCOM_Handle  scomQueue     /* handle of the SCOM queue to be deleted */
    )
{
    /* verify parameters */
    UTL_assert( scomQueue != NULL );
    
    QUE_remove( scomQueue );
    return MEM_free( 0, scomQueue, sizeof( SCOM_Obj ) );
}


/*
 *  ======== SCOM_putMsg ========
 *  
 *  Place the message on a synchronized queue
 */
Void SCOM_putMsg(
    SCOM_Handle  scomQueue,   /* SCOM queue to which message should be put */
    Ptr          msg          /* message to be sent */
    )
{
    /* verify parameters */
    UTL_assert( scomQueue != NULL );
    QUE_put ( &scomQueue->que, (QUE_Elem *)msg);
    SEM_post( &scomQueue->sem );
}


/*
 *  ======== SCOM_getMsg ========
 *  
 *  Receive a message from a synchronized queue
 */
Ptr SCOM_getMsg(
    SCOM_Handle  scomQueue,   /* SCOM queue from which to take a message */
    Uns          timeout      /* blocking timeout */
    )
{
    /* verify parameters */
    UTL_assert( scomQueue != NULL );
    
    if (SEM_pend( &scomQueue->sem, timeout ) == FALSE) {
        return NULL;          // semaphore timed out
    }

    return ( QUE_get( &scomQueue->que ) );
}

