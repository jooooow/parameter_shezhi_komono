#include "menu2.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define READONLY  MenuNode_t::READONLY
#define READWRITE MenuNode_t::READWRITE
#define UCHAR     MenuNodeValue_t::UCHAR
#define USHORT    MenuNodeValue_t::USHORT
#define DOUBLE     MenuNodeValue_t::DOUBLE

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

extern Oled* oled_ptr;
static short GetNodeValueFullDigit(MenuNode_t* node);
static short GetNodeValueIntegerDigit(MenuNode_t* node);
static double GetNodeWeightByPos(MenuNode_t* node, u8 pos);
static void SetNode(_MenuNode_t* node,const char* n, _MenuNode_t* _next, _MenuNode_t* _prev,_MenuNode_t* _child,_MenuNode_t* _parent, u8 _authority, double val, u8 type);

Menu::Menu(Oled* o)
{
  /*                          name          next                prev                child            parent          authority   value     val_type*/ 
  SetNode(&control,           "control",    &dp_para,           NULL,               &control_mode,   NULL          , READONLY,   0,        UCHAR );
  SetNode(&dp_para,           "chassis",    &yt_para,           &control,           &dp_pid,         NULL          , READONLY,   0,        UCHAR );
  SetNode(&yt_para,           "gimbal",     &bl_para,           &dp_para,           &yt_pitch,       NULL          , READONLY,   0,        UCHAR );
  SetNode(&bl_para,           "wave",       &gs_para,           &yt_para,           &bl_pid,         NULL          , READONLY,   0,        UCHAR );
  SetNode(&gs_para,           "follow",     &ny_para,           &bl_para,           &gs_pid,         NULL          , READONLY,   0,        UCHAR );
  SetNode(&ny_para,           "newyork",    &sample1,           &gs_para,           &ny_pid,         NULL          , READONLY,   0,        UCHAR );
  SetNode(&sample1,           "sample1",    &sample2,           &ny_para,           NULL,            NULL          , READWRITE,  0.0f,     DOUBLE);
  SetNode(&sample2,           "sample2",    &sample3,           &sample1,           NULL,            NULL          , READWRITE,  0.0f,     DOUBLE);
  SetNode(&sample3,           "sample3",    &sample4,           &sample2,           NULL,            NULL          , READWRITE,  0.0f,     DOUBLE);
  SetNode(&sample4,           "sample4",    &sample5,           &sample3,           NULL,            NULL          , READWRITE,  0.0f,     DOUBLE);
  SetNode(&sample5,           "sample5",    &config,            &sample4,           NULL,            NULL          , READWRITE,  0.0f,     DOUBLE);
  SetNode(&config,            "config",     NULL,               &sample5,           &config_waitack, NULL          , READONLY,   0,        UCHAR );
                                                                                                                                
  SetNode(&control_mode,      "mode",       &control_reset,     NULL,               NULL,            &control      , READWRITE,   1,       USHORT);
  SetNode(&control_reset,     "reset",      NULL,               &control_mode,      NULL,            &control      , READWRITE,   0,       USHORT);
                                                                                                                                                 
  SetNode(&dp_pid,            "pid",        &dp_maxch,          NULL,               &dp_pid_p,       &dp_para      , READONLY,   0,        UCHAR);
  SetNode(&dp_pid_p,          "p",          &dp_pid_i,          NULL,               NULL,            &dp_pid       , READWRITE,  9.0f,     DOUBLE);
  SetNode(&dp_pid_i,          "i",          &dp_pid_d,          &dp_pid_p,          NULL,            &dp_pid       , READWRITE,  0.4f,     DOUBLE);
  SetNode(&dp_pid_d,          "d",          NULL,               &dp_pid_i,          NULL,            &dp_pid       , READWRITE,  0.0f,     DOUBLE);
  SetNode(&dp_maxch,          "maxch",      &dp_scale,          &dp_pid,            NULL,            &dp_para      , READWRITE,  660,      USHORT);
  SetNode(&dp_scale,          "scale",      NULL,               &dp_maxch,          NULL,            &dp_para      , READWRITE,  6.0f,     DOUBLE);
                                                                                                                                                 
  SetNode(&yt_pitch,          "pitch",      &yt_yaw,            NULL,               &yt_pitch_pid,   &yt_para      , READONLY,   0,        UCHAR );
  SetNode(&yt_pitch_pid,      "pid",        &yt_pitch_fdbktype, NULL,               &yt_pitch_pid_p, &yt_pitch     , READONLY,   0,        UCHAR );
  SetNode(&yt_pitch_pid_p,    "p",          &yt_pitch_pid_i,    NULL,               NULL,            &yt_pitch_pid , READWRITE,  45.0f,    DOUBLE);
  SetNode(&yt_pitch_pid_i,    "i",          &yt_pitch_pid_d,    &yt_pitch_pid_p,    NULL,            &yt_pitch_pid , READWRITE,  0.32f,    DOUBLE);
  SetNode(&yt_pitch_pid_d,    "d",          NULL,               &yt_pitch_pid_i,    NULL,            &yt_pitch_pid , READWRITE,  40.0f,    DOUBLE);
  SetNode(&yt_pitch_fdbktype, "fb_type",    &yt_pitch_enable,   &yt_pitch_pid,      NULL,            &yt_pitch     , READWRITE,  1,        USHORT);
  SetNode(&yt_pitch_enable,   "enable",     NULL,               &yt_pitch_fdbktype, NULL,            &yt_pitch     , READWRITE,  1,        USHORT);
                                                                                                                                
  SetNode(&yt_yaw,            "yaw",        NULL,               &yt_pitch,          &yt_yaw_pid,     &yt_para      , READONLY,   0,        UCHAR );
  SetNode(&yt_yaw_pid,        "pid",        &yt_yaw_fdbktype,   NULL,               &yt_yaw_pid_p,   &yt_yaw       , READONLY,   0,        UCHAR );
  SetNode(&yt_yaw_pid_p,      "p",          &yt_yaw_pid_i,      NULL,               NULL,            &yt_yaw_pid   , READWRITE,  1900.0f,  DOUBLE);
  SetNode(&yt_yaw_pid_i,      "i",          &yt_yaw_pid_d,      &yt_yaw_pid_p,      NULL,            &yt_yaw_pid   , READWRITE,  0.0f,     DOUBLE);
  SetNode(&yt_yaw_pid_d,      "d",          NULL,               &yt_yaw_pid_i,      NULL,            &yt_yaw_pid   , READWRITE,  9200.0f,  DOUBLE);
  SetNode(&yt_yaw_fdbktype,   "fb_type",    &yt_yaw_enable,     &yt_yaw_pid,        NULL,            &yt_yaw       , READWRITE,  2,        USHORT);
  SetNode(&yt_yaw_enable,     "enable",     NULL,               &yt_yaw_fdbktype,   NULL,            &yt_yaw       , READWRITE,  1,        USHORT);
                                                                                                                               
  SetNode(&bl_pid,            "pid",        &bl_blocktime,      NULL,               &bl_pid_p,       &bl_para      , READONLY,   0.0f,     DOUBLE);
  SetNode(&bl_pid_p,          "p",          &bl_pid_i,          NULL,               NULL,            &bl_pid       , READWRITE,  10.0f,    DOUBLE);
  SetNode(&bl_pid_i,          "i",          &bl_pid_d,          &bl_pid_p,          NULL,            &bl_pid       , READWRITE,  0.05f,    DOUBLE);
  SetNode(&bl_pid_d,          "d",          NULL,               &bl_pid_i,          NULL,            &bl_pid       , READWRITE,  30.0f,    DOUBLE);
  SetNode(&bl_blocktime,      "blk_t",      &bl_reversetime,    &bl_pid,            NULL,            &bl_para      , READWRITE,  150,      USHORT);
  SetNode(&bl_reversetime,    "rvs_t",      &bl_speed,          &bl_blocktime,      NULL,            &bl_para      , READWRITE,  70,       USHORT);
  SetNode(&bl_speed,          "speed",      &bl_sample1,        &bl_reversetime,    NULL,            &bl_para      , READWRITE,  3000,     USHORT);
  SetNode(&bl_sample1,        "sample1",    &bl_sample2,        &bl_speed,          NULL,            &bl_para      , READWRITE,  1.0f,     DOUBLE);
  SetNode(&bl_sample2,        "sample2",    &bl_sample3,        &bl_sample1,        NULL,            &bl_para      , READWRITE,  2.0f,     DOUBLE);
  SetNode(&bl_sample3,        "sample3",    NULL,               &bl_sample2,        &bl_sample3_smp, &bl_para      , READONLY,   3.0f,     DOUBLE);
  SetNode(&bl_sample3_smp,    "sample",     NULL,               NULL,               NULL,            &bl_sample3   , READWRITE,  31,       USHORT);
                                                                                                                                
  SetNode(&gs_pid,            "pid",        &gs_maxch,          NULL,               &gs_pid_p,       &gs_para      , READONLY,   0.0f,     DOUBLE);
  SetNode(&gs_pid_p,          "p",          &gs_pid_i,          NULL,               NULL,            &gs_pid       , READWRITE,  0.9f,     DOUBLE);
  SetNode(&gs_pid_i,          "i",          &gs_pid_d,          &gs_pid_p,          NULL,            &gs_pid       , READWRITE,  0.001f,   DOUBLE);
  SetNode(&gs_pid_d,          "d",          NULL,               &gs_pid_i,          NULL,            &gs_pid       , READWRITE,  0.0f,     DOUBLE);
  SetNode(&gs_maxch,          "maxch",      &gs_enable,         &gs_pid,            NULL,            &gs_para      , READWRITE,  750,      USHORT);
  SetNode(&gs_enable,         "enable",     NULL,               &gs_maxch,          NULL,            &gs_para      , READWRITE,  1,        USHORT);
                                                                                                                                     
  SetNode(&ny_pid,            "pid",        &ny_maxch,          NULL,               &ny_pid_p,       &ny_para      , READONLY,   0.0f,     DOUBLE);
  SetNode(&ny_pid_p,          "p",          &ny_pid_i,          NULL,               NULL,            &ny_pid       , READWRITE,  4.0f,     DOUBLE);
  SetNode(&ny_pid_i,          "i",          &ny_pid_d,          &ny_pid_p,          NULL,            &ny_pid       , READWRITE,  0.001f,   DOUBLE);
  SetNode(&ny_pid_d,          "d",          NULL,               &ny_pid_i,          NULL,            &ny_pid       , READWRITE,  0.0f,     DOUBLE);
  SetNode(&ny_maxch,          "maxch",      &ny_scale,          &ny_pid,            NULL,            &ny_para      , READWRITE,  550,      USHORT);
  SetNode(&ny_scale,          "scale",      &ny_enable,         &ny_maxch,          NULL,            &ny_para      , READWRITE,  6.0f,     DOUBLE);
  SetNode(&ny_enable,         "enable",     NULL,               &ny_scale,          NULL,            &ny_para      , READWRITE,  1,        USHORT);
                                                                                                                    
  SetNode(&config_waitack,    "waitack",    NULL,               NULL,               NULL,            &config       , READWRITE,  1,        USHORT);
  
  cur_node = &control;
  cur_path = "";
  cur_cursor_row = 0; 
	oled = o;
	cur_value_set_pos = -1;
	is_into_value_set = 0;
}

