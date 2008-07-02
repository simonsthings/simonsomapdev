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

#define DDSP_DEBUG


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

typedef unsigned char BYTE;

//Wave Header
typedef struct {
    char        riff[4];        /* "RIFF"                                  */
    long        FileLen;        /* file length in bytes                    */
    char        wave[4];        /* "WAVE"                                  */
    char        fmt[4];         /* "fmt "                                  */
    long        blkSize;        /* in bytes (generally 16)                 */
    short       fmtTag;         /* 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM */
    short       nCh;            /* 1=mono, 2=stereo                        */
    long        SampRate;       /* Sampling rate in samples per second     */
    long        BytesPerSec;    /* bytes per second                        */
    short       BytesPerSamp;   /* 2=16-bit mono, 4=16-bit stereo          */
    short       BitsPerSamp;    /* Number of bits per sample               */
    char        Data[4];        /* "data"                                  */
    long        Length;         /* data length in bytes (filelength - 44)  */
}WAVEHDR ;

FILE *OpenWaveWrite(char *pName, WAVEHDR *pHdr)
{
    FILE *pFile;

    if (pName[strlen(pName)-1] == 'v')   {
        pFile= fopen(pName,"wb");
        if (!pFile) {
            printf("Open Output File Error!!\n");
            pFile = 0;
        }
        else     {
            fwrite(pHdr,1,44,pFile);
        }
    }
    else {
        printf("You must specify a valid .wav file name\n");
        pFile = 0;
    }
    return pFile;
}

void CloseWaveWrite(FILE *pFile,long nBytes)
{
    long Bytes;

    Bytes = nBytes;

    fseek(pFile,40,SEEK_SET);
    /*
     * int fseek(File *stream,long offset,int origin)
     * The function fseek sets the file position data for the given stream.
     * The "origin" value should have one of the following values:
     * SEEK_SET -> Seek from the start of the file
     * SEEK_CUR -> Seek from the current position
     * SSEK_END -> Seek from the end of the file*/
    fwrite(&Bytes,4,1,pFile);

    Bytes = Bytes + 44;

    fseek(pFile,4,SEEK_SET);
    fwrite(&Bytes,4,1,pFile);

    fclose(pFile);
}

int WriteWaveData(FILE *pFile, BYTE *pBuffer, int Len)
{
    int rtn;

    rtn = fwrite(pBuffer, sizeof(*pBuffer), Len, pFile);

    return rtn;

}

void MakeWaveHdr(WAVEHDR *pHdr, int SampRate, int nCh, int nBit)
{
    WAVEHDR DefaultHdr       = { "RIF", 0, "WAV", "fmt", 16, 1, 1,
                                 SampRate, 0, 2, 16, "dat", 0 };
    DefaultHdr.SampRate      = SampRate;
    DefaultHdr.nCh           = nCh;
    DefaultHdr.BitsPerSamp   = nBit;
    DefaultHdr.BytesPerSamp  = nCh*nBit/8;
    DefaultHdr.BytesPerSec   = SampRate * DefaultHdr.BytesPerSamp;
    DefaultHdr.riff[3]       = 'F';
    DefaultHdr.wave[3]       = 'E';
    DefaultHdr.fmt[3]        = ' ';
    DefaultHdr.Data[3]       = 'a';
    *pHdr = DefaultHdr;
}

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
static int running = 1;
static int shutdownongoing = 0;

int codecFd = 0;

/* wave file pointer, path/filename string*/
FILE            *fp;
char            g_path[1024];
WAVEHDR         Hdr;
unsigned long   TotalWavSize = 0;

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
unsigned short *WAV_Buffers[NUMOUTBUFS] ;

/* Function prototypes for local functions */
static void terminateAppl( char *errString, unsigned int runState );
static void terminateThread( char *errString, unsigned int runState );
static void configureCodec( int sampleFreq );
static void initDataStreaming( void );
static void primeDataStreaming( void );
static void runDataStreaming( void );
static void *streamThr( void *arg );
static void sendControlMsg( unsigned short int cmd,
                            unsigned short int arg1,
                            unsigned short int arg2 );
void inputLoop();

