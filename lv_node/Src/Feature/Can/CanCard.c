/*
 * @Description: 
 * @Author: chen
 * @Date: 2023-11-30 17:31:52
 * @LastEditors: chen
 * @LastEditTime: 2024-02-29 17:30:18
 */
#include "ui_app.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>


#define CAN_DEVICE "can0"    // CAN设备名称
#define CAN_ID1 0x123        // 可接收的CAN ID
#define CAN_ID2 0x111        // 可接收的CAN ID

static pthread_t can_port_thread;
static lv_obj_t *can_text_area = 0;
static char msg_buffer[MSG_BUFFER_SIZE] = "";
static uint32_t can_baud_rate;

static int canInit(uint32_t band_rate)
{
    char buffer[256];
    FILE* pipe = popen("ifconfig can0", "r");
    if (!pipe) {
        perror("popen err");
        return -1;
    }

    if(fgets(buffer, sizeof(buffer), pipe) != NULL) 
    {    
        printf("buffer %s\n", buffer);
        char *need = strstr(buffer, "error fetching interface");
        if(need != NULL){
            pclose(pipe);
            return -1;
        }
       
    }
    else{
        pclose(pipe);
        return -1;
   
    }
 
    pclose(pipe);
    // 初始化CAN设备, 设置波特率
    char cmd[125];
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "ifconfig %s down && canconfig %s bitrate %d ctrlmode triple-sampling \
             on && ifconfig %s up", CAN_DEVICE, CAN_DEVICE, band_rate, CAN_DEVICE);
    system(cmd);
    return 0;
}

static void can_update_text_area(const char *message, uint8_t is_sent)
{
    static char formatted_msg[1024];

    // Format the message with the appropriate icon (sent or received)
    if (is_sent == 1)
    {
        snprintf(formatted_msg, MSG_BUFFER_SIZE, "发送数据: %s\n", message);
    }
    else if (is_sent == 2)
    {
        snprintf(formatted_msg, MSG_BUFFER_SIZE, "接收数据: %s\n", message);
    }
    else
    {
        snprintf(formatted_msg, MSG_BUFFER_SIZE, " %s\n", message);
    }

    strncat(msg_buffer, formatted_msg, MSG_BUFFER_SIZE - strlen(msg_buffer) - 1);

    lv_textarea_set_text(can_text_area, msg_buffer);
}

//can发送
static int send_can_msg(int can_id, int can_dlc, const char *data)
{
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame = {0};
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW); // 创建套接字
    strcpy(ifr.ifr_name, CAN_DEVICE);
    ioctl(s, SIOCGIFINDEX, &ifr); // 指定can0 设备
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr)); // 将套接字与can0 绑定
    // 禁用过滤规则，本进程不接收报文，只负责发送
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    // 生成报文
    frame.can_id = can_id;
    frame.can_dlc = can_dlc;
    memcpy(frame.data, data, can_dlc);
    // 发送报文
    nbytes = write(s, &frame, sizeof(frame));
    if (nbytes != sizeof(frame))  {
        perror("Send Error!");
        close(s);
        return -1; // 发送错误
    }
    close(s);
    return 0;
}

// can接收
static int recv_can_msg(int can_id1, int can_id2, int *can_dlc, char **data, int *can_id)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[2];

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW); // 创建套接字
    strcpy(ifr.ifr_name, CAN_DEVICE);
    
    ioctl(s, SIOCGIFINDEX, &ifr); // 指定can0设备

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr)); // 将套接字与can0绑定

    // 定义接收规则，只接收指定的CAN ID报文
    rfilter[0].can_id = can_id1;
    rfilter[0].can_mask = CAN_SFF_MASK;
    rfilter[1].can_id = can_id2;
    rfilter[1].can_mask = CAN_EFF_MASK;

    //  设置过滤规则
     setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    // 读取CAN报文
    if(read(s, &frame, sizeof(frame)) <= 0){
        close(s);
        return -1;
    }

    // 检查读取到的数据长度是否符合预期
    *can_dlc = frame.can_dlc;
    *can_id = frame.can_id;

    // 动态分配缓冲区
    *data = (char *)malloc(*can_dlc * sizeof(char));
    memcpy(*data, frame.data, *can_dlc);
    printf("data %s\n", *data);
    close(s);
    return 0;
}

