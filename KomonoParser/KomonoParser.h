#pragma once
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define new_parser() { KomonoReset, KomonoSetCmd, KomonoGetRecvCmdPathDepth ,KomonoIsPathMatch,KomonoDataCheck, KomonoPushCmdChar }
#define MatchThenSet(a,b,c) SetValue((a),(b),(c),OnSetSuccess) 

typedef struct _KomonoCmdParser
{
	void(*Reset)(_KomonoCmdParser* self);
	void(*SetCmd)(_KomonoCmdParser* self, const char* cmd);
	int(*GetRecvCmdLen)(_KomonoCmdParser* self);
	char(*IsPathMatch)(_KomonoCmdParser* self, const char* path);
	char(*DataCheck)(_KomonoCmdParser* self);
	char(*PushCmdChar)(_KomonoCmdParser* self, char ch);
	char recv_cmd[64];
	char recv_cmd_path[64];
}KomonoCmdParser;

void KomonoReset(_KomonoCmdParser* self)
{
	memset(self->recv_cmd, '\0', 64);
	memset(self->recv_cmd_path, '\0', 64);
}

void KomonoSetCmd(_KomonoCmdParser* self, const char* cmd)
{
	int len = strlen(cmd);
	if (cmd[len - 1] == '\n' && cmd[len - 2] == '\r')
		memcpy(self->recv_cmd, cmd, len - 2);
	else
		memcpy(self->recv_cmd, cmd, len);
}

int KomonoGetRecvCmdPathDepth(KomonoCmdParser* self)
{
	char* p = self->recv_cmd;
	int path_depth_flag = 0;
	int path_depth = 0;
	while (*p != '\0')
	{
		if (*p == '/' && path_depth_flag == 0)
			path_depth_flag = 1;
		if (path_depth_flag == 1)
		{
			if (*p == ' ')
				break;
			path_depth++;
		}
		p++;
	}
	return path_depth;
}

char KomonoIsPathMatch(KomonoCmdParser* self, const char* path)
{
	int recv_path_depth = self->GetRecvCmdLen(self);
	memcpy(self->recv_cmd_path, self->recv_cmd + 5, recv_path_depth);
	if (strcmp(path, self->recv_cmd_path))
		return 0;
	else
		return 1;
}

char KomonoDataCheck(_KomonoCmdParser* self)
{
	char* p = self->recv_cmd;
	unsigned int sum = 0;
	while (*(p + 1) != '\0')
	{
		sum += *p;
		p++;
	}
	sum = sum % 26 + 'A';
	return (sum == *p);
}

char KomonoPushCmdChar(_KomonoCmdParser* self, char ch)
{
	static unsigned char begin_save_flag = 0;
	static unsigned char recv_cmd_cnt = 0;
	if (ch == '$')
	{
		begin_save_flag = 1;
	}
	if (begin_save_flag == 1)
	{
		if (ch != '\r' && ch != '\n')
		{
			self->recv_cmd[recv_cmd_cnt] = ch;
			recv_cmd_cnt++;
			if (recv_cmd_cnt >= 64)
			{
				self->Reset(self);
				recv_cmd_cnt = 0;
			}
		}
	}
	if (ch == '\n')
	{
		begin_save_flag = 0;
		if (self->DataCheck(self))
		{
			recv_cmd_cnt = 0;
			return 1;
		}
		else
		{
			self->Reset(self);
			recv_cmd_cnt = 0;
		}
	}
	return 0;
}

void SetValue(KomonoCmdParser* self, const char* path, double* value, void(*SuccessHook)(const char*, double*))
{
	if (self->IsPathMatch(self, path))
	{
		char temp[16] = { '\0' };
		int recv_path_depth = KomonoGetRecvCmdPathDepth(self);
		memcpy(temp, self->recv_cmd + 5 + recv_path_depth, strlen(self->recv_cmd) - 5 - recv_path_depth);
		double f = atof(temp);
		*value = f;
		SuccessHook(path, value);
	}
	else
	{
		SuccessHook(path, NULL);
	}
}

void OnSetSuccess(const char* path, double* val)
{
	if (val == NULL)
	{
		//printf("\nfail set : %s\n", path);
	}
	else
	{
		printf("\nsuccess set : %s to %.5f\n", path, *val);
	}
}