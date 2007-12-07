/** @mainpage I2C CSL 3.x
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the I2C module across various devices. The CSL developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given I2C module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular I2C Module. The CSL developer should use his discretion designing
 * new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# CSL-001-DES, CSL 3.x Design Specification DocumentVersion 1.02
 *
 */

/** @file csl_i2c.h
 *
 * @brief    Header file for functional layer of CSL
 *
 * Description
 *    - The different enumerations, structure definitions
 *      and function declarations
 *
 * Modification 1
 *    - modified on: 29/01/2004
 *    - reason: Created the sources
 *
 * Modification 2
 *    - modified on: 01/03/2004
 *    - reason: Make changes as per review comments.
 *              Remove "dlben" from Hwsetup
 *
 * Modification 3
 *    - modified on: 02/03/2004
 *    - reason: Added few symbolic constants.
 *
 * @date 29th Jan, 2004
 * @author Amruth Tadas.
 *
 */
#ifndef _CSL_I2C_H_
#define _CSL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_sysData.h>
#include <csl_types.h>
#include <cslr_i2c.h>

/**************************************************************************\
* I2C global macro declarations
\**************************************************************************/

/** Constants for passing parameters to the functions.
 */

/** For setting the SLAVE Mode for I2C           */
#define CSL_I2C_MODE_SLAVE                            (0)
/** For setting the MASTER Mode for I2C          */
#define CSL_I2C_MODE_MASTER                           (1)
/** For setting the RECEIVER Mode for I2C        */
#define CSL_I2C_DIR_RECEIVE                           (0)
/** For setting the TRANSMITTER Mode for I2C     */
#define CSL_I2C_DIR_TRANSMIT                          (1)
/** For setting the 7-bit Addressing Mode for I2C*/
#define CSL_I2C_ADDRSZ_SEVEN                          (0)
/** For setting the 10-bit Addressing Mode       */
#define CSL_I2C_ADDRSZ_TEN                            (1)
/** For setting the Little Endian Mode for I2C   */
#define CSL_I2C_ENDIAN_LITTLE                         (0)
/** For setting the Big Endian Mode for I2C      */
#define CSL_I2C_ENDIAN_BIG                            (1)
/** For Disabling the Transmitter DMA for I2C    */
#define CSL_I2C_TX_DMA_DISABLE                        (0)
/** For Enabling the Transmitter DMA for I2C     */
#define CSL_I2C_TX_DMA_ENABLE                         (1)
/** For Disabling the Receiver DMA for I2C       */
#define CSL_I2C_RX_DMA_DISABLE                        (0)
/** For Enabling the Receiver DMA for I2C        */
#define CSL_I2C_RX_DMA_ENABLE                         (1)
/** For Disabling the Start Byte Mode for I2C(Normal Mode) */
#define CSL_I2C_STB_DISABLE                           (0)
/** For Enabling the Start Byte Mode for I2C     */
#define CSL_I2C_STB_ENABLE                            (1)
/** For indicating the non-completion of Reset    */
#define CSL_I2C_RESET_NOT_DONE                        (0)
/** For indicating the completion of Reset    */
#define CSL_I2C_RESET_DONE                            (1)
/** For indicating that the bus is not busy    */
#define CSL_I2C_BUS_NOT_BUSY                          (0)
/** For indicating that the bus is busy    */
#define CSL_I2C_BUS_BUSY                              (1)
/** For indicating that the Receive ready signal is low    */
#define CSL_I2C_RX_NOT_READY                          (0)
/** For indicating that the Receive ready signal is high    */
#define CSL_I2C_RX_READY                              (1)
/** For indicating that the Transmit ready signal is low    */
#define CSL_I2C_TX_NOT_READY                          (0)
/** For indicating that the Transmit ready signal is high    */
#define CSL_I2C_TX_READY                              (1)
/** For indicating that the Access ready signal is low    */
#define CSL_I2C_ACS_NOT_READY                          (0)
/** For indicating that the Access ready signal is high    */
#define CSL_I2C_ACS_READY                              (1)
/** For indicating Single Byte Data signal is set */
#define CSL_I2C_SINGLE_BYTE_DATA                       (1)
/** For indicating Receive overflow signal is set */
#define CSL_I2C_RECEIVE_OVERFLOW                       (1)
/** For indicating Transmit underflow signal is set */
#define CSL_I2C_TRANSMIT_UNDERFLOW                     (1)
/** For indicating Arbitration Lost signal is set */
#define CSL_I2C_ARBITRATION_LOST                                           (1)