// 发送按钮点击事件
static void SendButton_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *TextareaVal = lv_event_get_user_data(e);
    if(code == LV_EVENT_CLICKED)
    {
        const char *lvtext = lv_textarea_get_text(TextareaVal); // 获取文本域中的文本
        send_can_msg(0x12, 8, lvtext);
        can_update_text_area(lvtext, 1);
        // printf("can send\n");
    }
    
}
// 按键点击事件
static void btnm_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *BtnTextarea = lv_event_get_user_data(e);
    const char *BtnText = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if (strcmp(BtnText, LV_SYMBOL_BACKSPACE) == 0)
        lv_textarea_del_char(BtnTextarea);
    else
        lv_textarea_add_text(BtnTextarea, BtnText);
}
// 创建按钮矩阵
static void lv_Port_Button(lv_obj_t *parent)
{
    // 创建文本框
    lv_obj_t *ButtonTextarea = lv_textarea_create(parent);
    lv_obj_align(ButtonTextarea, LV_ALIGN_TOP_MID, 210, 100);
    lv_textarea_set_one_line(ButtonTextarea, true);

    // 创建发送按钮
    lv_obj_t *SendButton = btn_create_text(parent, false, "发送", 0, 0);
    lv_obj_align(SendButton, LV_ALIGN_TOP_MID, 410, 105);
    lv_obj_add_event_cb(SendButton, SendButton_Event_Handler, LV_EVENT_ALL, ButtonTextarea);

    // 创建按钮矩阵
    static const char *btnm_map[] = {"1", "2", "3", "\n",
                                     "4", "5", "6", "\n",
                                     "7", "8", "9", "\n",
                                     LV_SYMBOL_BACKSPACE, "0", ""};

    lv_obj_t *btnm = lv_btnmatrix_create(parent);
    lv_obj_set_size(btnm, 420, 300);
    lv_obj_align(btnm, LV_ALIGN_TOP_MID, 270, 170);
    lv_obj_set_style_bg_opa(btnm, LV_OPA_0, 0);
    lv_obj_set_style_border_width(btnm, 0, LV_PART_MAIN); // 设置边框宽度
    lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ButtonTextarea);
    lv_btnmatrix_set_map(btnm, btnm_map);
}

// can接收线程
static void *recv_task(void *arg)
{   
    while (1)
    {
        char *data_recv = NULL;
        int can_dlc = 0, can_id = 0;
        int ret_recv = recv_can_msg(CAN_ID2, CAN_ID1, &can_dlc, &data_recv, &can_id);
        if (ret_recv == -1)  {
            printf("recv Error!\n");
            return ;
        }
        else{
            can_update_text_area(data_recv, 2);
            free(data_recv);  
        }
        
    }
    
    return NULL;
}
// 重新设置CAN波特率
static void CAN_Type_event(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
   

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // 先关掉can接收线程
        pthread_cancel(can_port_thread);
        pthread_join(can_port_thread, NULL);

        char buf_selected[32] = {0};
        lv_dropdown_get_selected_str(obj, buf_selected, sizeof(buf_selected));
        can_baud_rate = atoi(buf_selected);
        int ret = canInit(can_baud_rate);
        printf("can_baud_rate %d\n", can_baud_rate);

        pthread_create(&can_port_thread, NULL, recv_task, NULL);
    }


}

// 创建下拉列表
static lv_obj_t *create_drop_down_list(lv_obj_t *parent, const char *label_text, int16_t x, int16_t y, const char *options)
{
    create_label(parent, label_text, x + 380, y - 280);
    lv_obj_t *dropdown = lv_Port_Type(parent, options, x, y); // Change the last argument of lv_Port_Type to NULL since we set the event later
    lv_obj_add_event_cb(dropdown, CAN_Type_event, LV_EVENT_VALUE_CHANGED, NULL);
    return parent;
}





static void del_event_cb(lv_event_t *e)
{
    // 结束接收线程
    if(can_port_thread != 0){
        pthread_cancel(can_port_thread);
        pthread_join(can_port_thread, NULL);
        can_port_thread = 0;
    }
    
}

void CanCard(void)
{
    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "CAN通讯");
    lv_obj_add_event_cb(parent, del_event_cb, LV_EVENT_DELETE, NULL);
    
    can_text_area = lv_textarea_create(parent);
    lv_obj_align(can_text_area, LV_ALIGN_TOP_MID, -220, 100); // 设置位置
    lv_obj_set_style_text_font(can_text_area, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_size(can_text_area, 491, 257);
    lv_obj_set_style_width(can_text_area, 0, LV_PART_SCROLLBAR);
    lv_textarea_set_text(can_text_area, msg_buffer);

    can_baud_rate = 100000;
    int ret = canInit(can_baud_rate);
    if(ret == -1)
    {
        can_update_text_area("can初始化失败, 请检查设备节点", 3);
        return;
    }
    lv_Port_Button(parent);        // 创建按钮矩阵
    create_drop_down_list(parent, "波特率", -330, 390, CAN_PORT_BAUDRATE);
    

    pthread_create(&can_port_thread, NULL, recv_task, NULL);
}