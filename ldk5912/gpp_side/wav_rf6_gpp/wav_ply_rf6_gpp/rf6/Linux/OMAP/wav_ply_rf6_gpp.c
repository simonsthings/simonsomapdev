/** ============================================================================
 *  @file   message.c
 *
 *  @path   $(DSPLINK)/gpp/src/samples/message/
 *
 *  @desc   This is an application which sends messages to the DSP
 *          processor and receives them back using DSP/BIOS LINK.
 *          The message contents received are verified against the data
 *          sent to DSP.
 *
 *  @ver    01.30.06
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2002-2005
 *
 *  Use of this software is controlled by the terms and conditions found in the
 *  license agreement under which this software has been supplied or provided.
 *  ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/ioctl.h>

/* Linux specific include */
#include <linux/soundcard.h>

/*  ----------------------------------- DSP/BIOS Link                   */
#include <gpptypes.h>
#include <dsplink.h>
#include <errbase.h>
#include <platform.h>
#include <msgqdefs.h>
#include <proc.h>
#include <chnl.h>
#include <msgq.h>
#include <pool.h>

/*  ----------------------------------- Profiling                       */
#include <profile.h>



/* ----------------------------- DSPLINK 1.30 Related PreProcessor Definitions             */

/** ============================================================================
 *  @const  ID_PROCESSOR
 *
 *  @desc   The processor id of the processor being used.
 *  ============================================================================
 */
#define ID_PROCESSOR       0


/** ============================================================================
 *  @const  CHNL_ID_OUTPUT
 *
 *  @desc   ID of channel used to send data to DSP.
 *  ============================================================================
 */
#define CHNL_ID_OUTPUT     0


/** ============================================================================
 *  @const  CHNL_ID_INPUT
 *
 *  @desc   ID of channel used to receive data from DSP.
 *  ============================================================================
 */
#define CHNL_ID_INPUT      1
/*  ============================================================================
 *  @const  NUM_ARGS
 *
 *  @desc   Number of arguments specified to the DSP application.
 *  ============================================================================
 */
#define NUM_ARGS             2

/** ============================================================================
 *  @name   SAMPLE_POOL_ID
 *
 *  @desc   ID of the POOL used for the sample.
 *  ============================================================================
 */
#define SAMPLE_POOL_ID       0

/** ============================================================================
 *  @name   NUM_BUF_SIZES
 *
 *  @desc   Number of buffer pools to be configured for the allocator.
 *  ============================================================================
 */
#define NUM_BUF_SIZES        4

/** ============================================================================
 *  @const  NUM_BUF_POOL0
 *
 *  @desc   Number of buffers in first buffer pool.
 *  ============================================================================
 */
#define NUM_BUF_POOL0        1

/** ============================================================================
 *  @const  NUM_BUF_POOL1
 *
 *  @desc   Number of buffers in second buffer pool.
 *  ============================================================================
 */
#define NUM_BUF_POOL1        2

/** ============================================================================
 *  @const  NUM_BUF_POOL2
 *
 *  @desc   Number of buffers in third buffer pool.
 *  ============================================================================
 */
#define NUM_BUF_POOL2        1

/** ============================================================================
 *  @name   NUM_BUF_POOL3
 *
 *  @desc   Number of buffers in fourth buffer pool.
 *  ============================================================================
 */
#define NUM_BUF_POOL3        2

/** ============================================================================
 *  @name   SAMPLE_CTRLMSG_SIZE
 *
 *  @desc   Control message size.
 *  ============================================================================
 */
#if defined (ZCPY_LINK)
#define SAMPLE_CTRLMSG_SIZE ZCPYMQT_CTRLMSG_SIZE
#endif /* if defined (ZCPY_LINK) */

/** ============================================================================
 *  @const  MsgqDsp1
 *
 *  @desc   Name of the first MSGQ on the DSP.
 *  ============================================================================
 */
STATIC Char8 SampleDspMsgqName [DSP_MAX_STRLEN] = "DSPMSGQ1" ;

/** ============================================================================
 *  @name   SampleDspMsgq
 *
 *  @desc   DSP's MSGQ Object.
 *  ============================================================================
 */
STATIC MsgqQueue SampleDspMsgq = (Uint32) MSGQ_INVALIDMSGQ ;


