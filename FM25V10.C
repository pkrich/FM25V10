#include "FM25V10.H"

/* write to F-RAM enable */
void fm25v10_write_enable(void) {
  /* set SPI controller to 8 bit mode */
  fm25v10_prepare_spi(WL8);

  /*send command "write enable" to FM25V10 */
  fm25v10_data_put(FM25V10_CMD_WREN);

  spib_wait_till_external_transaction_complete();

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* write to F-RAM disable */
void fm25v10_write_disable(void) {
  /* set SPI controller to 8 bit mode */
  fm25v10_prepare_spi(WL8);

  /*send command "write disable" to FM25V10 */
  fm25v10_data_put(FM25V10_CMD_WRDI);

  spib_wait_till_external_transaction_complete();

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* read status register of FM25V10 */
void fm25v10_status_read(byte* status) {

  /* set SPI controller to 16 bit mode */
  fm25v10_prepare_spi(WL16);

  /*send command "read status register" with trailing zeroes to FM25V10 */
  fm25v10_data_put(FM25V10_CMD_RDSR << 8);

  /* get lower byte only */
  *status = fm25v10_data_get() & 0xff;

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* write to status register of FM25V10 */
void fm25v10_status_write(byte *data) {
  /*send command "write enable" to FM25V10 */
  fm25v10_write_enable();

  /* set SPI controller to 16 bit mode */
  fm25v10_prepare_spi(WL16);

  /*send command "write status register" combined with data to FM25V10 */
  fm25v10_data_put((FM25V10_CMD_WRSR << 8) | *data);

  spib_wait_till_external_transaction_complete();

  /*send command "write disable" to FM25V10 */
  fm25v10_write_disable();

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* 
 * write data to FM25V10:
 *  address - start address to write data to;
 *  data[]  - array of data to write
 *  data_length - width of data unit in data[], possible values are:
 *	WL8   - for byte data array;
 *	WL16  - for 16 bit word data array;
 *	WL32  - for 32 bit word data array;
 *  count   - number of data to write
 */
void fm25v10_data_write(uint address, uint* data, uint data_length, uint count) {
  uint index, message;

  /* form 32 bit message from 8 bit command and 24 bit start address */
  message = (FM25V10_CMD_WRITE << 24) | address;

  /*send command "write enable" to FM25V10 */
  fm25v10_write_enable();

  /* set SPI controller to 32 bit mode */
  fm25v10_prepare_spi(WL32);

  /* send write command and start address to FM25V10 */
  fm25v10_data_put(message);

  spib_wait_till_external_transaction_complete();

  /* set SPI controller to 8 or 16 bit mode 
   * is suboptimal for large transfers
   * because of overhead for each transfer
   * and to be revised for maximizing to improve performance */

  /* set SPI controller transfer unit */
  fm25v10_prepare_spi(data_length);

  /* write data to FM25V10 */
  /* loop through all requested data*/
  for (index = 0; index < count; index++) {
    /* send data to FM25V10 by data units*/
    fm25v10_data_put(data[index]);
  }

  spib_wait_till_external_transaction_complete();

  /* deselect all SPIB slaves */
  spib_cs_set_high();

  /*send command "write disable" to FM25V10 */
  fm25v10_write_disable();
}

/* read data from FM25V10:
 *  address - start address to read data from
 *  data[]  - destination array of data to read to
 *  data_length - width of data unit in data[], possible values are:
 *	WL8   - for byte data array;
 *	WL16  - for 16 bit word data array;
 *	WL32  - for 32 bit word data array;
 *  count   - number of data to read
 */
void fm25v10_data_read(uint address, uint* data, uint data_length, uint count) {
  uint index, message;

  /* form 32 bit message from 8 bit command and 24 bit start address */
  message = (FM25V10_CMD_READ << 24) | address;

  /* set SPI controller to 32 bit mode */
  fm25v10_prepare_spi(WL32);

  /* send read command and start address to FM25V10 */
  fm25v10_data_put(message);

  spib_wait_till_external_transaction_complete();

  /* set SPI controller to 8 or 16 bit mode 
   * is suboptimal for large transfers
   * because of overhead for each transfer
   * and to be revised for maximizing to improve performance */

  /* set SPI controller transfer unit */
  fm25v10_prepare_spi(data_length);

  /* loop through all requested data*/
  for (index = 0; index < count; index++) {
    /* clock SPI with no data sent while get data from FM25V10*/
    fm25v10_data_put(0);

    /* get data from SPI receive buffer */
    data[index] = fm25v10_data_get();
  }

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* send data to FM25V10 */
void fm25v10_data_put(uint data) {
  /* write data to SPI send buffer*/
  *pTXSPIB = data;
  /* Wait for the SPI TX buffer is empty. */
  spib_wait_till_tx_buffer_is_empty();
}

/* get data from FM25V10 */
uint fm25v10_data_get(void) {
  /* Wait for the SPI External Transaction Complete*/
  spib_wait_till_external_transaction_complete();
  return *pRXSPIB;
}

/* get 9 byte JEDEC device ID */
void fm25v10_get_id(byte * ID) {
  byte index;

  /* set SPI controller to 8 bit mode */
  fm25v10_prepare_spi(WL8);

  /* send command "read JEDEC ID" to FM25V10 */
  fm25v10_data_put(FM25V10_CMD_RDID);

  /* loop through all requested data*/
  for (index = 0; index < 9; index++) {

    /* clock SPI with no data sent while get data from FM25V10*/
    fm25v10_data_put(0);

    /* get data from SPI receive buffer */
    ID[index] = fm25v10_data_get();
  }

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* get 8 byte serial number (FM25VN10 only)*/
void fm25v10_get_sn(uint * SN) {
  byte index;

  /* set SPI controller to 8 bit mode */
  fm25v10_prepare_spi(WL8);

  /* send command "read serial number" to FM25V10 */
  fm25v10_data_put(FM25V10_CMD_SNR);

  /* loop through all requested data*/
  for (index = 0; index < 8; index++) {

    /* clock SPI with no data sent while get data from FM25V10*/
    fm25v10_data_put(0);

    /* get data from SPI receive buffer */
    SN[index] = fm25v10_data_get();
  }

  /* deselect all SPIB slaves */
  spib_cs_set_high();
}

/* 
 * prepare SPI controller for communication with FM25V10:
 * unset chip select signal, set bus speed...
 *  parameter data_length, one of:
 *    WL8, 
 *    WL16, 
 *    WL32
 *  correspond to transfer of 8, 16 or 32 bits in one transaction
 */
void fm25v10_prepare_spi(uint data_length) {
  select_spi_slave(fz.afram, 5000000); // 40 MHz max
  SRU(SPIB_MISO_PBEN_O, DPI_PBEN12_I);
  SRU(DPI_PB12_O, SPIB_MISO_I); //Connect DPI PB12 to MISO.
  *pSPICTLB = (TXFLSH | RXFLSH); // flush transmit and receive buffers
  *pSPICTLB = (SPIEN | SPIMS | data_length | MSBF | TIMOD1 | GM | CLKPL | CPHASE);
  *pSPIFLGB = (SPIFLG1 | SPIFLG2 | SPIFLG3 | DS0EN); // 0xe01; //включение CS
}

/* 
 * test FM25V10 functions
 * return code:
 *	bit_0 - get_id fail;
 *	bit_1 - disable write to FM25V10 fail;
 *	bit_2 - enable write to FM25V10 fail;
 *	bit_3 - write reference bytes then read and compare fail; 
 */
word fm25v10_test(void) {
  word result = 0;
  uint index;
  byte ID[9];
  byte SN[8];
  byte temp_array[1024]; // for temporary store data from FRAM
  byte status;
  byte FM25V10_ID[9] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xC2, 0x24, 0x00};

  uint reference_array[1024]; // array with test data
  uint test_array[1024]; // temp array for read

  /*****************************************************************************
   *  test get id (JEDEC identification)
   ****************************************************************************/
  fm25v10_get_id(&ID[0]);
  if (memcmp((void*) &FM25V10_ID[0], &ID[0], 9) != 0)
    result = 1;
  
  /*****************************************************************************
   * test disable write to FM25V10;
   ****************************************************************************/
  fm25v10_write_disable();
  fm25v10_status_read(&status); /* get status */
  if ((status & FM25V10_STA_WEL) != 0)
    result |= 2;
  
  /*****************************************************************************
   * test enable write to FM25V10;
   ****************************************************************************/
  fm25v10_write_enable();
  fm25v10_status_read(&status); /* get status */
  if ((status & FM25V10_STA_WEL) == 0)
    result |= 4;

  /*****************************************************************************
   * test write, read and compare reference bytes;
   ****************************************************************************/
  /* fill reference array */
  for (index = 0; index < 1024; index++) {
    reference_array[index] = index & 0xff; // we need 1 byte only
  }

  /* loop through all FM25V10 memory cells*/
  for (index = 0; index < 128; index++) {
    asm(" bit tgl flags FLG3;"); //watchdog
    asm(" bit tgl flags FLG1;"); //LED

    /* read data from FM25V10 to temp array for future restore */
    fm25v10_data_read(index * 1024, (uint*)&temp_array[0], WL8, 1024);

    asm(" bit tgl flags FLG3;"); //watchdog
    asm(" bit tgl flags FLG1;"); //LED

    /* write data from reference array */
    fm25v10_data_write(index * 1024, &reference_array[0], WL8, 1024);

    asm(" bit tgl flags FLG3;"); //watchdog
    asm(" bit tgl flags FLG1;"); //LED

    /* read data from FM25V10 to test array */
    fm25v10_data_read(index * 1024, &test_array[0], WL8, 1024);

    asm(" bit tgl flags FLG3;"); //watchdog
    asm(" bit tgl flags FLG1;"); //LED

    /* write stored data from temp array */
    fm25v10_data_write(index * 1024, (uint*)&temp_array[0], WL8, 1024);

    asm(" bit tgl flags FLG3;"); //watchdog
    asm(" bit tgl flags FLG1;"); //LED

    /* compare data in reference and in test array */
    if (memcmp(&reference_array[0], &test_array[0], 1024) != 0)
      result |= 8;
  }

  return result;
}
