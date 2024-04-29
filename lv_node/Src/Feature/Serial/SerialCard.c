/*
 * @Description:
 * @Author: lzs
 * @LastEditors: chen
 * @Date: 2023-03-09 11:28:06
 * @LastEditTime: 2023-12-12 16:57:44
 * @FilePath: /new_yz_demo/lv_node/Src/Feature/SerialCard.c
 */

#include "ui_app.h"

static pthread_t read_port_thread;
static uint32_t active_index_1 = 0;
static lv_Port_data_t Port_Data;
static lv_obj_t *Button_label;
static lv_obj_t *text_area;
static bool is_open = false;

static char msg_buffer[MSG_BUFFER_SIZE] = "";

void update_text_area(const char *message, uint8_t is_sent)
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

    lv_textarea_set_text(text_area, msg_buffer);
}

static void Port_Type_event(lv_event_t *e)
{

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        PortType portType = (PortType)lv_event_get_user_data(e);

        char buf_selected[32];
        lv_dropdown_get_selected_str(obj, buf_selected, sizeof(buf_selected));
        

        switch (portType)
        {
        case LV_PORT_BAUDRATE:
            Port_Data.baud_rate = atoi(buf_selected);
            break;
        case LV_PORT_CHECKBIT:
            Port_Data.parity = atoi(buf_selected);
            break;
        case LV_PORT_DATABIT:
            Port_Data.data_width = atoi(buf_selected);
            break;
        case LV_PORT_STOPBIT:
            Port_Data.nstop = atoi(buf_selected);
            break;
        default:
            break;
        }
    }
}

static lv_obj_t *create_dropdown_with_event(lv_obj_t *parent, const char *label_text, int16_t x, int16_t y, const char *options, PortType portType)
{
    create_label(parent, label_text, x + 380, y - 280);
    lv_obj_t *dropdown = lv_Port_Type(parent, options, x, y); // Change the last argument of lv_Port_Type to NULL since we set the event later
    lv_obj_add_event_cb(dropdown, Port_Type_event, LV_EVENT_ALL, NULL);
    lv_obj_set_user_data(dropdown, (void *)portType); // Cast portType to void *
    return parent;
}

// 串口发送按钮点击事件
static void SendButton_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *TextareaVal = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        char *text = lv_label_get_text(Button_label);
        if (strcmp(text, "关闭串口") == 0)
        {
            const char *lvtext = lv_textarea_get_text(TextareaVal); // 获取文本域中的文本
            if (strcmp(lvtext, "") == 0)
            {
                update_text_area("发送数据为空 !", 3);
            }
            else
            {
                serial_send(lvtext);
                update_text_area(lvtext, 1);
            }
        }
        else
        {
            printf("请先打开串口\n");
            update_text_area("请先打开串口 !", 3);
        }
    }
}

