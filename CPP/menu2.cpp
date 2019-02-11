#include "menu2.h"
#include "stdio.h"

#define READONLY  MenuNode_t::READONLY
#define READWRITE MenuNode_t::READWRITE
#define UCHAR     MenuNodeValue_t::UCHAR
#define USHORT    MenuNodeValue_t::USHORT
#define FLOAT     MenuNodeValue_t::FLOAT

#define IN_NODE_CHOOSE  cur_node->authority == READONLY
#define IN_VALUE_SET    cur_node->authority == READWRITE

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

static void SetNode(_MenuNode_t* node,const char* n, _MenuNode_t* _next, _MenuNode_t* _prev,_MenuNode_t* _child,_MenuNode_t* _parent, u8 _authority, float val, u8 type)
{
	node->Set(n,_next,_prev,_child,_parent,_authority,val,type);
}

Menu::Menu()
{
	/*                          name          next                prev                child            parent          authority   value     val_type*/ 
	SetNode(&control,           "control",    &dp_para,           NULL,               &control_mode,   NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&dp_para,           "chassis",    &yt_para,           &control,           &dp_pid,         NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&yt_para,           "gimbal",     &bl_para,           &dp_para,           &yt_pitch,       NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&bl_para,           "wave",       &gs_para,           &yt_para,           &bl_pid,         NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&gs_para,           "follow",     &ny_para,           &bl_para,           &gs_pid,         NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&ny_para,           "newyork",    &sample1,           &gs_para,           &ny_pid,         NULL          , READONLY,   0.0f,     FLOAT);
	SetNode(&sample1,           "sample1",    &sample2,           &ny_para,           NULL,            NULL          , READWRITE,  0.0f,     FLOAT);
	SetNode(&sample2,           "sample2",    &sample3,           &sample1,           NULL,            NULL          , READWRITE,  0.0f,     FLOAT);
	SetNode(&sample3,           "sample3",    &sample4,           &sample2,           NULL,            NULL          , READWRITE,  0.0f,     FLOAT);
	SetNode(&sample4,           "sample4",    &sample5,           &sample3,           NULL,            NULL          , READWRITE,  0.0f,     FLOAT);
	SetNode(&sample5,           "sample5",    &config,            &sample4,           NULL,            NULL          , READWRITE,  0.0f,     FLOAT);
	SetNode(&config,            "config",     NULL,               &sample5,           &config_waitack, NULL          , READONLY,   0,        USHORT);
	                                                                                                                              
	SetNode(&control_mode,      "mode",       &control_reset,     NULL,               NULL,            &control      , READWRITE,   1,       USHORT);
	SetNode(&control_reset,     "reset",      NULL,               &control_mode,      NULL,            &control      , READWRITE,   0,       USHORT);
	                                                                                                                                               
	SetNode(&dp_pid,            "pid",        &dp_maxch,          NULL,               &dp_pid_p,       &dp_para      , READONLY,   0.0f,     FLOAT);
  SetNode(&dp_pid_p,          "p",          &dp_pid_i,          NULL,               NULL,            &dp_pid       , READWRITE,  9.0f,     FLOAT);
	SetNode(&dp_pid_i,          "i",          &dp_pid_d,          &dp_pid_p,          NULL,            &dp_pid       , READWRITE,  0.4f,     FLOAT);
	SetNode(&dp_pid_d,          "d",          NULL,               &dp_pid_i,          NULL,            &dp_pid       , READWRITE,  0.0f,     FLOAT);
	SetNode(&dp_maxch,          "maxch",      &dp_scale,          &dp_pid,            NULL,            &dp_para      , READWRITE,  660,      USHORT);
	SetNode(&dp_scale,          "scale",      NULL,               &dp_maxch,          NULL,            &dp_para      , READWRITE,  6.0f,     FLOAT);
	                                                                                                                                               
	SetNode(&yt_pitch,          "pitch",      &yt_yaw,            NULL,               &yt_pitch_pid,   &yt_para      , READONLY,   0.0f,     FLOAT);
	SetNode(&yt_pitch_pid,      "pid",        &yt_pitch_fdbktype, NULL,               &yt_pitch_pid_p, &yt_pitch     , READONLY,   0.0f,     FLOAT);
	SetNode(&yt_pitch_pid_p,    "p",          &yt_pitch_pid_i,    NULL,               NULL,            &yt_pitch_pid , READWRITE,  45.0f,    FLOAT);
	SetNode(&yt_pitch_pid_i,    "i",          &yt_pitch_pid_d,    &yt_pitch_pid_p,    NULL,            &yt_pitch_pid , READWRITE,  0.32f,    FLOAT);
	SetNode(&yt_pitch_pid_d,    "d",          NULL,               &yt_pitch_pid_i,    NULL,            &yt_pitch_pid , READWRITE,  40.0f,    FLOAT);
	SetNode(&yt_pitch_fdbktype, "fb_type",    &yt_pitch_enable,   &yt_pitch_pid,      NULL,            &yt_pitch     , READWRITE,  1,        USHORT);
	SetNode(&yt_pitch_enable,   "enable",     NULL,               &yt_pitch_fdbktype, NULL,            &yt_pitch     , READWRITE,  1,        USHORT);
	                                                                                                                              
	SetNode(&yt_yaw,            "yaw",        NULL,               &yt_pitch,          &yt_yaw_pid,     &yt_para      , READONLY,   0.0f,     FLOAT);
	SetNode(&yt_yaw_pid,        "pid",        &yt_yaw_fdbktype,   NULL,               &yt_yaw_pid_p,   &yt_yaw       , READONLY,   0.0f,     FLOAT);
	SetNode(&yt_yaw_pid_p,      "p",          &yt_yaw_pid_i,      NULL,               NULL,            &yt_yaw_pid   , READWRITE,  1900.0f,  FLOAT);
	SetNode(&yt_yaw_pid_i,      "i",          &yt_yaw_pid_d,      &yt_yaw_pid_p,      NULL,            &yt_yaw_pid   , READWRITE,  0.0f,     FLOAT);
	SetNode(&yt_yaw_pid_d,      "d",          NULL,               &yt_yaw_pid_i,      NULL,            &yt_yaw_pid   , READWRITE,  9200.0f,  FLOAT);
	SetNode(&yt_yaw_fdbktype,   "fb_type",    &yt_yaw_enable,     &yt_yaw_pid,        NULL,            &yt_yaw       , READWRITE,  2,        USHORT);
	SetNode(&yt_yaw_enable,     "enable",     NULL,               &yt_yaw_fdbktype,   NULL,            &yt_yaw       , READWRITE,  1,        USHORT);
                                                                                                                               
	SetNode(&bl_pid,            "pid",        &bl_blocktime,      NULL,               &bl_pid_p,       &bl_para      , READONLY,   0.0f,     FLOAT);
	SetNode(&bl_pid_p,          "p",          &bl_pid_i,          NULL,               NULL,            &bl_pid       , READWRITE,  10.0f,    FLOAT);
	SetNode(&bl_pid_i,          "i",          &bl_pid_d,          &bl_pid_p,          NULL,            &bl_pid       , READWRITE,  0.05f,    FLOAT);
	SetNode(&bl_pid_d,          "d",          NULL,               &bl_pid_i,          NULL,            &bl_pid       , READWRITE,  30.0f,    FLOAT);
	SetNode(&bl_blocktime,      "blk_t",      &bl_reversetime,    &bl_pid,            NULL,            &bl_para      , READWRITE,  150,      USHORT);
	SetNode(&bl_reversetime,    "rvs_t",      &bl_speed,          &bl_blocktime,      NULL,            &bl_para      , READWRITE,  70,       USHORT);
  SetNode(&bl_speed,          "speed",      &bl_sample1,        &bl_reversetime,    NULL,            &bl_para      , READWRITE,  3000,     USHORT);
	SetNode(&bl_sample1,        "sample1",    &bl_sample2,        &bl_speed,          NULL,            &bl_para      , READWRITE,  1.0f,     FLOAT);
	SetNode(&bl_sample2,        "sample2",    &bl_sample3,        &bl_sample1,        NULL,            &bl_para      , READWRITE,  2.0f,     FLOAT);
	SetNode(&bl_sample3,        "sample3",    NULL,               &bl_sample2,        &bl_sample3_smp, &bl_para      , READONLY,   3.0f,     FLOAT);
	SetNode(&bl_sample3_smp,    "sample",     NULL,               NULL,               NULL,            &bl_sample3   , READWRITE,  31,       USHORT);
                                                                                                                                
	SetNode(&gs_pid,            "pid",        &gs_maxch,          NULL,               &gs_pid_p,       &gs_para      , READONLY,   0.0f,     FLOAT);
	SetNode(&gs_pid_p,          "p",          &gs_pid_i,          NULL,               NULL,            &gs_pid       , READWRITE,  0.9f,     FLOAT);
	SetNode(&gs_pid_i,          "i",          &gs_pid_d,          &gs_pid_p,          NULL,            &gs_pid       , READWRITE,  0.001f,   FLOAT);
	SetNode(&gs_pid_d,          "d",          NULL,               &gs_pid_i,          NULL,            &gs_pid       , READWRITE,  0.0f,     FLOAT);
	SetNode(&gs_maxch,          "maxch",      &gs_enable,         &gs_pid,            NULL,            &gs_para      , READWRITE,  750,      USHORT);
	SetNode(&gs_enable,         "enable",     NULL,               &gs_maxch,          NULL,            &gs_para      , READWRITE,  1,        USHORT);
                                                                                                                                     
	SetNode(&ny_pid,            "pid",        &ny_maxch,          NULL,               &ny_pid_p,       &ny_para      , READONLY,   0.0f,     FLOAT);
	SetNode(&ny_pid_p,          "p",          &ny_pid_i,          NULL,               NULL,            &ny_pid       , READWRITE,  4.0f,     FLOAT);
	SetNode(&ny_pid_i,          "i",          &ny_pid_d,          &ny_pid_p,          NULL,            &ny_pid       , READWRITE,  0.001f,   FLOAT);
	SetNode(&ny_pid_d,          "d",          NULL,               &ny_pid_i,          NULL,            &ny_pid       , READWRITE,  0.0f,     FLOAT);
	SetNode(&ny_maxch,          "maxch",      &ny_scale,          &ny_pid,            NULL,            &ny_para      , READWRITE,  550,      USHORT);
	SetNode(&ny_scale,          "scale",      &ny_enable,         &ny_maxch,          NULL,            &ny_para      , READWRITE,  6.0f,     FLOAT);
  SetNode(&ny_enable,         "enable",     NULL,               &ny_scale,          NULL,            &ny_para      , READWRITE,  1,        USHORT);
                                                                                                                    
	SetNode(&config_waitack,    "waitack",    NULL,               NULL,               NULL,            &config       , READWRITE,  1,        USHORT);
	
	cur_node = &control;
	cur_path = "";
	cur_cursor_row = 0; 
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
	if(cur_node->prev != NULL)
	{
		cur_node = cur_node->prev;

		if(cur_cursor_row == 0)
			oled_ptr->Push2Left(cur_node->name,0);
		
		MoveCursor(cur_cursor_row - 1);
		
		UpdateChildren();
	}
}