/* ------------------------------------------------- Wavplayer Application PreProcessor Definitions           */

/** ============================================================================
 *  @const  CTRL_MSGID
 *
 *  @desc   Message id to denote a message transfer.
 *  ============================================================================
 */
#define CTRL_MSGID            1

/** ============================================================================
 *  @const  APP_MSG_SIZE
 *
 *  @desc   Messaging buffer used by the application.
 *          Note: This buffer is aligned according to the alignment expected
 *          by the platform.
 *  ============================================================================
 */
//#define APP_MSG_SIZE  ALIGN (sizeof (CtrlMsg), DSPLINK_BUF_ALIGN)
#define APP_MSG_SIZE  sizeof (CtrlMsg)

/* 1024 * 7 is the size used due to accomodate a sampling rate of upto 88KHz   */ 
/* Minimum possible multiple is 1024*1                                         */
#define BUFFERSIZE             1024*7
#define NUMOUTBUFS             1

/* Audio data Stereo (L and R) and 16 bits/sample */
#define STEREO                 2
#define BITSPERSAMPLE          16

/* Max size of the user input. */
#define USERINPUTMAXNUMCHARS   70

/* Used in processing user input */
#define DELIMITERS              " "

/* Default configuration values */
#define DEFAULT_VOLUME          100

#define DEFAULT_COEFF           0

// path of the audio device and it's file descriptor
#define CODEC_DEV               "/dev/dsp"


/* Commands to the DSP.  Must match on DSP side */
enum {
    MSGNEWVOL = 0,
    MSGNEWCOEFF,
    MSGQUIT
};

/*
 *  The following describe the state that the application is in to allow
 *  a graceful clear-up of DSP/BIOS Link.
 */
enum {
    APPLSTATE0 = 0,
    APPLSTATE1,
    APPLSTATE2,
    APPLSTATE3,
    APPLSTATE4,
    APPLSTATE5,
    APPLSTATE6,
    APPLSTATE7,
    APPLSTATE8
};

/*
 *  The following describe the state that the audio thread is in to allow
 *  a graceful clear-up of DSP/BIOS Link.
 */
enum {
    AUDIOSTATE0 = 0,
    AUDIOSTATE1,
    AUDIOSTATE2,
    AUDIOSTATE3,
    AUDIOSTATE4,
    AUDIOSTATE5
};


/* flag used in shutdown */
static int flag = 1;

int codecFd = 0;

/* wave file pointer, path/filename string*/
FILE *fp;
char  g_path[1024];


/** ============================================================================
 *  @name   CtrlMsg
 *
 *  @desc   Structure used to pass the scaling factor from the GPP to the DSP.
 *
 *  @field  msgHeader
 *              Required first field of a message.
 *  @field  cmd
 *              Used to change volume 
 *                      change coefficient
 *      		quit   application
 *  @field  arg1 0-mute 1-valid value
 *  @field  arg2 DEFAULT VOLUME
 *  ============================================================================
 */

typedef struct CtrlMsg {
    MsgqMsgHeader msgHeader;
    uint16_t cmd;    // Message code
    uint16_t arg1;   // First message argument
    uint16_t arg2;   // Second message argument
} CtrlMsg;


/*  ============================================================================
 *  @name   WAV_BufferSize
 *
 *  @desc   Size of buffer to be used for data transfer.
 *  ============================================================================
 */
STATIC Uint32  WAV_BufferSize ;
//const Uint32  WAV_BufferSize;

/** ============================================================================
 *  @name   WAV_Buffers
 *
 *  @desc   Array of buffers used by input channel.
 *          Length of array in this application is 1.
 *  ============================================================================
 */
STATIC unsigned short * WAV_Buffers [NUMOUTBUFS] ;


// wave file header structure
typedef struct{
  unsigned int filesize;
  unsigned int wavefmtsize;
  unsigned short compression_code;
  unsigned short numberofchannels;
  unsigned int samplingrate;
  unsigned int byterate;
  unsigned short blockalign;
  unsigned short sigbits;
  unsigned short extrafmtsize;
  unsigned short extrafmt;
  unsigned short sampleperblock;
  unsigned short numofcoef;
  short coef[7][2];
  unsigned int factsize;
  unsigned int numberofsamples;
  unsigned int datasize;

}WAV_HEADER;

