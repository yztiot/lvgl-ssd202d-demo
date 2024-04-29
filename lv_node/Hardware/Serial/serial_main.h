#include "serial_mutex.h"
#include "serial_api.h"
#include "serial_define.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


/**
 * Serial port initialization
 * @param  handler This is the struct pointer
 * @return no return value
 */
void serial_init(uint8_t port, uint32_t baud_rate, uint8_t data_width, uint8_t parity, uint8_t nstop);


/**
 * Serial port uninitialization
 * @param  serial port pointer
 * @return no return value
 */
void serial_uinit();


/**
 * Serial send data
 * @param  what to send
 * @return no return value
 */

void serial_send(const char *text);


/**
 *print received content
 * @param NULL
 * @return no return value
 */
void serial_recv(void);


void *recv_port(void *args);
