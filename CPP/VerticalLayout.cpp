#include "VerticalLayout.h"
#include "stdio.h"

VerticalLayout::VerticalLayout(u8 w, u8 h, u8 m) : width(w), height(h), left_margin(m)
{
	printf("verticalLayout init done!\r\n");
}

void VerticalLayout::PushFront(const char* str)
{
	/*将字符串约束为layout元素尺寸*/
	char* p = GetFormatString(str);
	
	OledBufString_t* v = new OledBufString_t(p, 0);
	if(buf.size() >= height)
		PopBack();
	buf.push_front(v);
	
	delete p;
}

void VerticalLayout::PushBack(const char* str)
{
	/*将字符串约束为layout元素尺寸*/
	char* p = GetFormatString(str);
	
	OledBufString_t* v = new OledBufString_t(p, 0);
	if(buf.size() >= height)
		PopFront();
	buf.push_back(v);
	delete p;
}

void VerticalLayout::PopFront()
{
	delete buf.at(0);
	buf.pop_front();
}

void VerticalLayout::PopBack()
{
	delete buf.at(buf.size() - 1);
	buf.pop_back();
}

u8 VerticalLayout::GetBufSize()
{
	return buf.size();
}

OledBufString_t* VerticalLayout::GetAt(int index)
{
	if(index > buf.size() - 1)
		return NULL;
	
	return buf.at(index);
}

u8 VerticalLayout::ClearAt(u8 index)
{
	if(index > buf.size() - 1)
		return 0;
	delete buf.at(index);
	
	return 1;
}

void VerticalLayout::Clear()
{
	for(int i = 0; i < buf.size(); i++)
	{
		ClearAt(i);
	}
	buf.clear();
}

char* VerticalLayout::GetFormatString(const char* str, char blank)
{
	u8 len = strlen(str);
	char* p = new char[width];
	memcpy(p,str,(len > width ? width : len));
	if(len <= width)
		memset(p + len,blank,width - len);
	p[width] = '\0';
	return p;
}