// global header variable
WAV_HEADER header;

/* Function prototypes for local functions */
static void terminateAppl( char *errString, unsigned int runState );
static void terminateThread( char *errString, unsigned int runState );
static void configureCodec( int sampleFreq );
static void initDataStreaming( void );
static void primeDataStreaming( void );
static void runDataStreaming( void );
static void *streamThr( void *arg );
static int snd_getWavHeader(void);
static void sendControlMsg( unsigned short int cmd,
                            unsigned short int arg1,
                            unsigned short int arg2 );

/* Streaming thread handle */
pthread_t streamThread;


/*******************Wavplayer data*****************************************************************/



int main(int argc, char *argv[])
{
    char inbuf[USERINPUTMAXNUMCHARS];
    char *ptr;
    int val;    
    int sampleFreq = 44100;
    DSP_STATUS    status                  = DSP_SOK ;
    Uint32        size    [NUM_BUF_SIZES] = {APP_MSG_SIZE,
                                             SAMPLE_CTRLMSG_SIZE,
                                             ALIGN (sizeof (MsgqAsyncLocateMsg),
                                                    DSPLINK_BUF_ALIGN)} ;
    Uint32        numBufs [NUM_BUF_SIZES] = {NUM_BUF_POOL0,
                                             NUM_BUF_POOL1,
                                             NUM_BUF_POOL2,
                                             NUM_BUF_POOL3} ;

    
#if defined (ZCPY_LINK)
    SmaPoolAttrs  poolAttrs ;
    ZcpyMqtAttrs  mqtAttrs ;
#endif /* if defined (ZCPY_LINK) */


    MsgqLocateAttrs syncLocateAttrs ;

     printf( "\nReference Framework 6 audio application started\n\n" );
    
      strcpy(g_path,argv[2]);
      if (snd_getWavHeader()==-1) {
      exit(0);
      }
	
	// we need to configure the codec before setting up the DSP
     configureCodec(2);
    
    
    
     WAV_BufferSize = BUFFERSIZE;
				  
   
    /*
     *  Create and initialize the proc object.
     */
    status = PROC_Setup () ;

    /*
     *  Attach the Dsp with which the transfers have to be done.
     */
    if (DSP_SUCCEEDED (status)) {
    printf ("PROC_Setup () success!!!!!!!!!!!!!\n\n");
        status = PROC_Attach (ID_PROCESSOR, NULL) ;
        if (DSP_FAILED (status)) {
            terminateAppl( "Error: Failed PROC_Attach()", APPLSTATE2 );
        }
    }
    else {
         terminateAppl( "Error: Failed PROC_Setup()", APPLSTATE0 );
    }

    /*
     *  Open the pool.
     */
    if (DSP_SUCCEEDED (status)) {
    printf ("PROC_Attach () success!!!!!!!!!!!!!\n\n");
        size [3] = WAV_BufferSize ;
        poolAttrs.bufSizes      = (Uint32 *) &size ;
        poolAttrs.numBuffers    = (Uint32 *) &numBufs ;
        poolAttrs.numBufPools   = NUM_BUF_SIZES ;
#if defined (ZCPY_LINK)
        poolAttrs.exactMatchReq = TRUE ;
#endif /* if defined (ZCPY_LINK) */
        status = POOL_Open (SAMPLE_POOL_ID, &poolAttrs) ;
        if (DSP_FAILED (status)) {
           terminateAppl( "Error: Failed POOL_Open()", APPLSTATE3 );
        }
    }

    /*
     *  Load the executable on the DSP.
     */
    if (DSP_SUCCEEDED (status)) {
        printf ("POOL_Open () success!!!!!!!!!!!!!\n\n");
        status = PROC_Load(ID_PROCESSOR, argv[1], 0, NULL);
        if (DSP_FAILED (status)) {
            terminateAppl( "Error: Failed to load executable to DSP, terminating application...", APPLSTATE4 );
        }
    }

   

    /*
     *  Start execution on DSP.
     */
    if (DSP_SUCCEEDED (status)) {
    printf ("Proc_Load () success!!!!!!!!!!!!!\n\n");
        status = PROC_Start (ID_PROCESSOR) ;
        if (DSP_FAILED (status)) {
            terminateAppl( "Error: Failed to start the DSP,terminating application...", APPLSTATE4 );
        }
    }
    
    printf ("PROC_Start () success!!!!!!!!!!!!!\n\n");
#ifdef DDSP_DEBUG  
  printf("Attach emulator and run DSP, then press return to continue: \n");
  getchar();
  printf("Continuing\n");
#endif


    /*
     *  Open the remote transport.*/
     
    if (DSP_SUCCEEDED (status)) {
        mqtAttrs.poolId = SAMPLE_POOL_ID ;
        status = MSGQ_TransportOpen (ID_PROCESSOR, &mqtAttrs) ;
        if (DSP_FAILED (status)) {
           terminateAppl( "Error: Failed to Open the Transport to DSP,terminating application...", APPLSTATE3 );
        }
    }

     /*
     *  Locate the DSP's message queue*/
     
    if (DSP_SUCCEEDED (status)) {
    printf ("MSGQ_TransportOpen of DSP message queue done successfully!!!!!!!!!!!!\n\n") ; 
        syncLocateAttrs.timeout = WAIT_FOREVER;
        status = DSP_ENOTFOUND ;
        while (status == DSP_ENOTFOUND) {
            status = MSGQ_Locate (SampleDspMsgqName,
                                  &SampleDspMsgq,
                                  &syncLocateAttrs) ;
            if (status == DSP_ENOTFOUND) {
                usleep (100000) ;
            }
            else if (DSP_FAILED (status)) {
                terminateAppl( "Error: Failed to Open the Transport to DSP,terminating application...", APPLSTATE4 );
            }
        }
    }
    printf ("MSGQ_Locate of DSP message queue done successfully!!!!!!!!!!!!\n\n") ; 
    
    
    /* Create and start the thread handling the streaming of data
   * to and from the DSP.
   */
    printf("Initializing the data streaming thread\n");
    if (pthread_create(&streamThread, NULL, streamThr, (void *) sampleFreq)) {
      printf( "Error: Failed to create stream thread\n\n");        
    }
    

  sendControlMsg( MSGNEWVOL, 0, DEFAULT_VOLUME );
  sendControlMsg( MSGNEWVOL, 1, DEFAULT_VOLUME );
    
  printf("\nCommands:\n");
  printf("   Change volume      - v <volume 0-200>  e.g. 'v 100'\n");
  printf("   Quit               - q\n\n");
  printf("> ");
    
    printf("\n Thread created\n");
    
    while ( flag ) {

        /* Get a line from standard input */
        if ( fgets( inbuf, USERINPUTMAXNUMCHARS, stdin ) == NULL ) {

	    terminateAppl( "Error: Failed to get command", APPLSTATE8 );
        }

        /* Parse the string for the command */
        ptr = strtok( inbuf, DELIMITERS );

        if ( ptr != NULL ) {
            switch ( *ptr ) {
	     case 'v':
                /* Change the volume on the DSP */
                ptr = strtok( NULL, DELIMITERS );

                if ( ptr == NULL ) {
                    break;
                }

                val = atoi(ptr);

                if ( val < 0 || val > 200 ) {
                    printf("%d is not a valid volume parameter\n> ", val);
                    break;
                }

                /* Send the new volume for both left and right channels */
                sendControlMsg( MSGNEWVOL, 0, val );
                sendControlMsg( MSGNEWVOL, 1, val );

                printf("Message sent\n> ");
                break;
            case 'q':
                printf("Received quit command, exiting...\n");
		sendControlMsg( MSGQUIT, 0, 0 );
                flag = 0;
                break;

            default:
                printf( "Unrecognized command, try again\n> " );
            }
        }
    }    

    printf( "\n Leaving Reference Framework 6 audio application\n\n" );
        
    terminateAppl( "Clean-up completed.", APPLSTATE7 );
   
    return status ;
}

