#include "fifo.h"
#include "string.h"

unsigned short GetArrayFifoMaxLength(struct _ArrayFifo_t* fifo)
{
	return fifo->max_length;
}

unsigned short GetArrayFifoCurLength(struct _ArrayFifo_t* fifo)
{
	return fifo->cur_length;
}

unsigned char PushData2ArrayFifo(struct _ArrayFifo_t* fifo, unsigned char dat)
{
	unsigned char i;
	
	if(fifo->cur_length < fifo->max_length)
	{
		fifo->fifo_data[fifo->cur_length] = dat;
		fifo->cur_length++;
	}
	else
	{
		for(i = 0; i < fifo->cur_length - 1; i++)
		{
			fifo->fifo_data[i] = fifo->fifo_data[i+1];
		}
		fifo->fifo_data[fifo->max_length - 1] = dat;
	}
	
	return 1;
}

unsigned char PushString2ArrayFifo(struct _ArrayFifo_t* fifo, unsigned char* string)
{
	unsigned char len = strlen(string);
	printf("len : %d\r\n",len);
	
	if(fifo->cur_length + len < fifo->max_length)
	{
		
	}
	return 1;
}

