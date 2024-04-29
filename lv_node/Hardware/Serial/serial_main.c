/**
 * Here is a demo DEMO
 * The use of several API interfaces is demonstrated here
 * So the data sending and receiving is to initialize and call the API.
 */

#include "ui_app.h"

serial_handler_t *serial_handler;
void serial_init(uint8_t port, uint32_t baud_rate, uint8_t data_width, uint8_t parity, uint8_t nstop)
{
    if (serial_hander_init(&serial_handler, port, baud_rate, data_width, parity, nstop) != SERIAL_SUCCESS) // 使用通道一485口
    {
        printf("serial_init error!\r\n");
    }
}

void serial_uinit()
{
    serial_hander_uninit(serial_handler);
}

void serial_send(const char *text)
{
    serial_status_t status = SERIAL_SUCCESS;
    sprintf((char *)serial_handler->serial_frame_buff, text);
    serial_handler->serial_frame_length = strlen((char *)serial_handler->serial_frame_buff);
    serial_handler->frame_send(serial_handler); // 发送数据
}

void serial_recv(void)
{
    serial_status_t status = SERIAL_SUCCESS;

    status = serial_handler->frame_recv(serial_handler);
    if (status == SERIAL_SUCCESS)
    {

        // 定义足够大的缓冲区，用于存储拼接后的字符串
        char print_buffer[1024]; // 根据需要设置足够大的大小

        int print_length = 0;

        for (size_t i = 0; i < serial_handler->serial_frame_length; i++)
        {
            print_length += snprintf(print_buffer + print_length, sizeof(print_buffer) - print_length, "%c", serial_handler->serial_frame_buff[i]);
        }

        // 将拼接后的数据传递给update_text_area函数
        update_text_area(print_buffer, 2);
    }
}

void *recv_port(void *args)
{

// 判断一下串口打开没有
    while (1)
    {
        serial_recv();
    }
}
