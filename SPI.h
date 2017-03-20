/* 
 * File:   SPI.h
 * Author: kpa
 *
 * Created on June 30, 2016, 2:02 PM
 */

#ifndef SPI_H
#define SPI_H

#include <Cdef21489.h>
#include <sru21489.h>
#include "struct.h"
#include "mydef.h"

#ifdef __cplusplus
extern "C"
{
#endif

void select_spi_slave(uint adr, uint freq); //  выбор адреса spib

/* deselect all SPIB slaves */
void spib_cs_set_high(void);

/* Wait for the SPI TX buffer is empty. */
void spib_wait_till_tx_buffer_is_empty(void);

/* Wait for the SPI External Transaction Complete*/
void spib_wait_till_external_transaction_complete(void);

#ifdef __cplusplus
}
#endif

#endif /* SPI_H */

