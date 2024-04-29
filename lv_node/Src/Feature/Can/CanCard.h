/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-27 11:28:50
 * @LastEditors: chen
 * @LastEditTime: 2023-12-11 11:43:23
 */

#define CAN_PORT_BAUDRATE "100000\n"   \
                          "125000\n"   \
                          "250000\n"   \
                          "500000"  
         

#ifndef _LV_CARD_CAN_H
#define _LV_CARD_CAN_H

void CanCard(void);

#endif