/** Constants for status bit clear
 */

/** Clear the Arbitration Lost status bit        */
#define CSL_I2C_CLEAR_AL 0x1
/** Clear the No acknowledge status bit          */
#define CSL_I2C_CLEAR_NACK 0x2
/** Clear the Register access ready status bit   */
#define CSL_I2C_CLEAR_ARDY 0x4
/** Clear the Receive ready status bit           */
#define CSL_I2C_CLEAR_RRDY 0x8
/** Clear the Transmit ready status bit          */
#define CSL_I2C_CLEAR_XRDY 0x10
/** Clear the General call status bit            */
#define CSL_I2C_CLEAR_GC 0x20


/**************************************************************************\
* I2C global typedef declarations
\**************************************************************************/

/** @brief This object contains the reference to the instance of I2C opened
 *  using the @a CSL_i2cOpen().
 *
 *  The pointer to this, is passed to all I2C CSL APIs.
 */
typedef struct CSL_I2cObj {
        /** This is the mode which the CSL instance is opened     */
        CSL_OpenMode openMode;
        /** This is a unique identifier to the instance of I2C being
         *  referred to by this object
         */
        CSL_Uid uid;
        /** This is the variable that contains the current state of a
         *  resource being shared by current instance of I2C with
     *  other peripherals
     */
        CSL_Xio xio;
        /** This is a pointer to the registers of the instance of I2C
     *  referred to by this object
     */
        CSL_I2cRegsOvly regs;
        /** This is the instance of I2C being referred to by this object  */
        CSL_I2cNum perNum;
}CSL_I2cObj;

typedef struct CSL_I2cObj *CSL_I2cHandle;

/** @brief This has all the fields required to configure the
 *  I2C clock
 */
typedef struct CSL_I2cClkSetup_{
        /** Prescalar to the input clock     */
        Uint16 prescalar;
        /** Low time period of the clock         */
        Uint16 clklowdiv;
        /** High time period of the clock    */
        Uint16 clkhighdiv;
}CSL_I2cClkSetup;


/** @brief This has all the fields required to configure I2C at Power Up
 *  (After a Hardware Reset) or a Soft Reset
 *
 *  This structure is used to setup or obtain existing setup of
 *  I2C using @a CSL_i2cHwSetup() & @a CSL_i2cGetHwSetup() functions
 *  respectively.
 */
typedef struct CSL_I2cHwSetup_ {
        /** Master or Slave Mode : 1==> Master Mode, 0==> Slave Mode  */
        Uint16 mode;
        /** Transmitter Mode or Receiver Mode: 1==> Transmitter Mode,
         *  0 ==> Receiver Mode
         */
        Uint16 dir;
        /** Addressing Mode :0==> 7-bit Mode, 1==> 10-bit Mode           */
        Uint16 addrMode;
        /** Endianess : 0 ==> Little Endian, 1 ==> Big Endian.           */
        Uint16 endianess;
        /** Start Byte Mode : 1 ==> Start Byte Mode, 0 ==> Normal Mode   */
        Uint16 sttbyteen;
        /** Address of the own device    */
        Uint16 ownaddr;
        /** Transmission DMA Event : 1==> Enable the Transmission Event,
         *  0 ==> Disable the Transmission Event
         */
        Uint16 txdmaen;
        /** Receive DMA Event : 1==> Enable the Receive DMA Event,
     *  0 ==> Disable the Receive DMA Event
     */
        Uint16 rxdmaen;
        /** Interrupt Enable mask The mask can be for one interrupt or
         *  OR of multiple interrupts.
         */
        Uint16 inten;
        /** Prescalar, Clock Low Time and Clock High Time for Clock Setup        */
        CSL_I2cClkSetup *clksetup;
}CSL_I2cHwSetup;


/** @brief Enumeration for queries passed to @a CSL_i2cGetHwStatus()
 *
 * This is used to get the status of different operations or to get the
 * existing setup of I2C.
 */
