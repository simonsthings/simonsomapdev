/*
 *  Copyright 2004 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  ======== osk5912_i2c.h ========
 *      I2C implementation
 */

#include <osk5912.h>
#include <osk5912_i2c.h>

/* ------------------------------------------------------------------------ *
 *  OSK5912_I2C_init( )                                                     *
 *      Initialize I2C - self address is 0xCC                               *
 *                                                                          *
 *      The interrupts are not configured b/c this example will only deal   *
 *      with polling.                                                       *
 * ------------------------------------------------------------------------ */
void OSK5912_I2C_init( )
{
    Uint16 selfAddress = 0xCC;

    /* Disable I2C and set to default state */
    I2C_CON = 0;

    OSK5912_waitusec( 500 );

    /* Disable interrupts */
    I2C_IE = 0;

    /* Set Clock Prescaler ( since CK_REF = 12MHz ) no scale down needed */
    I2C_PSC = 0;

    /* Set SCL low time & SCL high time */
    /* ( 12MHz clock / ( 400kHz i2c clk * 2 ) ) minus 6 for reg. setting */
    /* Equivalent to ( ( 12 * 1000 ) / ( 400 * 2 ) ) - 6; */
    I2C_SCLH = I2C_SCLL = 9;

    /* Set Own Address - cannot conflict with other devices addresses */
    I2C_OA = selfAddress & I2C_OA_MASK;

    /* Enable I2C */
    I2C_CON = I2C_CON_I2C_EN;

    /* Set the Slave address to 0, this is only temporary */
    I2C_SA = 0;

    /* Set Data Count */
    I2C_CNT = 0;

    /* Enable interrupts */
    /* NOTE: for this example, polling is used instead of interrupts */
    I2C_IE = 0
          //| I2C_IE_GC_IE      // General Call
          //| I2C_IE_XRDY_IE    // Transmit Data Ready
          //| I2C_IE_RRDY_IE    // Receive Data Ready
          //| I2C_IE_ARDY_IE    // Register Access Ready
          //| I2C_IE_NACK_IE    // No Acknowledgement
          //| I2C_IE_AL_IE      // Arbitration Lost
            ;
}

/* ------------------------------------------------------------------------ *
 *  OSK5912_I2C_read( slave_addr, data, length )                            *
 *      Read from I2C                                                       *
 *      Returns:                                                            *
 *          0 - Pass                                                        *
 *          1001 - Fail ( Errors - Arbitration Lost )                       *
 *          1002 - Fail ( Errors - NACK )                                   *
 *          2001 - Fail ( Timeout - Bus Busy )                              *
 *          2002 - Fail ( Timeout - Read )                                  *
 * ------------------------------------------------------------------------ */
Uint16 OSK5912_I2C_read( Uint16 slave_addr, Uint8* data, Uint16 length )
{
    Uint16 *pdata = ( Uint16 * )data;
    Uint16 status;
    Int16 i, timecount, timeout = 0x1000;

    /* Set slave address */
    I2C_SA = slave_addr;

    /* Set data count */
    I2C_CNT = length;

    /* Poll BB field on I2C_STAT - Wait for bus to become free */
    for ( i = 0 ; I2C_STAT & I2C_STAT_BB ; i++ )
        if ( i > timeout )
            return 2001;

    /* Configure Options for Receiving - Master Reciever mode */
    I2C_CON = 0
            | I2C_CON_I2C_EN    // I2C Enable
            | I2C_CON_MST       // Master Mode
            | I2C_CON_RX        // Receive Mode
            ;

    /*  Set mode:
     *  OSK5912_I2C_MODE_SADP // (S)tart..(A)ddr..(D)ata..(n)..sto(P)
     *  OSK5912_I2C_MODE_SADD // (S)tart..(A)ddr..(D)ata..(n)..(D)ata */
    I2C_CON |= OSK5912_I2C_MODE_SADP;


    /* Poll until complete */
    for ( timecount = 0 ; timecount < timeout ; timecount++ )
    {
        /* Read I2C Status register */
        status = I2C_STAT;

        /* Check for NACK */
        if ( ( status & I2C_STAT_NACK ) != I2C_STAT_NACK )
        {
            /* Check for ARDY */
            if ( ( status & I2C_STAT_ARDY ) != I2C_STAT_ARDY )
            {
                /* Check for RRDY */
                if ( ( status & I2C_STAT_RRDY ) == I2C_STAT_RRDY )
                {
                    /* Read Data */
                    if ( length == 1 )
                    {
                        *pdata = I2C_DATA & 0xFF;
                        length--;
                    }
                    else
                    {
                        *pdata++ = I2C_DATA;
                        length -= 2;
                    }

                    /* Clear Receive Ready field */
                    I2C_STAT &= I2C_STAT_RRDY;
                }
            }

            /* Check for Errors - refresh status register */
            status = I2C_STAT;

            /* Check for Arbitration Lost */
            if ( ( status  & I2C_STAT_AL ) == I2C_STAT_AL )
            {
                I2C_STAT &= I2C_STAT_AL;
                return 1001;
            }

            /* Check for NACK */
            if ( ( status & I2C_STAT_NACK ) == I2C_STAT_NACK )
            {
                I2C_STAT &= I2C_STAT_NACK;
                return 1002;
            }

            /* Check for Register Access Ready - Good Condition */
            if ( ( I2C_STAT & I2C_STAT_ARDY ) == I2C_STAT_ARDY )
            {
                I2C_STAT &= I2C_STAT_ARDY;
                return 0;
            }
        }
    }

    /* I2C Timeout */
    return 2002;
}