void Menu::Move(u8 pushed_key)
{
	switch(pushed_key)
	{
		case UP:Up();break;
		case DOWN:Down();break;
		case LEFT:Left();break;
		case RIGHT:Right();break;
		case KAKUNIN:Kakunin();break;
		default:break;
	}
}

void Menu::Up()
{
	if(is_into_value_set == 0)
	{
		if(cur_node->prev != NULL)
		{
			cur_node = cur_node->prev;
			if(cur_cursor_row == 0)
			{
				oled_ptr->Push2Left(cur_node->name,0);
				oled->UpdateLeft();
			}
			MoveCursor(cur_cursor_row - 1);
			ShowChildrenOrValue();
			oled->UpdateRight();
		}
	}
	else
	{
		u8 cur_pos_num = oled->layout_right->GetAt(0)->str[cur_value_set_pos] - '0';
		if(cur_pos_num < 9)
		{
			double weight = GetNodeWeightByPos(cur_node,cur_value_set_pos);
			CurNodeValueAdd(weight);
			oled->ClearRight();
			char temp[7];
			sprintf(temp,"%.5f",cur_node->value.f);
			oled->Push2Right(temp,1);
			oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,cur_value_set_pos);
		}
	}
}

void Menu::Down()
{
	if(is_into_value_set == 0)
	{
		if(cur_node->next != NULL)
		{
			cur_node = cur_node->next;	
			if(cur_cursor_row == oled->GetMaxLayoutHeight() - 1)
			{
				oled_ptr->Push2Left(cur_node->name,1);
				oled->UpdateLeft();
			}
			MoveCursor(cur_cursor_row + 1);
			ShowChildrenOrValue();
			oled->UpdateRight();
		}
	}
	else
	{
		u8 cur_pos_num = oled->layout_right->GetAt(0)->str[cur_value_set_pos] - '0';
		if(cur_pos_num > 0)
		{
			double weight = GetNodeWeightByPos(cur_node,cur_value_set_pos);
			CurNodeValueAdd(-weight);
			oled->ClearRight();
			char temp[7];
			sprintf(temp,"%.5f",cur_node->value.f);
			oled->Push2Right(temp,1);
			oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,cur_value_set_pos);
		}
	}
}

