#ifndef __VERTICAL_LAYOUT_H__
#define __VERTICAL_LAYOUT_H__

#include "sys.h"
#include "string.h"
#include "stdio.h"
#include <deque>

typedef struct _OledBufString_t
{
	_OledBufString_t(const char* _str, u8 _id)
	{
		id = _id;
		len = strlen(_str);
		str = new char[len];
		memcpy(str,_str,len);
		str[len] = '\0';
	}
	
	~_OledBufString_t()
	{
		delete[] str;
	}
	
	char* str;
	u8 len;
	u8 id;
}OledBufString_t;


class VerticalLayout
{
public:
	VerticalLayout(u8 w, u8 h, u8 m);

	u8 GetMaxHeight(){return height;}
	u8 GetMaxWidth() {return width;}
	u8 GetCurHeight(){return buf.size();}
	u8 GetLeftMargin(){return left_margin;}
	
	void PushFront(const char* str);
	void PushBack(const char* str);
	void PopFront();
	void PopBack();
	void Clear();
	u8 GetBufSize();
	OledBufString_t* GetAt(int index);
		
private:
	u8 width;
	u8 height;
	u8 left_margin;

	std::deque<OledBufString_t*> buf;
	u8 ClearAt(u8 index);
	char* GetFormatString(const char* str, char blank = ' ');
};


#endif

