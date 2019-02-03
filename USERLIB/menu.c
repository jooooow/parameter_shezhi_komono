#include "menu.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "oled.h"

static void CleanMenu(Oled_t* oled);
static short GetDigit(short value);
static short GetNodeDigit(MenuNode_t* node);
static float GetNodeWeightByPos(MenuNode_t* node, u8 pos);
static void NodeValueAdd(MenuNode_t* node, float value);
static float GetWeightByPos(float value, unsigned char pos);

MenuNode_t control;
MenuNode_t dp_para;
MenuNode_t yt_para;
MenuNode_t bl_para;
MenuNode_t gs_para;
MenuNode_t ny_para;
MenuNode_t sample1;
MenuNode_t sample2;
MenuNode_t sample3;
MenuNode_t sample4;
MenuNode_t sample5;
MenuNode_t config;

MenuNode_t control_mode;
MenuNode_t control_reset;

MenuNode_t dp_pid;
MenuNode_t dp_pid_p;
MenuNode_t dp_pid_i;
MenuNode_t dp_pid_d;
MenuNode_t dp_maxch;
MenuNode_t dp_scale;


MenuNode_t yt_pitch;
MenuNode_t yt_pitch_pid;
MenuNode_t yt_pitch_pid_p;
MenuNode_t yt_pitch_pid_i;
MenuNode_t yt_pitch_pid_d;
MenuNode_t yt_pitch_fdbktype;
MenuNode_t yt_pitch_enable;

MenuNode_t yt_yaw;
MenuNode_t yt_yaw_pid;
MenuNode_t yt_yaw_pid_p;
MenuNode_t yt_yaw_pid_i;
MenuNode_t yt_yaw_pid_d;
MenuNode_t yt_yaw_fdbktype;
MenuNode_t yt_yaw_enable;

MenuNode_t bl_pid;
MenuNode_t bl_pid_p;
MenuNode_t bl_pid_i;
MenuNode_t bl_pid_d;
MenuNode_t bl_blocktime;
MenuNode_t bl_reversetime;
MenuNode_t bl_speed;
MenuNode_t bl_sample1;
MenuNode_t bl_sample2;
MenuNode_t bl_sample3;
MenuNode_t bl_sample3_smp;

MenuNode_t gs_pid;
MenuNode_t gs_pid_p;
MenuNode_t gs_pid_i;
MenuNode_t gs_pid_d;
MenuNode_t gs_maxch;
MenuNode_t gs_enable;

MenuNode_t ny_pid;
MenuNode_t ny_pid_p;
MenuNode_t ny_pid_i;
MenuNode_t ny_pid_d;
MenuNode_t ny_maxch;
MenuNode_t ny_scale;
MenuNode_t ny_enable;

MenuNode_t config_waitack;