/*
 *  ======== terminateAppl ========
 *
 */
 
 
static void terminateAppl( char *errString, unsigned int runState )
{ 
    DSP_STATUS status = DSP_SOK ;  
    switch ( runState ) {
        case APPLSTATE7:           
            flag = 0;
	    pthread_join( streamThread, NULL );
	case APPLSTATE6:
	   MSGQ_Release ( SampleDspMsgq );
	   printf("\n Message queue released\n");
        case APPLSTATE5:
           MSGQ_TransportClose ( ID_PROCESSOR );
           printf("\n MSGQ Transport Closed\n");
        case APPLSTATE4:
           PROC_Stop( ID_PROCESSOR );  
           printf("\n DSP PROCESSOR Stopped\n");
        case APPLSTATE3:
           status = POOL_Close (SAMPLE_POOL_ID) ;
	   if(status == DSP_SOK)
	       printf("\n POOL Closed\n");	  
        case APPLSTATE2:
	   PROC_Detach( ID_PROCESSOR );
	   printf("\n DSP PROCESSOR Detached\n");
        case APPLSTATE1:
           PROC_Destroy();
	   printf("\n DSP PROCESSOR INSTANCE Deleted\n");
        case APPLSTATE0:
           // we need to close the device driver only AFTER closing the DSP
           close( codecFd ); 
	   printf("\n closed the codec\n");
        default:            
            fprintf(stderr, "%s\n", errString);
            exit( 0 );
            break;
    }    
}