void Menu::Right()
{
	if(is_into_value_set == 0)
	{
		if(cur_node->child != NULL)
		{
			cur_node = cur_node->child;
			MoveCursor(0);
			ShowNexts();
			ShowChildrenOrValue();
			oled->Update();
		}
		else
		{
			if(cur_node->authority == READWRITE && is_into_value_set == 0)
				is_into_value_set = 1;
		}
	}
	
	if(is_into_value_set == 1)
	{
		cur_value_set_pos++;
		
		u8 full_digit = GetNodeValueFullDigit(cur_node);
		u8 int_digit = GetNodeValueIntegerDigit(cur_node);
		
		if(cur_value_set_pos == int_digit && cur_node->value.type == DOUBLE)
			cur_value_set_pos++;
		else if(cur_value_set_pos >= full_digit)
			cur_value_set_pos = full_digit - 1;

		oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,cur_value_set_pos);
	}
}

void Menu::Left()
{
	if(is_into_value_set == 0)
	{
		if(cur_node->parent != NULL)
		{
			cur_node = cur_node->parent;
			MoveCursor(0);
			ShowNexts();
			ShowChildrenOrValue();
			oled->Update();
		}
	}
	else
	{
		cur_value_set_pos--;
		if(cur_value_set_pos == -1)
		{
			is_into_value_set = 0;
			oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,-1);
			return;
		}
		
		u8 full_digit = GetNodeValueFullDigit(cur_node);
		u8 int_digit = GetNodeValueIntegerDigit(cur_node);
		
		if(cur_value_set_pos == int_digit && cur_node->value.type == DOUBLE)
		{
			cur_value_set_pos--;
			if(cur_value_set_pos == -1)
			{
				is_into_value_set = 0;
				oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,-1);
				return;
			}
		}
		else if(cur_value_set_pos >= full_digit)
			cur_value_set_pos = full_digit - 1;

		oled->ShowString(72,0,oled->layout_right->GetAt(0)->str,cur_value_set_pos);
	}
}