void InitMenu(void)
{
	/*                          name        value     val_type   authority      next                prev                child            parent*/
	SetNode(&control,           "Control",  0.0f,     FLOAT,     READONLY,      &dp_para,           NULL,               &control_mode,   NULL);
	SetNode(&dp_para,           "chassis",  0.0f,     FLOAT,     READONLY,      &yt_para,           &control,           &dp_pid,         NULL);
	SetNode(&yt_para,           "gimbal",   0.0f,     FLOAT,     READONLY,      &bl_para,           &dp_para,           &yt_pitch,       NULL);
	SetNode(&bl_para,           "wave",     0.0f,     FLOAT,     READONLY,      &gs_para,           &yt_para,           &bl_pid,         NULL);
	SetNode(&gs_para,           "follow",   0.0f,     FLOAT,     READONLY,      &ny_para,           &bl_para,           &gs_pid,         NULL);
	SetNode(&ny_para,           "newyork",  0.0f,     FLOAT,     READONLY,      &sample1,           &gs_para,           &ny_pid,         NULL);
	SetNode(&sample1,           "sample1",  0.0f,     FLOAT,     READWRITE,     &sample2,           &ny_para,           NULL,            NULL);
	SetNode(&sample2,           "sample2",  0.0f,     FLOAT,     READWRITE,     &sample3,           &sample1,           NULL,            NULL);
	SetNode(&sample3,           "sample3",  0.0f,     FLOAT,     READWRITE,     &sample4,           &sample2,           NULL,            NULL);
	SetNode(&sample4,           "sample4",  0.0f,     FLOAT,     READWRITE,     &sample5,           &sample3,           NULL,            NULL);
	SetNode(&sample5,           "sample5",  0.0f,     FLOAT,     READWRITE,     &config,            &sample4,           NULL,            NULL);
	SetNode(&config,            "config",   0,        USHORT,    READONLY,      NULL,               &sample5,           &config_waitack, NULL);
	
	SetNode(&control_mode,       "mode",     1,       USHORT,    READWRITE,     &control_reset,     NULL,               NULL,            &control);
	SetNode(&control_reset,      "reset",    0,       USHORT,    READWRITE,     NULL,               &control_mode,      NULL,            &control);
	                                                                                                               
	SetNode(&dp_pid,            "pid",      0.0f,     FLOAT,     READONLY,      &dp_maxch,          NULL,               &dp_pid_p,       &dp_para);
  SetNode(&dp_pid_p,          "p",        9.0f,     FLOAT,     READWRITE,     &dp_pid_i,          NULL,               NULL,            &dp_pid);
	SetNode(&dp_pid_i,          "i",        0.4f,     FLOAT,     READWRITE,     &dp_pid_d,          &dp_pid_p,          NULL,            &dp_pid);
	SetNode(&dp_pid_d,          "d",        0.0f,     FLOAT,     READWRITE,     NULL,               &dp_pid_i,          NULL,            &dp_pid);
	SetNode(&dp_maxch,          "maxch",    660,      SHORT,     READWRITE,     &dp_scale,          &dp_pid,            NULL,            &dp_para);
	SetNode(&dp_scale,          "scale",    6.0f,     FLOAT,     READWRITE,     NULL,               &dp_maxch,          NULL,            &dp_para);
	                                                                                                                        
	SetNode(&yt_pitch,          "pitch",    0.0f,     FLOAT,     READONLY,      &yt_yaw,            NULL,               &yt_pitch_pid,   &yt_para);
	SetNode(&yt_pitch_pid,      "pid",      0.0f,     FLOAT,     READONLY,      &yt_pitch_fdbktype, NULL,               &yt_pitch_pid_p, &yt_pitch);
	SetNode(&yt_pitch_pid_p,    "p",        45.0f,    FLOAT,     READWRITE,     &yt_pitch_pid_i,    NULL,               NULL,            &yt_pitch_pid);
	SetNode(&yt_pitch_pid_i,    "i",        0.32f,    FLOAT,     READWRITE,     &yt_pitch_pid_d,    &yt_pitch_pid_p,    NULL,            &yt_pitch_pid);
	SetNode(&yt_pitch_pid_d,    "d",        40.0f,    FLOAT,     READWRITE,     NULL,               &yt_pitch_pid_i,    NULL,            &yt_pitch_pid);
	SetNode(&yt_pitch_fdbktype, "fb_type",  1,        USHORT,    READWRITE,     &yt_pitch_enable,   &yt_pitch_pid,      NULL,            &yt_pitch);
	SetNode(&yt_pitch_enable,   "enable",   1,        USHORT,    READWRITE,     NULL,               &yt_pitch_fdbktype, NULL,            &yt_pitch);
	
	SetNode(&yt_yaw,            "yaw",      0.0f,     FLOAT,     READONLY,      NULL,               &yt_pitch,          &yt_yaw_pid,     &yt_para);
	SetNode(&yt_yaw_pid,        "pid",      0.0f,     FLOAT,     READONLY,      &yt_yaw_fdbktype,   NULL,               &yt_yaw_pid_p,   &yt_yaw);
	SetNode(&yt_yaw_pid_p,      "p",        1900.0f,  FLOAT,     READWRITE,     &yt_yaw_pid_i,      NULL,               NULL,            &yt_yaw_pid);
	SetNode(&yt_yaw_pid_i,      "i",        0.0f,     FLOAT,     READWRITE,     &yt_yaw_pid_d,      &yt_yaw_pid_p,      NULL,            &yt_yaw_pid);
	SetNode(&yt_yaw_pid_d,      "d",        9200.0f,  FLOAT,     READWRITE,     NULL,               &yt_yaw_pid_i,      NULL,            &yt_yaw_pid);
	SetNode(&yt_yaw_fdbktype,   "fb_type",  2,        USHORT,    READWRITE,     &yt_yaw_enable,     &yt_yaw_pid,        NULL,            &yt_yaw);
	SetNode(&yt_yaw_enable,     "enable",   1,        USHORT,    READWRITE,     NULL,               &yt_yaw_fdbktype,   NULL,            &yt_yaw);
                                                    
	SetNode(&bl_pid,            "pid",      0.0f,     FLOAT,     READONLY,      &bl_blocktime,      NULL,               &bl_pid_p,       &bl_para);
	SetNode(&bl_pid_p,          "p",        10.0f,    FLOAT,     READWRITE,     &bl_pid_i,          NULL,               NULL,            &bl_pid);
	SetNode(&bl_pid_i,          "i",        0.05f,    FLOAT,     READWRITE,     &bl_pid_d,          &bl_pid_p,          NULL,            &bl_pid);
	SetNode(&bl_pid_d,          "d",        30.0f,    FLOAT,     READWRITE,     NULL,               &bl_pid_i,          NULL,            &bl_pid);
	SetNode(&bl_blocktime,      "blk_t",    150,      SHORT,     READWRITE,     &bl_reversetime,    &bl_pid,            NULL,            &bl_para);
	SetNode(&bl_reversetime,    "rvs_t",    70,       SHORT,     READWRITE,     &bl_speed,          &bl_blocktime,      NULL,            &bl_para);
  SetNode(&bl_speed,          "speed",    3000,     SHORT,     READWRITE,     &bl_sample1,        &bl_reversetime,    NULL,            &bl_para);
	SetNode(&bl_sample1,        "sample1",  1.0f,     FLOAT,     READWRITE,     &bl_sample2,        &bl_speed,          NULL,            &bl_para);
	SetNode(&bl_sample2,        "sample2",  2.0f,     FLOAT,     READWRITE,     &bl_sample3,        &bl_sample1,        NULL,            &bl_para);
	SetNode(&bl_sample3,        "sample3",  3.0f,     FLOAT,     READONLY,      NULL,               &bl_sample2,        &bl_sample3_smp, &bl_para);
	SetNode(&bl_sample3_smp,    "sample",   31,       SHORT,     READWRITE,     NULL,               NULL,               NULL,            &bl_sample3);

	SetNode(&gs_pid,            "pid",      0.0f,     FLOAT,     READONLY,      &gs_maxch,          NULL,               &gs_pid_p,       &gs_para);
	SetNode(&gs_pid_p,          "p",        0.9f,     FLOAT,     READWRITE,     &gs_pid_i,          NULL,               NULL,            &gs_pid);
	SetNode(&gs_pid_i,          "i",        0.001f,   FLOAT,     READWRITE,     &gs_pid_d,          &gs_pid_p,          NULL,            &gs_pid);
	SetNode(&gs_pid_d,          "d",        0.0f,     FLOAT,     READWRITE,     NULL,               &gs_pid_i,          NULL,            &gs_pid);
	SetNode(&gs_maxch,          "maxch",    750,      SHORT,     READWRITE,     &gs_enable,         &gs_pid,            NULL,            &gs_para);
	SetNode(&gs_enable,         "enable",   1,        USHORT,    READWRITE,     NULL,               &gs_maxch,          NULL,            &gs_para);

	SetNode(&ny_pid,            "pid",      0.0f,     FLOAT,     READONLY,      &ny_maxch,          NULL,               &ny_pid_p,       &ny_para);
	SetNode(&ny_pid_p,          "p",        4.0f,     FLOAT,     READWRITE,     &ny_pid_i,          NULL,               NULL,            &ny_pid);
	SetNode(&ny_pid_i,          "i",        0.001f,   FLOAT,     READWRITE,     &ny_pid_d,          &ny_pid_p,          NULL,            &ny_pid);
	SetNode(&ny_pid_d,          "d",        0.0f,     FLOAT,     READWRITE,     NULL,               &ny_pid_i,          NULL,            &ny_pid);
	SetNode(&ny_maxch,          "maxch",    550,      SHORT,     READWRITE,     &ny_scale,          &ny_pid,            NULL,            &ny_para);
	SetNode(&ny_scale,          "scale",    6.0f,     FLOAT,     READWRITE,     &ny_enable,         &ny_maxch,          NULL,            &ny_para);
  SetNode(&ny_enable,         "enable",   1,        USHORT,    READWRITE,     NULL,               &ny_scale,          NULL,            &ny_para);

	SetNode(&config_waitack,    "waitack",  1,        USHORT,    READWRITE,     NULL,               NULL,               NULL,            &config);
}