/*
 *  ======== terminateThread ========
 *
 */
static void terminateThread( char *errString, unsigned int runState )
{   
    switch ( runState ) {
        case AUDIOSTATE5:
	
            CHNL_Idle( ID_PROCESSOR, CHNL_ID_OUTPUT );  
	    printf("\n Transferred remaining data to DSP\n");      
        case AUDIOSTATE4:
            CHNL_FreeBuffer( ID_PROCESSOR, CHNL_ID_OUTPUT,
                     (char **)WAV_Buffers, NUMOUTBUFS );
            printf("\n Freed the Buffers allocated for Data Transfer\n");
        case AUDIOSTATE3:
        case AUDIOSTATE2:
            CHNL_Delete( ID_PROCESSOR, CHNL_ID_OUTPUT );
	    printf("\n Deleted the channel to DSP\n");
        case AUDIOSTATE1:
        case AUDIOSTATE0:
	default:            
            fprintf(stderr, "%s\n", errString);
            pthread_exit( NULL );
            break;
    }    
}


/*
 *  ======== sendControlMsg ========
 *  Send a control message to the DSP.
 */
static void sendControlMsg( unsigned short int cmd,
                            unsigned short int arg1,
                            unsigned short int arg2 )
{
      CtrlMsg * msg;
    DSP_STATUS status = DSP_ENOTFOUND ;
     
    
    
    status = MSGQ_Alloc (SAMPLE_POOL_ID, APP_MSG_SIZE,(MsgqMsg *) &msg) ;
    
  
    if (DSP_SUCCEEDED (status)) {
#ifdef DDSP_DEBUG
           printf ("MSGQ_Alloc done successfully for Control Message to DSP!!!!!!!!!!!!\n\n") ;
#endif
            msg->cmd  = cmd;
            msg->arg1 = arg1;
            msg->arg2 = arg2;
            /* Send the allocated message */
            MSGQ_SetMsgId ((MsgqMsg) msg, CTRL_MSGID) ;
	    status = MSGQ_Put (SampleDspMsgq, (MsgqMsg) msg) ;
            if(status != DSP_SOK)
	    {
                   MSGQ_Free( (MsgqMsg)msg );
		   printf ("MSGQ_Put failed--> Released the message!!!!!!!!!!!!\n\n") ;
            }
      }      
       
    return;
}


/*
 *  ======== streamThr ========
 *  Thread to process the audio data.
 */
static void *streamThr( void *arg )
{  
    
    /* Initialize the data streaming channels */
    initDataStreaming();
         
    /* Prime the data streaming channels */
    primeDataStreaming();
    
    runDataStreaming();
      
    /* Terminate this thread */
    terminateThread( "Audio streaming thread terminating.", AUDIOSTATE5 );
    
    return (0);
}


/*
 *  ======== initDataStreaming ========
 *  Code to initialize the input and output data streaming channels
 */