void Menu::Kakunin()
{
	if(cur_node->authority == READWRITE)
	{
		UpdateCurPath();
		std::string set_str = "";
		std::string path_str = "$SET " + cur_path;
		char value_str[16];
		
		if(cur_node->value.type == UCHAR)
			sprintf(value_str,"%d",cur_node->value.u_8);
		else if(cur_node->value.type == USHORT)
			sprintf(value_str,"%d",cur_node->value.u_16);
		else if(cur_node->value.type == DOUBLE)
			sprintf(value_str,"%.5f",cur_node->value.f);
		
		set_str = path_str + " " + value_str;
		printf("%s\r\n",set_str.data());
	}
}

void Menu::ShowChildrenOrValue()
{
	if(cur_node->authority == READONLY)
			ShowChildren();
	else if(cur_node->authority == READWRITE)
			ShowValue();
}

void Menu::ShowChildren()
{
	oled_ptr->ClearRight();
	MenuNode_t* temp = cur_node->child;
	u8 cnt = 0;
	while(temp != NULL && cnt < oled->GetMaxLayoutHeight())
	{
		oled->Push2Right(temp->name,1);
		temp = temp->next;
		cnt++;
	}
}

void Menu::ShowNexts()
{
	oled_ptr->ClearLeft();
	MenuNode_t* temp = cur_node;
	u8 cnt = 0;
	while(temp != NULL && cnt < oled->GetMaxLayoutHeight())
	{
		oled->Push2Left(temp->name,1);
		temp = temp->next;
		cnt++;
	}
}

