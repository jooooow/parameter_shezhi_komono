#ifndef __MENU_H__
#define __MENU_H__

#include "sys.h"

#define READONLY  0
#define READWRITE 1

#define SHORT     0
#define FLOAT     1
#define USHORT     2

#define UP_PUSH      (!k_sta[1] && old_k_sta[1])
#define DOWN_PUSH    (!k_sta[2] && old_k_sta[2])
#define LEFT_PUSH    (!k_sta[0] && old_k_sta[0])
#define RIGHT_PUSH   (!k_sta[3] && old_k_sta[3])
#define KAKUNIN_PUSH (!k_sta[4] && old_k_sta[4])

typedef union
{
	u8    us;
	short s;
	float f;
}MenuValue_t;

typedef struct _MenuNode_t
{
	char name[7];
	MenuValue_t value;
	u8 value_type;
	u8 authority;
	struct _MenuNode_t* next;
	struct _MenuNode_t* prev;
	struct _MenuNode_t* child;
	struct _MenuNode_t* parent;
}MenuNode_t;

typedef struct _Menu_t
{
	MenuNode_t* head_node;
	MenuNode_t* cur_node;
}Menu_t;

void InitMenu(void);
void SetNode(MenuNode_t* self, char n[7], float val, u8 value_type, u8 au, MenuNode_t* next, MenuNode_t* prev, MenuNode_t* child, MenuNode_t* parent);
void Move(Menu_t* self);
void GetFullPathAndValue(MenuNode_t* node);
void DisplayNext(MenuNode_t* nod);
void DisplayChildren(MenuNode_t* node);
void DisplayDiv(void);
void DisplaySolider(void);

void MenuUp(Menu_t* self);
void MenuDown(Menu_t* self);
void MenuLeft(Menu_t* self);
void MenuRight(Menu_t* self);
void MenuKakunin(Menu_t* self);
void ValueSet(Menu_t* node);

#endif