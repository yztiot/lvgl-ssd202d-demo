/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-12-07 11:55:26
 * @LastEditors: chen
 * @LastEditTime: 2023-12-18 11:18:44
 */

#ifndef _AUDIO_LIST_H
#define _AUDIO_LIST_H

// 定义音频路径链表节点结构体 
typedef struct  Node{  
    char path[128];   
    struct Node* prev;   // 前一个节点  
    struct Node* next;   // 后一个节点  
} Node;  
extern Node *au_head;


void insertNode(Node **head, char *data);
Node *findNode(Node *head, char *data);
void destroyList(Node** head);
void printList(Node *head);

#endif