typedef enum {
  /** Get current clock setup parameters (response type:@a CSL_I2cClkSetup *) */
  CSL_I2C_QUERY_CLOCK_SETUP = 1,
  /** Get the Bus Busy status information (response type: @a Uint16 *)        */
  CSL_I2C_QUERY_BUS_BUSY ,
  /** Get the Receive Ready status information(response type: @a Uint16 *)    */
  CSL_I2C_QUERY_RX_RDY ,
  /** Get the Transmit Ready status information(response type: @a Uint16 *)   */
  CSL_I2C_QUERY_TX_RDY ,
  /** Get the Register Ready status information(response type: @a Uint16 *)   */
  CSL_I2C_QUERY_ACS_RDY ,
  /** Get the Single Byte Data bit information (response type: @a Uint16 *)   */
  CSL_I2C_QUERY_SBD ,
  /** Get the Receive overflow status information(response type: @a Uint16 *) */
  CSL_I2C_QUERY_ROVR ,
  /** Get the Transmit underflow status information(response type:@a Uint16 *)*/
  CSL_I2C_QUERY_XUDF ,
  /** Get the Arbitration Lost status information(response type: @a Uint16 *) */
  CSL_I2C_QUERY_AL ,
  /** Get the Reset Done status bit information(response type: @a Uint16 *)   */
  CSL_I2C_QUERY_RDONE
} CSL_I2cHwStatusQuery;


/** @brief Enumeration for queries passed to @a CSL_i2cHwControl()
 *
 * This is used to select the commands to control the operations
 * existing setup of I2C. The arguments to be passed with each
 * enumeration if any are specified next to the enumeration.
 */
 typedef enum {
        /** Enable the I2C : no argument                                 */
        CSL_I2C_CMD_ENABLE = 1,
        /** Disable the I2C: no argument                                 */
        CSL_I2C_CMD_DISABLE ,
        /** Software command to the I2C : no argument    */
        CSL_I2C_CMD_RESET ,
        /** Clear the status bits. The argument next to the command specifies the
         *  status bit to be cleared. The status bit can be
         *  CSL_I2C_CLEAR_AL,
         *  CSL_I2C_CLEAR_NACK,
         *  CSL_I2C_CLEAR_ARDY,
         *  CSL_I2C_CLEAR_RRDY,
     *  CSL_I2C_CLEAR_XRDY,
     *  CSL_I2C_CLEAR_GC
     */
        CSL_I2C_CMD_CLEAR_STATUS,
        /** Set the address of the Slave device : argument @a (Uint16 *)         */
        CSL_I2C_CMD_SET_SLAVE_ADDR ,
    /** Set the Data Count : argument @a (Uint16 *)                    */
    CSL_I2C_CMD_SET_DATA_COUNT ,
    /** Set the start condition : no argument                */
    CSL_I2C_CMD_START ,
    /** Set the stop condition : no argument                 */
    CSL_I2C_CMD_STOP ,
    /** Set the transmission mode : no argument                  */
    CSL_I2C_CMD_DIR_TRANSMIT ,
    /** Set the receiver mode : no argument                  */
    CSL_I2C_CMD_DIR_RECEIVE
} CSL_I2cHwControlCmd;


/** @brief Enumeration for I2C Test commands passed to @a CSL_i2cTest()
 *
 * This is used to select the Test commands to control the operations
 * existing setup of I2C.
 */
typedef enum {
        /** Enable the Test Mode                         */
        CSL_I2C_TEST_ENABLE = 1,
        /** Disable the Test mode                        */
        CSL_I2C_TEST_DISABLE = 2,
        /** Enable the Free run mode             */
        CSL_I2C_FREE_MODE   = 3,
        /** Enable the Stop mode                         */
        CSL_I2C_STOP_MODE  = 4,
        /** Enable the Test mode for Clocks      */
        CSL_I2C_TMODE_CLK  = 5,
        /** Enable the Loop back mode            */
        CSL_I2C_TMODE_LOOP = 6,
        /** Enable the Status bit test           */
        CSL_I2C_SET_STATUS_BITS = 7
 } CSL_I2cTestCmd;

/**************************************************************************\
* I2C global function declarations
\**************************************************************************/

