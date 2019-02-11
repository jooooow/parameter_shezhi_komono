#ifndef __MENU2_H__
#define __MENU2_H__

#include "sys.h"
#include "string.h"
#include "oled2.h"
#include <string>

typedef struct _MenuNodeValue_t
{
	enum Type{UCHAR, USHORT, FLOAT, UNKNOWN};
	
	_MenuNodeValue_t(){u_8 = 0; u_16 = 0; f = 0.0f; type = UNKNOWN;}
	void SetValue(float val, u8 tp)
	{
		if(tp == UCHAR)
			u_8 = (u8)val;
		else if(tp == USHORT)
			u_16 = (u16)val;
		else if(tp == FLOAT)
			f = val;
	}
		
	u8    u_8;
	u16   u_16;
	float f;
	
	u8 type;
}MenuNodeValue_t;

typedef struct _MenuNode_t
{
	enum Authority{READONLY, READWRITE, SYS, UNKNOWN};
	
	void Set(const char* n, _MenuNode_t* _next, _MenuNode_t* _prev,_MenuNode_t* _child,_MenuNode_t* _parent, u8 _authority, float val, u8 type)
	{
		memcpy(name, n, strlen(n) >= 7 ? 7 : strlen(n));
		name[7] = '\0';
		
		next = _next;
		prev = _prev;
		child = _child;
		parent = _parent;
		authority = _authority;
		value.SetValue(val, type);
	}
	
	_MenuNode_t* next;
	_MenuNode_t* prev;
	_MenuNode_t* child;
	_MenuNode_t* parent;
	
	u8 authority;
	MenuNodeValue_t value;
	char name[7];
}MenuNode_t;

class Menu
{
	
public:
	enum Key{UP,DOWN,LEFT,RIGHT,KAKUNIN,UNKNOWN};
	Menu();
	~Menu(){};
	void Move(u8 pushed_key = UNKNOWN);
		
	MenuNode_t* GetCurNode(){return cur_node;}
	void ShowNexts(Oled* oled);
	void ShowChildren(Oled* oled);
	
private:
	MenuNode_t* cur_node;
	std::string cur_path;
	short cur_cursor_row;

private:
	void Left();
	void Right();
	void Up();
	void Down();
	void Kakunin();

	void UpdateChildren();
	void UpdateAll();
	void MoveCursor(short row);
};

#endif