void SetNode(MenuNode_t* self, char n[7], float val, u8 value_type, u8 au, MenuNode_t* next, MenuNode_t* prev, MenuNode_t* child, MenuNode_t* parent)
{
	memcpy(self->name,n,7);
	if(value_type == FLOAT)
		self->value.f = val;
	else if(value_type == SHORT)
		self->value.s = (short)val;
	else if(value_type == USHORT)
		self->value.us = (u8)val;
	self->value_type = value_type;
	self->authority = au;
	self->next = next;
	self->prev = prev;
	self->child = child;
	self->parent = parent;
}

extern u8 k_sta[5];
extern u8 old_k_sta[5];
u8 full_path_name[64];

extern Oled_t oled;
extern Oled_t oled_left;
extern Oled_t oled_right;
extern Oled_t oled_arrow;
short cur_row = 0;
char window_row_max = 0;

u8 into_value_set_flag = 0;

void Move(Menu_t* self)
{
	if(UP_PUSH)
	{
		if(self->cur_node->prev != NULL)
		{
			if(!into_value_set_flag)
				MenuUp(self);
		}
		if(into_value_set_flag)
		{
			ValueSet(self);
		}
	}
	else if(DOWN_PUSH)
	{
		if(self->cur_node->next != NULL)
		{
			if(!into_value_set_flag)
				MenuDown(self);
		}
		if(into_value_set_flag)
		{
			ValueSet(self);
		}
	}
	else if(LEFT_PUSH)
	{
		if(self->cur_node->parent != NULL)
		{
			if(!into_value_set_flag)
				MenuLeft(self);
		}
		if(into_value_set_flag)
		{
			ValueSet(self);
		}
	}
	else if(RIGHT_PUSH)
	{
		if(self->cur_node->child != NULL)
		{
			if(!into_value_set_flag)
				MenuRight(self);
		}
		else
		{
			into_value_set_flag = 1;
		}
		
		if(into_value_set_flag)
		{
			ValueSet(self);
		}
	}
	
	if(KAKUNIN_PUSH)
	{
		if(self->cur_node->authority == READWRITE)
		{
			MenuKakunin(self);
		}
	}
}