void Menu::ShowValue()
{
	oled->ClearRight();
	MenuNode_t* temp = cur_node;
	char str[16] = {'\0'};
	switch(temp->value.type)
	{
		case UCHAR:sprintf(str,"%d",temp->value.u_8);break;
		case USHORT:sprintf(str,"%d",temp->value.u_16);break;
		case DOUBLE:sprintf(str,"%.7f",temp->value.f);break;
		default:break;
	}
	oled->Push2Right(str,1);
}

void Menu::MoveCursor(short row)
{
	oled_ptr->ShowChar(0,cur_cursor_row,' ');
	cur_cursor_row = row;
	if(cur_cursor_row > oled->GetMaxLayoutHeight() - 1)
		cur_cursor_row = oled->GetMaxLayoutHeight() - 1;
	else if(cur_cursor_row < 0)
		cur_cursor_row = 0;
	oled_ptr->ShowChar(0,cur_cursor_row,'>');
}

void Menu::UpdateCurPath()
{
	cur_path = "";
	MenuNode_t* temp = cur_node;
	while(temp != NULL)
	{
		cur_path = "/" + (std::string)temp->name + cur_path;
		temp = temp->parent;
	}
}

void Menu::CurNodeValueAdd(double val)
{
	MenuNode_t* temp = cur_node;
	switch(temp->value.type)
	{
		case UCHAR:temp->value.u_8 += (u8)val;break;
		case USHORT:temp->value.u_16 += (u16)val;break;
		case DOUBLE:temp->value.f += (double)val;break;
		default:break;
	}
}

static void SetNode(_MenuNode_t* node,const char* n, _MenuNode_t* _next, _MenuNode_t* _prev,_MenuNode_t* _child,_MenuNode_t* _parent, u8 _authority, double val, u8 type)
{
	node->Set(n,_next,_prev,_child,_parent,_authority,val,type);
}

static short GetNodeValueFullDigit(MenuNode_t* node)
{
	if(node->value.type == DOUBLE)
		return 7;
	else
		return GetNodeValueIntegerDigit(node);
}

static short GetIntegerDigit(short value)
{
	if(value / 10 == 0)    return 1;
	if(value / 100 == 0)   return 2;
	if(value / 1000 == 0)  return 3;
	if(value / 10000 == 0) return 4;
} 

static short GetNodeValueIntegerDigit(MenuNode_t* node)
{
	u8 digit = 0;
	
	if(node->value.type == DOUBLE)
		digit = GetIntegerDigit((short)node->value.f);
	else if(node->value.type == USHORT)
		digit = GetIntegerDigit(node->value.u_16);
	else if(node->value.type == UCHAR)
		digit = GetIntegerDigit(node->value.u_8);
	
	return digit;
}

static double GetWeightByPos(double value, unsigned char pos)
{
	short sign = value >= 0 ? 1 : -1;
	short int_value = (short)value;
	short digit = GetIntegerDigit(int_value);
	double weight = 0;
	
	if(pos < digit)
	{
		weight = pow((double)10,(int)(digit - pos - 1));
	}
	else if(pos > digit)
	{
		weight = pow((double)0.1,(int)(pos - digit));
	}
	else if(pos == digit)
	{
		weight = 0.0f;
	}
	
	return weight * sign;
}

static double GetNodeWeightByPos(MenuNode_t* node, u8 pos)
{
	double weight = 0.0f;
	
	if(node->value.type == DOUBLE)
		weight = GetWeightByPos(node->value.f, pos);
	else if(node->value.type == USHORT)
		weight = GetWeightByPos((double)node->value.u_16, pos);
	else if(node->value.type == UCHAR)
		weight = GetWeightByPos((double)node->value.u_8, pos);
	
	return weight;
}
