/*
 * bin_fifo.h
 *
 *  Created on: 18 сент. 2020 г.
 *      Author: John
 */

#ifndef SRC_BIN_FIFO_H_
#define SRC_BIN_FIFO_H_
#include <stdint.h>

#define fifo_bin_index_t uint16_t

//FIFO for bytes
//FIFO for bytes data structure
typedef struct{
  fifo_bin_index_t NewI;            //Start index (place for new data)
  fifo_bin_index_t OldI;            //Finish index (oldest data index)
  fifo_bin_index_t Count;           //Total byte count stored in fifo
  fifo_bin_index_t Total_SZ;        //Total number of bytes stored in fifo
  uint8_t* Buffer;                  //Pointer for FIFO buffer (static allocation)
}fifo_bin_t;

//Define byte fifo
#define fifo_bin_def(name,sz)\
uint8_t name##_buf[sz];\
fifo_bin_t name = {.NewI=0,.OldI=0,.Count=0,.Total_SZ=sz,.Buffer=name##_buf};


//put data in fifo
//fifo - pointer to fifo data structure
//byte - byte for put
//return - 0 when no rewrite
int8_t fifo_bin_put_byte(fifo_bin_t* fifo, uint8_t byte);

//get data from fifo
//fifo - pointer to fifo data structure
//byte - pointer to byte
//return - 0 when get suceed
int8_t fifo_bin_get_byte(fifo_bin_t* fifo, uint8_t* byte);

//put data in fifo
//fifo - pointer to fifo data structure
//byte - pointer to byte array
//len - length of byte array (in bytes)
//return - 0 when no rewrite
int8_t fifo_bin_put(fifo_bin_t* fifo, uint8_t* byte,fifo_bin_index_t len);

//get data from fifo
//fifo - pointer to fifo data structure
//byte - pointer to byte array
//len - length of byte array (in bytes)
//return - 0 when get suceed
int8_t fifo_bin_get(fifo_bin_t* fifo, uint8_t* byte,fifo_bin_index_t len);

//delite all data in fifo
//fifo - pointer to fifo data structure
void fifo_bin_flash(fifo_bin_t* fifo);

//get data count in fifo
//fifo - pointer to fifo data structure
//return - number of bytes in fifo
fifo_bin_index_t fifo_bin_count(fifo_bin_t* fifo);

#endif /* SRC_BIN_FIFO_H_ */
