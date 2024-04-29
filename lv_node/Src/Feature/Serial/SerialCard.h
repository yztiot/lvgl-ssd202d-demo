#ifndef _LV_CARD_PORT_H
#define _LV_CARD_PORT_H

#define MSG_BUFFER_SIZE 2048

#define PORT_BAUDRATE "9600\n"   \
                      "4800\n"   \
                      "19200\n"  \
                      "38400\n"  \
                      "57600\n"  \
                      "115200\n" \
                      "460800\n" \
                      "2400"

#define PORT_DATABIT "8\n" \
                     "7\n" \
                     "6\n" \
                     "5"
#define PORT_STOPBIT "1\n"   \
                     "1.5\n" \
                     "2"
#define PORT_CHECKBIT "None\n"      \
                      "Odd\n"       \
                      "Eyen Mark\n" \
                      "Space"

typedef enum
{
    LV_PORT_BAUDRATE,
    LV_PORT_CHECKBIT,
    LV_PORT_DATABIT,
    LV_PORT_STOPBIT
} PortType;

typedef struct
{
    uint8_t port;       // 端口号
    uint8_t data_width; // 数据位
    uint8_t parity;     // 校验位
    uint8_t nstop;      // 停止位
    uint32_t baud_rate; // 波特率

} lv_Port_data_t;


void SerialCard(void);
void update_text_area(const char *message, uint8_t is_sent);

#endif