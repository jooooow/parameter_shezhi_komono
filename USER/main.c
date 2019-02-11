#include "includes.h"
#include "spi.h"
#include "oled.h"
#include "oled.hpp"
#include "string.h"
#include "stdio.h"
#include "menu.h"

#define MODE_SET   0
#define MODE_DEBUG 1

Oled_t oled  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,7,16,0};
Oled_t oled_arrow  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,2,0};
Oled_t oled_left  = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,7,8};
Oled_t oled_right = {OledClearAll,OledSetPos,OledDisplayChar,OledDisplayImage,OledDisplayString,OledShowHLLIcon,OledPushString2BufAndDisplay,0,6,7,72};

u8 mode = MODE_SET;