void MenuUp(Menu_t* self)
{
	printf("front %d,%d\r\n",cur_row,window_row_max);
	self->cur_node = self->cur_node->prev;
	
	CleanMenu(&oled_right);
	DisplayChildren(self->cur_node);
	printf("into %s\r\n",self->cur_node->name);
	
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,' ');
	cur_row--;
	
	if(window_row_max - cur_row > 6)
		oled_left.PushString2BufAndDisplay(&oled_left,self->cur_node->name,FRONT_PUSH);
	
	if(cur_row < 0)
		cur_row = 0;
	
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,'>');
}

void MenuDown(Menu_t* self)
{
	self->cur_node = self->cur_node->next;
	
	CleanMenu(&oled_right);
	DisplayChildren(self->cur_node);
	printf("into %s\r\n",self->cur_node->name);
	
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,' ');
	
	cur_row++;
	if(cur_row > window_row_max)
		window_row_max++;
	
	if(cur_row > 5)
	{
		cur_row = 5;
		
		oled_left.PushString2BufAndDisplay(&oled_left,self->cur_node->name,BACK_PUSH);
	}
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,'>');
}

void MenuLeft(Menu_t* self)
{
	self->cur_node = self->cur_node->parent;
			
	CleanMenu(&oled_left);
	CleanMenu(&oled_right);
	DisplayNext(self->cur_node);
	DisplayChildren(self->cur_node);
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,' ');
	cur_row = 0;
	window_row_max = 6;
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,'>');
	
	DisplayDiv();
	
	printf("into %s\r\n",self->cur_node->name);
}

void MenuRight(Menu_t* self)
{
	self->cur_node = self->cur_node->child;
			
	CleanMenu(&oled_left);
	CleanMenu(&oled_right);
	DisplayNext(self->cur_node);
	DisplayChildren(self->cur_node);
	
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,' ');
	cur_row = 0;
	window_row_max = 0;
	oled_arrow.DisplayChar(&oled_arrow,0,cur_row,'>');
	
	DisplayDiv();
	
	printf("into %s\r\n",self->cur_node->name);
}

u8 wait_host_kakunin_flag = 0;