/* ------------------------------------------------------------------------ *
 *  OSK5912_I2C_write( Uint16 slave_addr, Uint8* data, Uint16 length )      *
 *      Write to I2C                                                        *
 * ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ *
 *  OSK5912_I2C_write( slave_addr, data, length )                           *
 *      Write to I2C                                                        *
 *      Returns:                                                            *
 *          0 - Pass                                                        *
 *          1001 - Fail ( Errors - Arbitration Lost )                       *
 *          1002 - Fail ( Errors - NACK )                                   *
 *          2001 - Fail ( Timeout - Bus Busy )                              *
 *          2002 - Fail ( Timeout - Write )                                 *
 * ------------------------------------------------------------------------ */
Uint16 OSK5912_I2C_write( Uint16 slave_addr, Uint8* data, Uint16 length )
{
    Uint16 *pdata = ( Uint16 * )data;
    Uint16 last_byte = ( data[length-1] & 0xFF );
    Uint16 status;
    Int16 i, timecount, timeout = 0x1000;

    /* Set slave address */
    I2C_SA = slave_addr;

    /* Set data count */
    I2C_CNT = length;

    /* Poll BB field on I2C_STAT - Wait for bus to become free */
    for ( i = 0 ; I2C_STAT & I2C_STAT_BB ; i++ )
        if ( i > timeout )
            return 2001;

    /* Configure Options for Transmitting - Master Transmit mode */
    I2C_CON = 0
            | I2C_CON_I2C_EN    // I2C Enable
            | I2C_CON_MST       // Master Mode
            | I2C_CON_TX        // Transmit Mode
            ;

    /*  Set Mode:
     *  OSK5912_I2C_MODE_SADP // (S)tart..(A)ddr..(D)ata..(n)..sto(P)
     *  OSK5912_I2C_MODE_SADD // (S)tart..(A)ddr..(D)ata..(n)..(D)ata */
    I2C_CON |= OSK5912_I2C_MODE_SADP;

    /* Poll until complete */
    for ( timecount = 0 ; timecount < timeout ; timecount++ )
    {
        /* Read I2C Status register */
        status = I2C_STAT;

        /* Check for NACK */
        if ( ( status & I2C_STAT_NACK ) != I2C_STAT_NACK )
        {
            /* Check for ARDY */
            if ( ( status & I2C_STAT_ARDY ) != I2C_STAT_ARDY )
            {
                /* Check for XRDY */
                if ( ( status & I2C_STAT_XRDY ) == I2C_STAT_XRDY )
                {
                    /* Send Data */
                    if ( length == 1 )
                    {
                        I2C_DATA = last_byte;
                        length--;
                    }
                    else
                    {
                        I2C_DATA = *pdata++;
                        length -= 2;
                    }

                    /* Clear Transmit Ready field */
                    I2C_STAT &= I2C_STAT_XRDY;
                }
            }

            /* Check for Errors - refresh status register */
            status = I2C_STAT;

            /* Check for Arbitration Lost */
            if ( ( status  & I2C_STAT_AL ) == I2C_STAT_AL )
            {
                I2C_STAT &= I2C_STAT_AL;
                return 1001;
            }

            /* Check for NACK */
            if ( ( status & I2C_STAT_NACK ) == I2C_STAT_NACK )
            {
                I2C_STAT &= I2C_STAT_NACK;
                return 1002;
            }

            /* Check for Register Access Ready - Good Condition */
            if ( ( I2C_STAT & I2C_STAT_ARDY ) == I2C_STAT_ARDY )
            {
                I2C_STAT &= I2C_STAT_ARDY;
                return 0;
            }
        }
    }

    /* I2C Timeout */
    return 2002;
}