void Menu::Down()
{
	if(cur_node->next != NULL)
	{
		cur_node = cur_node->next;
		
		if(cur_cursor_row == 4)
			oled_ptr->Push2Left(cur_node->name,1);
		
		MoveCursor(cur_cursor_row + 1);
		
		UpdateChildren();
	}
}

void Menu::Left()
{
	if(cur_node->parent != NULL)
	{
		cur_node = cur_node->parent;
		MoveCursor(0);
		UpdateAll();
	}
}

void Menu::Right()
{
	if(cur_node->child != NULL)
	{
		cur_node = cur_node->child;
		MoveCursor(0);
		UpdateAll();
	}
}

void Menu::Kakunin()
{
	printf("%s\r\n",cur_node->name);
}

void Menu::ShowChildren(Oled* oled)
{
	MenuNode_t* temp = cur_node->child;
	u8 cnt = 0;
	while(temp != NULL && cnt < 5)
	{
		oled->Push2Right(temp->name,1);
		temp = temp->next;
		cnt++;
	}
}

void Menu::ShowNexts(Oled* oled)
{
	MenuNode_t* temp = cur_node;
	u8 cnt = 0;
	while(temp != NULL && cnt < 5)
	{
		oled->Push2Left(temp->name,1);
		temp = temp->next;
		cnt++;
	}
}

void Menu::UpdateChildren()
{
	oled_ptr->ClearRight();
	ShowChildren(oled_ptr);
	oled_ptr->Update();
}

void Menu::UpdateAll()
{
	oled_ptr->ClearLeft();
	ShowNexts(oled_ptr);
	UpdateChildren();
}

void Menu::MoveCursor(short row)
{
	oled_ptr->ShowChar(0,cur_cursor_row,' ');
	cur_cursor_row = row;
	if(cur_cursor_row > 4)
		cur_cursor_row = 4;
	else if(cur_cursor_row < 0)
		cur_cursor_row = 0;
	oled_ptr->ShowChar(0,cur_cursor_row,'>');
}