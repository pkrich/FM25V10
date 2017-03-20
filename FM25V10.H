/* 
 * File:   FM25V10.H
 * Author: kpa
 *
 * Created on August 19, 2016, 10:17 AM
 */


#ifndef FM25V10_H
#define FM25V10_H

#include <stdlib.h>
#include <string.h>
#include "SPI.h"

#include <processor_include.h>

// Commands
#define FM25V10_CMD_WREN     (unsigned)0x06  // Set Write Enable Latch
#define FM25V10_CMD_WRDI     (unsigned)0x04  // Write Disable

#define FM25V10_CMD_RDSR     (unsigned)0x05  // Read  Status register
#define FM25V10_CMD_WRSR     (unsigned)0x01  // Write Status register

#define FM25V10_CMD_READ     (unsigned)0x03  // Read Memory data
#define FM25V10_CMD_FSTRD    (unsigned)0x0B  // Fast Read Memory data
#define FM25V10_CMD_WRITE    (unsigned)0x02  // Write memory data

#define FM25V10_CMD_SLEEP    (unsigned)0xB9  // Enter Sleep Mode

#define FM25V10_CMD_RDID     (unsigned)0x9F  // Read DeviceID
#define FM25V10_CMD_SNR      (unsigned)0xC3  // Read Serial Number

#define FM25V10_CMD_BULK     (unsigned)0xC7  // Bulk   Erase FULL
#define FM25V10_CMD_SEER     (unsigned)0xD8  // Sector Erase 64k
#define FM25V10_CMD_SER      (unsigned)0x20  // Sector Erase  4k

//Status Register
#define FM25V10_STA_WEL      (byte)0x02      // Write Enable Bit
#define FM25V10_STA_BP0      (byte)0x04      // Block Protect bit С0Т
#define FM25V10_STA_BP1      (byte)0x08      // Block Protect bit С1Т
#define FM25V10_STA_WPEN     (byte)0x80      // Write Protect Pin Enable bit


#ifdef __cplusplus
extern "C" {
#endif

    //////////////////// верхний уровень ////////////////////////////////

    /* 
     * write data to FM25V10:
     *  address - start address to write data to;  in bytes
     *  data[]  - array of data to write
     *  data_length - width of data unit in data[], possible values are:
     *	WL8   - for byte data array;
     *	WL16  - for 16 bit word data array;  //MSB first
     *	WL32  - for 32 bit word data array;  //MSB first
     *  count   - number of data to write
     */
    void fm25v10_data_write(uint address,
            uint* data,
            uint data_length,
            uint count);

    /* read data from FM25V10:
     *  address - start address to read data from   in bytes
     *  data[]  - destination array of data to read to
     *  data_length - width of data unit in data[], possible values are:
     *	WL8   - for byte data array;
     *	WL16  - for 16 bit word data array;//MSB first
     *	WL32  - for 32 bit word data array;
     *  count   - number of data to read
     */
    void fm25v10_data_read(uint address,
            uint* data,
            uint data_length,
            uint count);

    /* write to F-RAM enable */
    void fm25v10_write_enable(void);

    /* write to F-RAM disable */
    void fm25v10_write_disable(void);

    /* read status register of FM25V10 */
    void fm25v10_status_read(byte* status);

    /* write to status register of FM25V10 */
    void fm25v10_status_write(byte *data);


    /* send data to FM25V10 */
    void fm25v10_data_put(uint data);

    /* get data from FM25V10 */
    uint fm25v10_data_get(void);

    /* get 9 byte JEDEC device ID */
    void fm25v10_get_id(byte * ID);

    /* get 8 byte serial number (FM25VN10 only)*/
    void fm25v10_get_sn(uint * SN);

    /* 
     * prepare SPI controller for communication with FM25V10
     *  parameter data_length, one of:
     *    WL8, 
     *    WL16, 
     *    WL32
     *  correspond to transfer of 8, 16 or 32 bits in one transaction
     */
    void fm25v10_prepare_spi(uint data_length);

    /* test FM25V10 functions*/
    word fm25v10_test(void);

#ifdef __cplusplus
}
#endif

#endif /* FM25V10_H */