/* Streaming thread handle */
pthread_t streamThread;


void usage(){
	printf("Usage: ./wav_rec_rf6_gpp <wav_rec_rf6_dsp.c55> <filename> [sampling f] \n");
	printf("  wav_rec_rf6_gpp: linux executable \n");
	printf("  <wav_rec_rf6_dsp.c55> dsp application file (*.c55)\n");
	printf("  <filename> output wave file (*.wav)\n");
	printf("  [sampling f] optional sampling frequency, default: 44100\n");
	printf("  - supported frequencies: 96, 88.2, 48, 44.1, 32, 24, 22.05, 16 and 8KHz\n");
	printf("Examples:\n");
	printf("  ./wav_rec_rf6_gpp wav_rec_rf6_dsp.c55 /sound/rec.wav\n");
	printf("  ./wav_rec_rf6_gpp wav_rec_rf6_dsp.c55 /sound/rec.wav 44100\n");
	exit(0);
}

/**
 * This function does 3 things:
 * 1.) It creates and opens (OpenWaveWrite) a .wav-file
 * with correct header (MakeWaveHdr) for writing the sound data to disk.
 * 2.) Then, it uses the DSPLink functions to load the DSP executable onto the
 * DSP (PROC_Setup, PROC_Attach and PROC_Load) and start it (PROC_Start).
 * 3.) After that, it opens a message queue for communication with the DSP side program.
 * (using MSGQ_TransportOpen and MSGQ_Locate).
 * */
DSP_STATUS setupDSP(int sampleFreq, char* dspfilename, char* wavefilename)
{
    DSP_STATUS status = DSP_SOK;

    Uint32        size    [NUM_BUF_SIZES] = {APP_MSG_SIZE,  SAMPLE_CTRLMSG_SIZE,
                                             ALIGN (sizeof (MsgqAsyncLocateMsg),
                                             DSPLINK_BUF_ALIGN)} ;

    Uint32        numBufs [NUM_BUF_SIZES] = {NUM_BUF_POOL0,
                                             NUM_BUF_POOL1,
                                             NUM_BUF_POOL2,
                                             NUM_BUF_POOL3} ;

    SmaPoolAttrs poolAttrs;
    ZcpyMqtAttrs mqtAttrs;
    MsgqLocateAttrs syncLocateAttrs;
    printf("\nReference Framework 6 audio application started\n\n");
    MakeWaveHdr(&Hdr, sampleFreq, 2, 16);
    fp = OpenWaveWrite(wavefilename, &Hdr);
    TotalWavSize = 0;
    // we need to configure the codec before setting up the DSP
    configureCodec(sampleFreq);
    WAV_BufferSize = BUFFERSIZE;
    /*
     *  Create and initialize the proc object.
     */
    status = PROC_Setup();
    /*
     *  Attach the Dsp with which the transfers have to be done.
     */
	if (DSP_SUCCEEDED(status))
	{
		printf("PROC_Setup () success!!!!!!!!!!!!!\n\n");
		status = PROC_Attach(ID_PROCESSOR, NULL);

		if (DSP_FAILED(status))
		{
			terminateAppl("Error: Failed PROC_Attach()", APPLSTATE2);
		}
	} else
	{
		terminateAppl("Error: Failed PROC_Setup()", APPLSTATE0);
	}
    /*
     *  Open the pool.
     */
    if (DSP_SUCCEEDED (status)) {
        printf ("PROC_Attach () success!!!!!!!!!!!!!\n\n");
        size [3]                = WAV_BufferSize ;
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
        printf("*****File Name******* %s\n", dspfilename);
        status = PROC_Load(ID_PROCESSOR, dspfilename, 0, NULL);
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
    printf("PROC_Start () success!!!!!!!!!!!!!\n\n");
    printf("Attach emulator and run DSP, then press return to continue: \n");
    getchar(); // TODO: deactivate this for the final version! (maybe just wait 1 second instead..?)
#ifdef DSP_DEBUG
    //print out dots to show the file is being read, and sent to the dsp
       printf(".");
       fflush(stdout);
       d++;
       if (d == 60){
          d = 0;
          printf("\n");
       }
#endif
    printf("Continuing\n");
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
            status = MSGQ_Locate (SampleDspMsgqName,&SampleDspMsgq,&syncLocateAttrs) ;
            if (status == DSP_ENOTFOUND) {
                usleep (100000) ;
            }else if (DSP_FAILED (status)) {
                terminateAppl( "Error: Failed to Open the Transport to DSP,terminating application...", APPLSTATE4 );
            }
        }
    }
    printf("MSGQ_Locate of DSP message queue done successfully!!!!!!!!!!!!\n\n");

    return status;
}