/**  This function is idempotent in that calling it many times is same as
 *   calling it once. This function initializes the I2C CSL data structures.
 *
 * <b> Usage Constraints: </b>
 * CSL system initialization must be successfully completed by invoking
 * @a CSL_sysInit() before calling this function. This function should be
 * called before using any of the CSL APIs
 *
 * @b Example:
 * @verbatim


   ...
   CSL_sysInit();
   if (CSL_SOK != CSL_i2cInit()) {
       return;
   }
   @endverbatim
 *
 * @return returns the status of the operation
 *
 */
CSL_Status  CSL_i2cInit(
    void
);


/** The open call sets up the data structures for the particular instance of
 *  I2C device. The device can be re-opened anytime after it has been normally
 *  closed if so required. I2C Hardware setup will be performed at the end of
 *  the open call only if the HwSetup Pointer supplied was non- NULL. The handle
 *  returned by this call is input as an essential argument for rest of the APIs
 *  described for this module.
 *
 *  <b> Usage Constraints: </b>
 *  The CSL system as well as i2c must be successfully initialized
 *  via @a CSL_sysInit() and @a CSL_i2cInit() before calling this
 *  function. Memory for the @a CSL_i2cObj must be allocated outside
 *  this call. This object must be retained while usage of this peripheral.
 *
 *  @b Example:
 *  @verbatim

          CSL_I2cObj     i2cObj;
          CSL_I2cHwSetup i2cSetup;
          CSL_Status       status;
                ...
          hI2c = CSL_i2cOpen(&i2cObj,
                          CSL_I2C_0,
                          CSL_EXCLUSIVE,
                          &i2cSetup,
                          &status);
   @endverbatim
 *
 * @return returns a handle @a CSL_I2cHandle to the requested instance of
 * I2C if the call is successful, otherwise, a @a NULL is returned.
 *
 */
CSL_I2cHandle CSL_i2cOpen (
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
         */
    CSL_I2cObj              *hI2cObj,
    /** Instance of I2C to which a handle is requested
         */
    CSL_I2cNum              i2cNum,
    /** Specifies if I2C should be opened with exclusive or
     *  shared access to the associate pins
         */
    CSL_OpenMode            openMode,
    /** If a valid structure is passed (not @a NULL), then
     *  the @a CSL_i2cHwSetup() is called with this parameter
         */
    CSL_I2cHwSetup          *hwSetup,
    /** This returns the status (success/errors) of the call
         */
    CSL_Status              *status
        );

/**  The Close call releases the resource and appropriate shared pins.
 *
 * <b> Usage Constraints: </b>
 * Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 * in that order before @a CSL_i2cClose() can be called.
 *
 * @b Example:
 * @verbatim


   CSL_I2cHandle hI2c;
   ...
   CSL_i2cClose(hI2c);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_i2cClose(
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
         */
    CSL_I2cHandle                         hI2c
);


/** This function initializes the device registers with the appropriate values
 *  provided through the HwSetup Data structure. This function needs to be called
 *  only if the HwSetup Structure was not previously passed through the Open call.
 *  After the Setup is completed, the serial device is ready for data transfer.
 *  For information passed through the HwSetup Data structure refer
 *  @a CSL_I2cHwSetup.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called
 *  successfully in that order before this function can be called. The user
 *  has to allocate space for & fill in the main setup structure appropriately
 *  before calling this function
 *
 * @b Example:
 * @verbatim
     CSL_I2cHandle hI2c;
     CSL_I2cHwSetup hwSetup = CSL_I2C_HWSETUP_DEFAULTS;
     CSL_i2cHwSetup(hI2c, &hwSetup);
  @endverbatim
 *
 * @return Returns the status of the setup operation
 *
 */
CSL_Status  CSL_i2cHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
         */
    CSL_I2cHandle                         hI2c,
    /** Pointer to setup structure which contains the
     *  information to program I2C to a useful state
         */
    CSL_I2cHwSetup                        *setup
        );

/** This function gets the current setup of the I2C. The status is
 *  returned through @a CSL_I2cHwSetup. The obtaining of status
 *  is the reverse operation of @a CSL_i2cHwSetup() function.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cGetHwSetup() can be called.
 *
 * @b Example:
 * @verbatim
          CSL_I2cHandle hI2c;
          CSL_Status status;
          CSL_I2cHwSetup *mysetup;
           ...
          status = CSL_i2cGetHwSetup(hI2c, &mysetup);
   @endverbatim
 *
 * @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_i2cGetHwSetup(
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
     */
    CSL_I2cHandle                         hI2c,
    /** Pointer to setup structure which contains the
     *  information to program I2C to a useful state
     */
    CSL_I2cHwSetup                        *setup
);