void MenuKakunin(Menu_t* self)
{
	u8 temp[16];
	
	memset(full_path_name,'\0',64);
	GetFullPathAndValue(self->cur_node);
	
	if(self->cur_node->value_type == FLOAT)
	{
		printf("\r\n@SET %s%.4f\r\n",full_path_name,self->cur_node->value.f);
	}
	else if(self->cur_node->value_type == SHORT)
	{
		printf("\r\n@SET %s%d\r\n",full_path_name,self->cur_node->value.s);
	}
	else if(self->cur_node->value_type == USHORT)
	{
		printf("\r\n@SET %s%d\r\n",full_path_name,self->cur_node->value.us);
	}
	if(config_waitack.value.us == 1)
	{
		wait_host_kakunin_flag++;
		oled.DisplayString(&oled,0,6,"wait ack..");
	}
}

void ValueSet(Menu_t* node)
{
	static u8 pos = 0;
	u8 cur_pos_num = '0';
	u8 value[7] = {'\0'};
	float weight = 0.0f;
	u8 digit = 0;
	u8 temp_pos = 0;
	
	if(node->cur_node->value_type == FLOAT)
		sprintf(value,"%.4f",node->cur_node->value.f);
	else if(node->cur_node->value_type == SHORT)
		sprintf(value,"%d",node->cur_node->value.s);
	else if(node->cur_node->value_type == USHORT)
		sprintf(value,"%d",node->cur_node->value.us);
	
	if(UP_PUSH)
	{
		temp_pos = pos;
		
		digit = GetNodeDigit(node->cur_node);
		if(pos > digit + 1)
			temp_pos = pos - 1;
		
		weight = GetNodeWeightByPos(node->cur_node, temp_pos - 1);
		cur_pos_num = value[pos - 1] - '0';
		printf("%d\r\n",cur_pos_num);
		printf("weight : %.4f\r\n",weight);
		if(cur_pos_num + 1 <= 9)
		{
			NodeValueAdd(node->cur_node, weight);
			
			if(node->cur_node->value_type == FLOAT)
				sprintf(value,"%.4f",node->cur_node->value.f);
			else if(node->cur_node->value_type == SHORT)
				sprintf(value,"%d",node->cur_node->value.s);
			else if(node->cur_node->value_type == USHORT)
				sprintf(value,"%d",node->cur_node->value.us);
			OledDisplayStringWithMask(&oled_right, oled_right.pos_col, 0, value, pos - 1);
		}
	}
	else if(DOWN_PUSH)
	{
		temp_pos = pos;
		
		digit = GetNodeDigit(node->cur_node);
		if(pos > digit + 1)
			temp_pos = pos - 1;
		
		weight = GetNodeWeightByPos(node->cur_node, temp_pos - 1);
		
		cur_pos_num = value[pos - 1] - '0';
		printf("%d\r\n",cur_pos_num);
		printf("weight : %.4f\r\n",weight);
		if(cur_pos_num - 1 >= 0)
		{
			printf("cur_pos_num - 1 >= 0\r\n");
			NodeValueAdd(node->cur_node, -weight);
			
			if(node->cur_node->value_type == FLOAT)
				sprintf(value,"%.4f",node->cur_node->value.f);
			else if(node->cur_node->value_type == SHORT)
				sprintf(value,"%d",node->cur_node->value.s);
			else if(node->cur_node->value_type == USHORT)
				sprintf(value,"%d",node->cur_node->value.us);
			OledDisplayStringWithMask(&oled_right, oled_right.pos_col, 0, value, pos - 1);
		}
	}
	else if(LEFT_PUSH)
	{
		pos--;
		
		digit = GetNodeDigit(node->cur_node);
		if(pos == digit + 1 && node->cur_node->value_type == FLOAT)
			pos--;
		
		if(pos == 0)
		{
			into_value_set_flag = 0;
			pos = 0;
			OledDisplayStringWithMask(&oled_right, oled_right.pos_col, 0, value, 8);
			return;
		}
		OledDisplayStringWithMask(&oled_right, oled_right.pos_col, 0, value, pos - 1);
	}
	else if(RIGHT_PUSH)
	{
		pos++;
		
		digit = GetNodeDigit(node->cur_node);
		if(pos == digit + 1 && node->cur_node->value_type == FLOAT)
			pos++;
		
		if(pos > 7)
			pos = 7;
		if(pos > strlen(value))
			pos = strlen(value);
		OledDisplayStringWithMask(&oled_right, oled_right.pos_col, 0, value, pos - 1);
	}

}