/**
 * Tells the DSP to finish its program.
 */
void stopDSP()
{
	sendControlMsg( MSGQUIT, 0, 0 );
}

/**
 * Create and start the thread handling the
 * streaming of data to and from the DSP.
 */
void startStreamThread(int sampleFreq)
{
    printf("Initializing the data streaming thread\n");
    if (pthread_create(&streamThread, NULL, streamThr, (void *) sampleFreq)) {
      printf( "Error: Failed to create stream thread\n\n");
    }
    printf("\n Thread created using function streamThr(..) ! \n");
}

/**
 * Sets the flag variable to zero so that the stream thread ends.
 * Specifically, the function runDataStreaming(..) contains a
 * "while( flag == 1 )" loop.
 */
void stopStreamThread()
{
	shutdownongoing = 1;
	running = 0;

	// just loop until the other thread has finished.
//	printf("Waiting for the streamThread to finish:\n");
//	while (shutdownongoing)
//	{
//		printf(". ");
//		sleep(100);
//	}
//	printf("streamThread has finished!\n");
}

/*******************Wavplayer data*****************************************************************/


/**
 *
 * Arguments:
 * 0. filename of this executable
 * 1. The filename of the dsp executable (e.g. wav_rec_dsp.out)
 * 2. The filename for the new wave file (e.g. example.wav)
 * 3. The sampling rate (e.g. 32000)
 *
 * These values are stored as strings in the argv[] vector.
 * The number of arguments is stored in argc.
 */
int main_old(int argc, char *argv[])
{
    int sampleFreq = 44100;
    DSP_STATUS status;

    char* dspfilename;
    char* wavefilename;

    if ( argc < 3 ) {
       usage();
    }

    if (argc > 3){
      sampleFreq = atoi(argv[3]);
      if (sampleFreq >= 96000) sampleFreq = 96000;
      else if (sampleFreq >= 88200) sampleFreq = 88200;
      else if (sampleFreq >= 48000) sampleFreq = 48000;
      else if (sampleFreq >= 44100) sampleFreq = 44100;
      else if (sampleFreq >= 32000) sampleFreq = 32000;
      else if (sampleFreq >= 24000) sampleFreq = 24000;
	  else if (sampleFreq >= 22050) sampleFreq = 22050;
      else if (sampleFreq >= 16000) sampleFreq = 16000;
      else if (sampleFreq >= 8000)  sampleFreq = 8000; // doesn't always work well
   }else{
      sampleFreq = 44100;
  }

    //strcpy(g_path, argv[2]);
    strcpy(wavefilename, argv[2]);
    strcpy(dspfilename, argv[1]);


    // set up the DSP side.
    status = setupDSP(sampleFreq,dspfilename,wavefilename) ;

    // start the thread that receives the data from the DSP and writes it to a .wav file.
    startStreamThread(sampleFreq);

    // The input loop waits for user input and loops infinitely until the user presses 'q' for quit.
    inputLoop();

    return status ;
}

/*
 * The input loop waits for user input and loops
 * infinitely until the user presses 'q' for quit.
 * */
