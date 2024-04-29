/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-12-07 11:55:18
 * @LastEditors: chen
 * @LastEditTime: 2023-12-18 14:11:04
 */

#include "ui_app.h"

 

Node *au_head = NULL; // 音频链表头结点

// 创建新节点  
Node* createNode(char* data) 
{  
    Node* newNode = (Node*)malloc(sizeof(Node));  
    strcpy(newNode->path, data);   // 复制数据内容  
    newNode->prev = newNode;  
    newNode->next = newNode;  
    return newNode;  
}  
  
// 插入节点到链表尾部  
void insertNode(Node **head, char *data) 
{  
    Node* newNode = createNode(data);  
    if (*head == NULL) {   // 如果链表为空，将新节点设置为头节点  
        *head = newNode;  
    } 
    else 
    {   // 否则，将新节点插入到尾部  
        Node* curr = (*head)->prev;   
        curr->next = newNode;  
        newNode->prev = curr;  
        newNode->next = (*head);
        (*head)->prev = newNode;
    }  
}  
  
 
  
// 查找节点并输出其数据内容  
Node *findNode(Node *head, char *data) 
{  
    Node* curr = head;  
    if(curr == NULL)    return NULL;    
    do
    {   
        // 查找要查找的节点  
        if (strcmp(curr->path, data) == 0) 
        {   // 如果找到了节点  
            printf("Found node with data: %s\n", curr->path); 
            return curr; 
        }
        curr = curr->next;  
        
    }while (curr != head);
    return NULL;    
}

// 销毁链表  
void destroyList(Node** head) 
{  

    if(*head == NULL)   return;
    Node* curr = (*head)->next;  
    
    while (curr != (*head)) // 遍历链表并逐个释放节点内存 
    {    
        Node* next = curr->next;  
        free(curr);   // 释放节点内存  
        curr = next;  
    }  
    free(curr);
    *head = NULL;   // 将头节点指针设置为NULL  
}

void printList(Node *head)
{
    printf("%s\n", head->path);
    Node *curr = head->next;
    while (curr != head)
    {
        printf("%s\n", curr->path);
        curr = curr->next;
    }
    
}