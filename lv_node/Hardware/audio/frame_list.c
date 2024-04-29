/*
 * @Description: 储存MP3每一帧位置到链表
 * @Author: chen
 * @Date: 2023-12-12 17:06:44
 * @LastEditors: chen
 * @LastEditTime: 2023-12-18 11:27:38
 */





#include "ui_app.h"

// 预处理获取每帧信息到链表，播放时使用链表的帧信息
FrameList *frame_head = NULL;

// 在链表尾部插入元素  
void frame_list_insert(FrameList **head, uint32_t pos, uint32_t size, uint32_t count) 
{  
    FrameList *new_node = (FrameList*)malloc(sizeof(FrameList));  
    if(new_node == NULL)
    {
        perror("malloc err");
        return;
    }
    new_node->pos   = pos;  
    new_node->size  = size;  
    new_node->count = count;  
    new_node->next  = NULL;  

    if(*head == NULL){
        *head = new_node;
    }
    else{
        FrameList *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
     
  
} 

// 销毁链表，释放所有节点的内存  
void destroy_frame_list(FrameList *head) 
{  
    if(head == NULL)   return;

    FrameList *temp = head;  
    while (temp != NULL) {  
        FrameList *next = temp->next;  
        free(temp);  
        temp = next;  
    }  

    frame_head = NULL; // 头节点也被释放，所以将其置为NULL  
}

// 通过帧序号查找链表节点
FrameList *get_node_by_count(int need)
{
    FrameList *temp = frame_head;  
    while (temp != NULL) 
    {  
        if(temp->count == need)
            return temp;
        temp = temp->next;  
    }  

    return NULL;
}

void print_frame_list(FrameList *head)
{
    FrameList *temp = head;  
    while (temp != NULL) 
    {  
       printf("pos %d, size %d, count %d time %lld\n", temp->pos, temp->size, temp->count, temp->dura);
  
        temp = temp->next;  
    }  
}