void inputLoop()
{
    char inbuf[USERINPUTMAXNUMCHARS];
    char *ptr;
    int val;


    sendControlMsg( MSGNEWVOL, 0, DEFAULT_VOLUME );
    sendControlMsg( MSGNEWVOL, 1, DEFAULT_VOLUME );

    printf("\nCommands:\n");
    printf("   Change volume      - v <volume 0-200>  e.g. 'v 100'\n");
    printf("   Quit               - q\n\n");
    printf("> ");


    while ( running ) {

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
                     running = 0;
                     break;

               default:
                     printf( "Unrecognized command, try again\n> " );
         }
      }
   }

   printf( "\n Leaving Reference Framework 6 audio application\n\n" );
   terminateAppl( "Clean-up completed.", APPLSTATE7 );

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
            running = 0;
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
            close( codecFd );
	          printf("\n closed the codec\n");
            CloseWaveWrite(fp, TotalWavSize);
            printf(" Total Wave Size %d \n", (int)TotalWavSize);
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
            CHNL_Idle( ID_PROCESSOR, CHNL_ID_INPUT );
	          printf("\n Transferred remaining data to DSP\n");
        case AUDIOSTATE4:
            CHNL_FreeBuffer( ID_PROCESSOR, CHNL_ID_INPUT,
                     (char **)WAV_Buffers, NUMOUTBUFS );
            printf("\n Freed the Buffers allocated for Data Transfer\n");
        case AUDIOSTATE3:
        case AUDIOSTATE2:
            CHNL_Delete( ID_PROCESSOR, CHNL_ID_INPUT );
	          printf("\n Deleted the channel to DSP\n");
        case AUDIOSTATE1:
        case AUDIOSTATE0:
	      default:
            fprintf(stderr, "%s\n", errString);

            printf("streamThr: All work done, telling everyone!\n");
            // Tell the main thread that we are finished!
        	shutdownongoing = 0;

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

           MSGQ_SetMsgId ((MsgqMsg) msg, CTRL_MSGID) ;
	         status = MSGQ_Put (SampleDspMsgq, (MsgqMsg) msg) ;

           if(status != DSP_SOK){
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

    /*
     * Run the actual loop that receives data from
     * the DSP and stores it in a .wav file .
     **/
    runDataStreaming();

    /* Terminate this thread */
    terminateThread( "Audio streaming thread terminating.", AUDIOSTATE5 );

    return (0);
}


/*
 *  ======== initDataStreaming ========
 *  Code to initialize the input and output data streaming channels.
 *  "Make a data connection between ARM and DSP sides."
 *  (Uses CHNL_Create and CHNL_AllocateBuffer)
 */
static void initDataStreaming( void )
{
    ChannelAttrs chnlAttrOutput;
    DSP_STATUS status = DSP_SOK;

    /*
     *  Create a channel to DSP
     */
    if (DSP_SUCCEEDED (status)) {
        chnlAttrOutput.mode      = ChannelMode_Input ;
        chnlAttrOutput.endianism = Endianism_Default ;
        chnlAttrOutput.size      = ChannelDataSize_16bits ;
        printf ("PROC_Start () success!!!!!!!!!!!!!\n\n");
        status = CHNL_Create (ID_PROCESSOR, CHNL_ID_INPUT, &chnlAttrOutput) ;
        if (DSP_FAILED (status)) {
             terminateThread( "Error: Failed CHNL_Create() of output channel, terminating thread", AUDIOSTATE1 );
        }
    }

     /*
     *  Allocate buffer(s) for data transfer to DSP.
     */

    if (DSP_SUCCEEDED (status)) {
        printf ("CHNL_Create () success!!!!!!!!!!!!!\n\n");
        status = CHNL_AllocateBuffer (ID_PROCESSOR, CHNL_ID_INPUT, (char **)WAV_Buffers,
                                      WAV_BufferSize, NUMOUTBUFS) ;
        if (DSP_FAILED (status)) {
           terminateThread( "Error: Failed CHNL_AllocateBuffer() on output, terminating thread", AUDIOSTATE3 );
        }else{
	          printf("CHNL_AllocateBuffer success\n");
	      }
    }

}

/**
 * Initializes the buffer variable and calls CHNL_Issue for the first time.
 */
