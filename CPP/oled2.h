#ifndef __OLED_HPP__
#define __OLED_HPP__
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include <deque>
#include "VerticalLayout.h"

class Oled
{
private:
	u8 height;
	u8 width;
  u8 layout_left_x;
	u8 layout_right_x;

public:
	VerticalLayout* layout_left;
	VerticalLayout* layout_right;

public:
	Oled(u8 h, u8 w);
	~Oled();

	u8 GetHeight();
	u8 GetWidth();

	void SetLayout(VerticalLayout* l, VerticalLayout* r);

	void ClearAll();
	void ShowChar(u8 x, u8 y, u8 ch);
	void ShowString(u8 x,u8 y,const char *str);
	void ShowImg(u8 x0,u8 y0,u8 x1,u8 y1,u8 img[]);

	void Update();
	void UpdateRight();
	void UpdateLeft();

	void Push2Left(const char* str, u8 direction);
	void Push2Right(const char* str, u8 direction);

	void ClearRight();
	void ClearLeft();
	
	u8 GetMaxLayoutHeight();

private:
	void InitGpio();
	void InitOled();
	void SetPosition(u8 x, u8 y);
	void WriteByte(u8 byt);
	void WriteData(u8 dat);
	void WriteCmd(u8 cmd);
};



#endif
