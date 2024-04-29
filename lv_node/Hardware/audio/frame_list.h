/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-12-12 17:06:55
 * @LastEditors: chen
 * @LastEditTime: 2024-02-06 15:44:13
 */



#ifndef _FRAME_LIST_H_
#define _FRAME_LIST_H_

#include <stdint.h>

// 音频帧链表
typedef struct frame_list
{
    uint32_t pos;   // 帧头位置
    uint32_t size;  // 帧长
    uint32_t count; // 帧序号
    uint64_t dura;  // 帧所在时间
    struct frame_list *next;
}FrameList;

extern FrameList *frame_head;

void frame_list_insert(FrameList **head, uint32_t pos, uint32_t size, uint32_t count) ;
void destroy_frame_list(FrameList *head) ;
void print_frame_list(FrameList *head);
FrameList *get_node_by_count( int need);



#endif