static void primeDataStreaming()
{

 DSP_STATUS       status = DSP_SOK;
 ChannelIOInfo    ioReqOutput;
 Uint32           i ;


      for ( i = 0; i < NUMOUTBUFS; i++ ) {

        /*
         *  Since this is the output channel, need to initialize
         *  data to silence.
         */
       	memset( WAV_Buffers[i], 0, BUFFERSIZE );

        ioReqOutput.buffer = (char *) WAV_Buffers[i];
        ioReqOutput.size   = WAV_BufferSize;

        /*
         *  Send data to DSP.
         *  Issue 'filled' buffer to the channel.
         */
#ifdef DDSP_DEBUG
	 printf("Before Channel Issue \n");
#endif
        status = CHNL_Issue (ID_PROCESSOR, CHNL_ID_INPUT, &ioReqOutput) ;
#ifdef DDSP_DEBUG
         printf("After Channel Issue \n");
#endif
       }
}


/*
 *  ======== runDataStreaming ========
 * Run the actual loop that receives data from
 * the DSP and stores it in a .wav file .
 * The loop ends when the global variable "flag"
 * becomes zero.
 */
static void runDataStreaming( void )
{
    ChannelIOInfo ioReqOutput;
    DSP_STATUS status;
    int k = 0;                  // counting through each buffer
    char tmp[BUFFERSIZE];       // variable to store the data from the read call
#ifdef DSP_DEBUG
  int d = 0; // counting debug statements
#endif

    while( running == 1 ) {


        /* Reclaim empty buffer from the output channel */

        status = CHNL_Reclaim( ID_PROCESSOR, CHNL_ID_INPUT,WAIT_FOREVER, &ioReqOutput );

        if ( !DSP_SUCCEEDED( status ) ) {
            fprintf(stderr, "Error: Failed to reclaim from output stream\n");
            break;
        }

        /* Swap left and right channel.
         * As the audio data is 16bit but the array field are only 8 bit each,
         * we need to swap two fields at a time.
         * (LRLR --> RLRL) (16 bit values)
         */
        for (k=0;k<BUFFERSIZE;k=k+4){
            tmp[k+2] = ioReqOutput.buffer[k]  ;
            tmp[k+3] = ioReqOutput.buffer[k+1];
            tmp[k]   = ioReqOutput.buffer[k+2];
            tmp[k+1] = ioReqOutput.buffer[k+3];
        }

       // Write audio data to wave file.
       TotalWavSize += WriteWaveData( fp, tmp, BUFFERSIZE);

#ifdef DSP_DEBUG
    //print out dots to show the file is being read, and sent to the dsp
       printf(".");
       fflush(stdout);
       d++;
       if (d == 60){
          d = 0;
          printf("\n");
       }
#endif

        /* Issue the full output buffer to the DSP */
        status = CHNL_Issue( ID_PROCESSOR, CHNL_ID_INPUT, &ioReqOutput );
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
    codecFd = open( CODEC_DEV, O_RDONLY );

    if( codecFd == -1 ) {
       terminateThread( "Error: Failed to open audio driver", AUDIOSTATE0 );
    }

    /* Configure the bits/sample in the driver */
    ioctlarg = 16;

    if( ioctl(codecFd, SNDCTL_DSP_SETFMT, &ioctlarg) == -1 ) {
        terminateThread( "Error: cannot set bits", AUDIOSTATE1 );
    }

    /* Configure the number of audio channels in the data stream */
    ioctlarg = 2;    // AIC23 is a stereo codec

    if( ioctl(codecFd, SNDCTL_DSP_CHANNELS, &ioctlarg) == -1 ) {
        //printf( "Error: cannot set channels\n");
	      terminateThread( "Error: cannot set channels", AUDIOSTATE1 );
    }

    /* Configure the speed of the data stream */
    ioctlarg = sampleFreq;
    printf("configuring codec to %d sampling rate\n",sampleFreq);


    if( ioctl(codecFd, SNDCTL_DSP_SPEED, &ioctlarg) == -1 ) {

	      terminateThread( "Error: cannot set speed", AUDIOSTATE1 );
    }

    ioctlarg = 0;
    if( ioctl(codecFd, SOUND_MIXER_WRITE_MIC, &ioctlarg) == -1 ) {
        printf("Error: cannot set Mic Mode\n");
    }

    ioctlarg = 80;
    if( ioctl(codecFd, SOUND_MIXER_WRITE_LINE, &ioctlarg) == -1 ) {
        printf("Error: cannot set Line-In Mode\n");
    }

}