/** Control operations for the I2C.  For a particular control operation, the
 *  pointer to the corresponding data type needs to be passed as argument HwControl
 *  function Call. All the arguments (Structure elements included) passed to  the
 *  HwControl function are inputs. For the list of commands supported and argument
 *  type that can be @a void* casted & passed with a particular command refer to
 *  @a CSL_I2cHwControlCmd.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cHwControl() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_I2cHwControlCmd
 *
 * @b Example:
 * @verbatim
       CSL_I2cHandle hI2c;
       CSL_Status status;
       ...
       status = CSL_i2cHwControl(hI2c,
                                    CSL_I2C_CMD_START,
                                   &command);
   @endverbatim
 *
 *  @return returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_i2cHwControl(
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
         */
    CSL_I2cHandle                         hI2c,
    /** The command to this API indicates the action to be taken
         */
    CSL_I2cHwControlCmd                   cmd,
    /** An optional argument @a void* casted
         */
    void                                     *arg
        );

/** Reads the received data from the data register.
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cRead() can be called.
 *
 * @b Example:
 * @verbatim
 *
      Uint16 *data_pro;
      CSL_I2cHandle hI2c;
      CSL_Status status;
      ...
      status = CSL_i2cRead(hI2c,&data_pro );
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status CSL_i2cRead(
        /**  Handle of already opened peripheral
         */
        CSL_I2cHandle hI2c,
        /** Pointer to memory where data will be read and stored
         */
        void* buf
);

/** This function writes the specified data into I2C data register..
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cWrite() can be called.
 *
 *  @b Example:
 *  @verbatim

      CSL_I2cHandle hI2c;
      ...
      status = CSL_i2cWrite(hI2c,&data_out );
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status CSL_i2cWrite(
        /** Handle of already opened peripheral
         */
        CSL_I2cHandle hI2c,
        /** Pointer to data to be written
         */
        void* buf
) ;

/** This function is used to read the current device configuration, status flags
 *  and the value present associated registers. Following table details the various
 *  status queries supported and the associated data structure to record the response.
 *  User should allocate memory for the said data type and pass its pointer as an
 *  unadorned void* argument to the status query call. For details about the various
 *  status queries supported and the associated data structure to record the response,
 *  refer to @a CSL_I2cHwStatusQuery
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cGetHwStatus() can be called. For the
 *  argument type that can be @a void* casted & passed with a particular command
 *  refer to @a CSL_I2cHwStatusQuery
 *
 * @b Example:
 * @verbatim
          CSL_I2cHandle hI2c;
          CSL_Status status;
          Uint16  *response;
           ...
          status = CSL_i2cGetHwStatus(hI2c,
                           CSL_I2C_QUERY_BUS_BUSY,
                                  &response);
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status  CSL_i2cGetHwStatus(
    /** Pointer to the object that holds reference to the
     *  instance of I2C requested after the call
         */
    CSL_I2cHandle                         hI2c,
    /** The query to this API which indicates the status
     *  to be returned
         */
    CSL_I2cHwStatusQuery                  query,
    /** Placeholder to return the status. @a void* casted */
    void                                    *response
        );

/** This function is used for the testing of the I2C module.
 *  For details about the various test commands supported refer
 *  @a CSL_I2cTestCmd
 *
 *  <b> Usage Constraints: </b>
 *  Both @a CSL_i2cInit() and @a CSL_i2cOpen() must be called successfully
 *  in that order before @a CSL_i2cTest() can be called.
 *
 * @b Example:
 * @verbatim

      CSL_I2cHandle hI2c;
      CSL_Status status;
       ...
      status = CSL_i2cTest(hI2c,CSL_I2C_TMODE_CLK );
   @endverbatim
 *
 * @return Returns the status of the operation (see @a CSL_Status)
 *
 */
CSL_Status CSL_i2cTest(
        /** Handle of already opened peripheral
         */
        CSL_I2cHandle hI2c,
        /** Test command
         */
        CSL_I2cTestCmd command
);

#ifdef __cplusplus
}
#endif


#endif