static void initDataStreaming( void )
{
    ChannelAttrs chnlAttrOutput;
    DSP_STATUS status = DSP_SOK;
    
       /*
     *  Create a channel to DSP
     */
    if (DSP_SUCCEEDED (status)) {
        chnlAttrOutput.mode      = ChannelMode_Output     ;
        chnlAttrOutput.endianism = Endianism_Default      ;
        chnlAttrOutput.size      = ChannelDataSize_16bits ;
        printf ("PROC_Start () success!!!!!!!!!!!!!\n\n");
        status = CHNL_Create (ID_PROCESSOR, CHNL_ID_OUTPUT, &chnlAttrOutput) ;
        if (DSP_FAILED (status)) {
             terminateThread( "Error: Failed CHNL_Create() of output channel, terminating thread", AUDIOSTATE1 );
        }
    }

     /*
     *  Allocate buffer(s) for data transfer to DSP.
     */
    
    if (DSP_SUCCEEDED (status)) {
        printf ("CHNL_Create () success!!!!!!!!!!!!!\n\n");
        status = CHNL_AllocateBuffer (ID_PROCESSOR,
                                      CHNL_ID_OUTPUT,
                                      (char **)WAV_Buffers,
                                      WAV_BufferSize,
                                      NUMOUTBUFS) ;
        if (DSP_FAILED (status)) {
           terminateThread( "Error: Failed CHNL_AllocateBuffer() on output, terminating thread", AUDIOSTATE3 );
        }
	else
	{
	   printf("CHNL_AllocateBuffer success\n");
	}
    }
    

}


