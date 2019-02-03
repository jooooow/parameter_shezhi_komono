#ifndef __FIFO_H__
#define __FIFO_H__

typedef struct _ListFifoNode_t
{
	unsigned short data;
	struct _ListFifoNode_t* next;
}ListFifoNode_t;

typedef struct _ListFifo_t
{
	struct ListFifoNode_t* head;
	struct ListFifoNode_t* last;
	unsigned short max_length;
	unsigned short cur_length;
	
	unsigned short(*GetCurLength)(struct _ListFifo_t* fifo);
	unsigned char(*PushData)(struct _ListFifo_t* fifo,unsigned short dat);
	unsigned char(*ClearAll)(struct _ListFifo_t* fifo);
}ListFifo_t;

typedef struct _ArrayFifo_t
{
	unsigned char* fifo_data;
	unsigned short max_length;
	unsigned short cur_length;
	
	unsigned short(*GetMaxLength)(struct _ArrayFifo_t* fifo);
	unsigned short(*GetCurLength)(struct _ArrayFifo_t* fifo);
	unsigned char(*PushData)(struct _ArrayFifo_t* fifo, unsigned char dat);
	unsigned char(*PushString)(struct _ArrayFifo_t* fifo, unsigned char* string);
	unsigned char(*ClearAll)(struct _ArrayFifo_t* fifo);
}ArrayFifo_t;

unsigned short GetArrayFifoMaxLength(struct _ArrayFifo_t* fifo);
unsigned short GetArrayFifoCurLength(struct _ArrayFifo_t* fifo);
unsigned char PushData2ArrayFifo(struct _ArrayFifo_t* fifo, unsigned char dat);
unsigned char PushString2ArrayFifo(struct _ArrayFifo_t* fifo, unsigned char* string);

typedef struct _OledFifo_t
{
	unsigned char* fifo_data;
}OledFifo_t;
#endif