// 创建一个发送按钮
static lv_obj_t *Create_Send_Button(lv_obj_t *parent)
{
    lv_obj_t *Button = lv_btn_create(parent);
    lv_obj_set_size(Button, 90, 31);
    lv_obj_align(Button, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_radius(Button, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(Button, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(Button, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(Button, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_t *Buttonlabel = lv_label_create(Button);
    lv_label_set_text(Buttonlabel, "发送");
    lv_obj_set_style_text_color(Buttonlabel, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Buttonlabel, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(Buttonlabel, LV_ALIGN_CENTER, 0, 0);

    return Button;
}

// 点击打开按钮事件
static void OpenButton_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED)
    {
        char *text = lv_label_get_text(obj);

        if (strcmp(text, "打开串口") == 0)
        {
            if (Port_Data.baud_rate == 0)
            {
                Port_Data.baud_rate = 9600;
            }
            if (Port_Data.data_width == 0)
            {
                Port_Data.data_width = 8;
            }
            if (Port_Data.nstop == 0)
            {
                Port_Data.nstop = 1;
            }
            // serial_uinit();
            serial_init(Port_Data.port, Port_Data.baud_rate, Port_Data.data_width, Port_Data.parity, Port_Data.nstop);
            // 创建一个接收线程

            pthread_create(&read_port_thread, NULL, recv_port, NULL);

            lv_label_set_text(obj, "关闭串口");

            is_open = true;
        }
        else
        {
            // 在关闭串口时等待接收线程结束
            pthread_cancel(read_port_thread);
            pthread_join(read_port_thread, NULL);

            serial_uinit();
            lv_label_set_text(obj, "打开串口");
            is_open = false;
        }
    }
}

// 创建一个打开串口按钮
static lv_obj_t *Create_Open_Button(lv_obj_t *parent)
{
    lv_obj_t *OpenButton = lv_btn_create(parent);
    lv_obj_set_size(OpenButton, 90, 31);
    lv_obj_align(OpenButton, LV_ALIGN_TOP_LEFT, 255, 522);
    lv_obj_set_style_radius(OpenButton, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(OpenButton, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(OpenButton, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(OpenButton, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);

    Button_label = lv_label_create(OpenButton);
    if (is_open == true)
    {
        lv_label_set_text(Button_label, "关闭串口");
    }
    else
    {
        lv_label_set_text(Button_label, "打开串口");
    }
    lv_obj_set_style_text_color(Button_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Button_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(Button_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_event_cb(OpenButton, OpenButton_Event_Handler, LV_EVENT_ALL, Button_label);

    return OpenButton;
}

static void cleanButton_Event_Handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        memset(msg_buffer, 0, sizeof(msg_buffer));
        lv_textarea_set_text(text_area, ""); // 清空文本域
    }
}

static lv_obj_t *Create_clean_Button(lv_obj_t *parent)
{
    lv_obj_t *OpenButton = lv_btn_create(parent);
    lv_obj_set_size(OpenButton, 90, 31);
    lv_obj_align(OpenButton, LV_ALIGN_TOP_LEFT, 405, 522);
    lv_obj_set_style_radius(OpenButton, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(OpenButton, lv_color_hex(0x14e7f0), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(OpenButton, lv_color_hex(0x2d6af2), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(OpenButton, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);

    lv_obj_t *lv_Button_label = lv_label_create(OpenButton);
    lv_label_set_text(lv_Button_label, "清屏");
    lv_obj_set_style_text_color(lv_Button_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lv_Button_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(lv_Button_label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_event_cb(OpenButton, cleanButton_Event_Handler, LV_EVENT_ALL, lv_Button_label);

    return OpenButton;
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
    lv_obj_t *SendButton = Create_Send_Button(parent);
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

// 单选框选择点击事件
static void radio_event_handler(lv_event_t *e)
{
    uint32_t *active_id = lv_event_get_user_data(e);
    lv_obj_t *cont = lv_event_get_current_target(e);
    lv_obj_t *act_cb = lv_event_get_target(e);
    lv_obj_t *old_cb = lv_obj_get_child(cont, *active_id);

    /*Do nothing if the container was clicked*/
    if (act_cb == cont)
        return;

    lv_obj_clear_state(old_cb, LV_STATE_CHECKED); /*Uncheck the previous radio button*/
    lv_obj_add_state(act_cb, LV_STATE_CHECKED);   /*Uncheck the current radio button*/

    *active_id = lv_obj_get_index(act_cb);
    Port_Data.port = *active_id;
    printf("串口选择类型：%d \n", Port_Data.port);
}

// 创建RS485和RS232的串口选择框
static lv_obj_t *lv_Port_RadioBox(lv_obj_t *parent)
{
    lv_obj_t *cont1 = lv_obj_create(parent); // 创建一个标签
    lv_obj_set_flex_flow(cont1, LV_FLEX_FLOW_ROW);
    lv_obj_set_size(cont1, 250, 70);
    lv_obj_align(cont1, LV_ALIGN_TOP_MID, -350, 510);
    lv_obj_set_style_bg_opa(cont1, LV_OPA_0, 0);
    lv_obj_set_style_border_width(cont1, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(cont1, radio_event_handler, LV_EVENT_CLICKED, &active_index_1);

    // 创建单选框
    lv_radiobutton_create(cont1, "RS485");
    lv_radiobutton_create(cont1, "RS232");
    lv_obj_add_state(lv_obj_get_child(cont1, 0), LV_STATE_CHECKED);
    return cont1;
}

// // 设置添加文字的样式
static lv_obj_t *Port_SetText_Label(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, char *text)
{
    lv_obj_t *Portlabel = lv_label_create(parent);   // 创建标签对象
    lv_obj_align(Portlabel, LV_ALIGN_DEFAULT, x, y); // 设置标签位置
    lv_label_set_text(Portlabel, text);

    return Portlabel;
}

static lv_obj_t *create_drop_down_list(lv_obj_t *parent)
{
    create_dropdown_with_event(parent, "波特率", -330, 390, PORT_BAUDRATE, LV_PORT_BAUDRATE);
    create_dropdown_with_event(parent, "校验位", -330, 460, PORT_CHECKBIT, LV_PORT_CHECKBIT);
    create_dropdown_with_event(parent, "数据位", -100, 390, PORT_DATABIT, LV_PORT_DATABIT);
    create_dropdown_with_event(parent, "停止位", -100, 460, PORT_STOPBIT, LV_PORT_STOPBIT);

    return parent;
}

void SerialCard(void)
{

    lv_obj_t *parent = Page_Background_Popup(lv_scr_act(), "串口通讯");
    text_area = lv_textarea_create(parent);
    lv_obj_align(text_area, LV_ALIGN_TOP_MID, -220, 100); // 设置位置
    lv_obj_set_style_text_font(text_area, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_size(text_area, 491, 257);
    lv_obj_set_style_width(text_area, 0, LV_PART_SCROLLBAR);
    lv_textarea_set_text(text_area, msg_buffer);

    create_drop_down_list(parent); // 创建下拉列表
    lv_Port_Button(parent);        // 创建按钮矩阵
    lv_Port_RadioBox(parent);      // 创建单选框
    Create_Open_Button(parent);    // 打开
    Create_clean_Button(parent);   // 清屏
}