static void primeDataStreaming()
{

 DSP_STATUS       status = DSP_SOK;
 ChannelIOInfo ioReqOutput;
 Uint32           i ;
     

      for ( i = 0; i < NUMOUTBUFS; i++ ) {

        /*
         *  Since this is the output channel, need to initialize
         *  data to silence.
         */
       	memset( (char *)WAV_Buffers[i], 0, BUFFERSIZE );

        ioReqOutput.buffer = (char *) WAV_Buffers[i];
        ioReqOutput.size   = WAV_BufferSize;
   
        /*
         *  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
#ifdef DDSP_DEBUG
	 printf("Before Channel Issue \n");
#endif
        status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_OUTPUT, &ioReqOutput) ;
#ifdef DDSP_DEBUG
         printf("After Channel Issue \n");
#endif
       }
}


/*
 *  ======== runDataStreaming ========
 *  Code to loopback the audio data to the DSP
 */
static void runDataStreaming( void )
{
    ChannelIOInfo ioReqOutput;
    DSP_STATUS status;
    int k = 0;                  // counting through each buffer
    int j = 0;                  // counting amount of data transfered
    char tmp[BUFFERSIZE];       // variable to store the data from the read call
#ifdef DSP_DEBUG  
  int d = 0; // counting debug statements
#endif
   
    while( flag == 1 ) {

        /* Reclaim empty buffer from the output channel */
	
        status = CHNL_Reclaim( ID_PROCESSOR, CHNL_ID_OUTPUT,WAIT_FOREVER, &ioReqOutput );
	
        if ( !DSP_SUCCEEDED( status ) ) {
            fprintf(stderr, "Error: Failed to reclaim from output stream\n");
            break;
        }

        if (header.numberofchannels == 1){ // mono
			if (header.compression_code != 1){
				if (fread(tmp,1,BUFFERSIZE/2,fp)==-1 || j>= header.datasize){
					printf("End of file\n");
					break;
				}

				j+=BUFFERSIZE/2;
				// Simpy copy the data. Could add real processing here... 
				// need reordering because reading a file bytewize is in different
				// endianness than the 16 bit words the DSP uses
				for (k=0;k<BUFFERSIZE;k=k+4){
					ioReqOutput.buffer[k] = tmp[k/2];
					ioReqOutput.buffer[k+1] = tmp[k/2+1];
					ioReqOutput.buffer[k+2] = tmp[k/2];
					ioReqOutput.buffer[k+3] = tmp[k/2+1];
				}  
			}else{
				printf("alaw or ulaw data mono\n");
				if (fread(tmp,1,BUFFERSIZE/2,fp)==-1 || j>= header.datasize){
					printf("End of file\n");
					break;
				}

				j+=BUFFERSIZE/2;
				// Simpy copy the data. Could add real processing here... 
				// need reordering because reading a file bytewize is in different
				// endianness than the 16 bit words the DSP uses
				for (k=0;k<BUFFERSIZE;k=k+4){
					ioReqOutput.buffer[k] = tmp[k/2];
					ioReqOutput.buffer[k+1] = tmp[k/2+1];
					ioReqOutput.buffer[k+2] = tmp[k/2];
					ioReqOutput.buffer[k+3] = tmp[k/2+1];
				}  
			}
        }else{
                        //printf("\n Stereo mode\n");
			if (header.compression_code != 1){
				if (fread(tmp,1,BUFFERSIZE/2,fp)==-1 || j>= header.datasize){
					printf("End of file\n");
					break;
				}

				j+=BUFFERSIZE;
				// Simpy copy the data. Could add real processing here... 
				// need reordering because reading a file bytewize is in different
				// endianness than the 16 bit words the DSP uses
				for (k=0;k<BUFFERSIZE/2;k=k+4){
					ioReqOutput.buffer[k] = tmp[k+2];
					ioReqOutput.buffer[k+1] = tmp[k+3];
					ioReqOutput.buffer[k+2] = tmp[k];
					ioReqOutput.buffer[k+3] = tmp[k+1];
				}
			}else{
				if (fread(tmp,1,BUFFERSIZE,fp)==-1 || j>= header.datasize){
					printf("End of file\n");
					break;
				}

				j+=BUFFERSIZE;
				// Simpy copy the data. Could add real processing here... 
				// need reordering because reading a file bytewize is in different
				// endianness than the 16 bit words the DSP uses
				for (k=0;k<BUFFERSIZE;k=k+4){
					ioReqOutput.buffer[k] = tmp[k+2];
					ioReqOutput.buffer[k+1] = tmp[k+3];
					ioReqOutput.buffer[k+2] = tmp[k];
					ioReqOutput.buffer[k+3] = tmp[k+1];
				}
			}
        }
#ifdef DSP_DEBUG   
    //print out dots to show the file is being read, and sent to the dsp       
    printf(".");
    d++; 
    if (d == 20) 
    {
      d = 0; 
      printf("\n");
    }
#endif

        /* Issue the full output buffer to the DSP */
        status = CHNL_Issue( ID_PROCESSOR, CHNL_ID_OUTPUT, &ioReqOutput );
        if ( !DSP_SUCCEEDED(status ) ) {
            fprintf(stderr, "Error: Failed to issue to output stream\n");
            break;
        }
    }
}


/*
 *  ======== configureCodec ========
 *  Configure the codec.
 */
static void configureCodec( int sampleFreq )
{
    int ioctlarg;    

    /* Open the codec */
    codecFd = open( CODEC_DEV, O_WRONLY, 0 );

    if( codecFd == -1 ) {
       terminateThread( "Error: Failed to open audio driver", AUDIOSTATE0 );
    }

    /* Configure the bits/sample in the driver */
    ioctlarg = BITSPERSAMPLE; 

    if( ioctl(codecFd, SNDCTL_DSP_SETFMT, &ioctlarg) == -1 ) {
        terminateThread( "Error: cannot set bits", AUDIOSTATE1 );
    }

    /* Configure the number of audio channels in the data stream */
    ioctlarg = STEREO;    // AIC23 is a stereo codec

    if( ioctl(codecFd, SNDCTL_DSP_CHANNELS, &ioctlarg) == -1 ) {
        //printf( "Error: cannot set channels\n");
	terminateThread( "Error: cannot set channels", AUDIOSTATE1 );
    }

    /* Configure the speed of the data stream */
    ioctlarg = header.samplingrate;
    printf("configuring codec to %d sampling rate\n",header.samplingrate);
	

    if( ioctl(codecFd, SNDCTL_DSP_SPEED, &ioctlarg) == -1 ) {
       
	terminateThread( "Error: cannot set speed", AUDIOSTATE1 );
    }
    return;
}


/* snd_getWavHeader()
 *  
 * return -1 on failure (if the file is not a WAVE file), 0 on success
 * 
 * Description:
 * extract first 44 bytes (or more if needed) of the wave file header:
 * RIFF - 4 bytes
 * filesize 4 byte integer
 * WAVE - 4 bytes
 * fmt  - 4 bytes
 * fmtsize - 4 byte integer
 * compression code - 2 byte integer must be 1
 * number of channels - 2 byte integer
 * sampling rate - 4 byte integer must be one of 
 *                 96000, 88200, 48000, 24000, 44100, 32000, 16000, or 8000
 * byte rate - 4 byte integer, should be sampling rate x 4 or 2 for mono
 * block align - 2 byte integer
 * number of bits - 2 byte integer, must be 16
 * search for "data" - 4 bytes
 * datasize 4 byte integer
 */
static int snd_getWavHeader()
{  
  char field_name[256];

  fp = fopen(g_path,"rb");
  if (fp == NULL)
  {
    printf("unable to open file: %s\n",g_path); 
    return -1; 
  }
  printf("\n Successfully opened the wav file\n");
  
  memset(field_name,0,256);
  if (fread(field_name,4,1,fp)==-1)
    { printf("error reading header 0\n"); fclose(fp); return -1; }
  
  if (strcmp(field_name,"RIFF")!=0)
    { printf("file not RIFF format\n"); fclose(fp); return -1; }
  
  if (fread(&header.filesize,1,4,fp)==-1)
    { printf("error reading header.filesize\n"); fclose(fp); return -1; }
  
  memset(field_name,0,256);
  if (fread(field_name,4,1,fp)==-1)
    { printf("error reading header 0\n"); fclose(fp); return -1; }
  
  if (strcmp(field_name,"WAVE")!=0)
    { printf("file not WAVE format\n"); fclose(fp); return -1; }
   
  memset(field_name,0,256);
  if (fread(field_name,4,1,fp)==-1)
    { printf("error reading header 0\n"); fclose(fp); return -1; }
 
  if (strcmp(field_name,"fmt ")!=0)
    { printf("file not right WAVE format\n"); fclose(fp); return -1; }
  
  if (fread(&header.wavefmtsize,1,4,fp)==-1)
    { printf("error reading wave format structure size\n"); fclose(fp); return -1; }
  
  if (fread(&header.compression_code,1,2,fp)==-1)
    { printf("error reading compression code\n"); fclose(fp); return -1; }
 
  if (fread(&header.numberofchannels,1,2,fp)==-1)
    { printf("error reading number of channels\n"); fclose(fp); return -1; }
  
  if (fread(&header.samplingrate,1,4,fp)==-1)
    { printf("error reading sampling rate\n"); fclose(fp); return -1; }
  switch(header.samplingrate)
  {
  case 96000:
  case 88200:
  case 48000:
  case 24000:
  case 44100:
  case 32000:
	case 22050:
  case 16000:
  case 8000:
  break;
  default:
    printf("Unsupported sampling rate: %d\n",header.samplingrate); fclose(fp); return -1;
  }

  if (fread(&header.byterate,1,4,fp)==-1)
    { printf("error reading Avg byte rate\n"); fclose(fp); return -1; }
  
  if (fread(&header.blockalign,1,2,fp)==-1)
    { printf("error reading Block alignment\n"); fclose(fp); return -1; }
  
  if (fread(&header.sigbits,1,2,fp)==-1)
    { printf("error reading significant bits / sample\n"); fclose(fp); return -1; }
  
  if (header.compression_code != 1 && header.compression_code != 6 &&
	    header.compression_code != 7)
			{
				printf("non supported compression code files not supported \n"); fclose(fp); return -1;
			}
  if (header.sigbits != 16 && header.compression_code == 1 )
    {
      printf("non 16 bit PCM files not supported \n"); fclose(fp); return -1;
    }

  // search for data
  if (fread(field_name,4,1,fp)==-1)
    { printf("error reading header 0\n"); fclose(fp); return -1; }
    
  while (strcmp(field_name,"data")!=0)
  {
    field_name[0] = field_name[1]; field_name[1] = field_name[2]; field_name[2] = field_name[3];
    if (fread(&field_name[3],1,1,fp)==-1)
      { printf("file not data format\n"); fclose(fp); return -1; }
  }

  if (fread(&header.datasize,1,4,fp)==-1)
    { printf("error data chunk size\n"); fclose(fp); return -1; }

  // the next byte(s) are PCM data

  return 0;
}