void DisplayChildren(MenuNode_t* node)
{
	MenuNode_t* temp = node->child;
	u8 cnt = 0;
	u8 value[8];
	
	if(temp != NULL)
	{
		while(temp != NULL)
		{
			oled_right.PushString2BufAndDisplay(&oled_right,temp->name,BACK_PUSH);
			cnt++;
			if(cnt >= 6)
				break;
			temp = temp->next;
		}
	}
	else if(temp == NULL && node->authority == READWRITE)
	{
		if(node->value_type == FLOAT)
		{
			sprintf(value,"%.4f",node->value.f);
		}
		else if(node->value_type == SHORT)
		{
			sprintf(value,"%d",node->value.s);
		}
		else if(node->value_type == USHORT)
		{
			sprintf(value,"%d",node->value.us);
		}
		oled_right.PushString2BufAndDisplay(&oled_right,value,BACK_PUSH);
	}
}



void GetFullPathAndValue(MenuNode_t* node)
{
	MenuNode_t* temp = node;
	u8 full_path_name_cnt = 0;
	u8 name_len = 0;
	u8 i;
	
	while(temp != NULL)
	{
		name_len = strlen(temp->name);
		
		for(i = full_path_name_cnt + name_len; i > name_len; i--)
		{
			full_path_name[i] = full_path_name[i - name_len - 1];
		}
		
		memcpy(full_path_name,temp->name,name_len);
		memset(full_path_name+name_len,' ',1);
		full_path_name_cnt += name_len + 1;
		temp = temp->parent;
	}
}

static void CleanMenu(Oled_t* oled)
{
	u8 i,j;
	memset(oled->buf,' ',7*16);
	oled->row_cnt = 0;
	for(i = 0; i < oled->max_row_cnt; i++)
	{
		for(j = 0; j < oled->max_col_cnt * 8; j++)
		{
			oled->DisplayChar(oled, oled->pos_col + j, i, ' ');
		}
	}
}

void DisplayNext(MenuNode_t* node)
{
	u8 cnt = 0;
	MenuNode_t* temp = node;
	while(temp != NULL)
	{
		oled_left.PushString2BufAndDisplay(&oled_left,temp->name, BACK_PUSH);
		cnt++;
		if(cnt >= 6)
			break;
		temp = temp->next;
	}
}

void DisplayDiv(void)
{
	u8 i;
	for(i = 0; i < 6; i++)
	{
		OledDisplayChar(NULL,64,i,'|');
	}
}

void DisplaySolider(void)
{
	static u8 col = 0;
	
	if(col < 120)
	{
		OledDisplayChar(NULL,col,6,0XFF);
		col+=8;
		if(col == 120)
		{
			printf("reset done!\r\n");
			col = 121;
			OledDisplayString(&oled,0,6,"                ");
		}
	}
}

static float GetWeightByPos(float value, unsigned char pos)
{
	short sign = value >= 0 ? 1 : -1;
	short int_value = (short)value;
	short digit = GetDigit(int_value);
	float weight = 0;
	
	if(pos < digit)
	{
		weight = pow(10,(digit - pos - 1));
	}
	else if(pos >= digit)
	{
		weight = pow(0.1,(pos - digit + 1));
	}
	
	return weight * sign;
}

static short GetDigit(short value)
{
	if(value / 10 == 0)    return 1;
	if(value / 100 == 0)   return 2;
	if(value / 1000 == 0)  return 3;
	if(value / 10000 == 0) return 4;
} 

static short GetNodeDigit(MenuNode_t* node)
{
	u8 digit = 0;
	
	if(node->value_type == FLOAT)
		digit = GetDigit((short)node->value.f);
	else if(node->value_type == SHORT)
		digit = GetDigit(node->value.s);
	else if(node->value_type == USHORT)
		digit = GetDigit(node->value.us);
	
	return digit;
}

static float GetNodeWeightByPos(MenuNode_t* node, u8 pos)
{
	float weight = 0.0f;
	
	if(node->value_type == FLOAT)
		weight = GetWeightByPos(node->value.f, pos);
	else if(node->value_type == SHORT)
		weight = GetWeightByPos((float)node->value.s, pos);
	else if(node->value_type == USHORT)
		weight = GetWeightByPos((float)node->value.us, pos);
	
	return weight;
}

static void NodeValueAdd(MenuNode_t* node, float value)
{
	if(node->value_type == FLOAT)
	{
		node->value.f += value;
	}
	else if(node->value_type == SHORT)
	{
		node->value.s += (short)value;
	}
	else if(node->value_type == USHORT)
	{
		node->value.us += value;
	}
}