#ifndef __PID_H__
#define __PID_H__

#include "includes.h"

typedef struct
{
	float tar;
	float cur;
	
	float kp;
	float ki;
	float kd;
	
	float error;
	float old_error;
	float integration;
	
	float kp_output;
	float ki_output;
	float kd_output;
	float output;
}PositionPid;

typedef struct
{
	float tar;
	float cur;
	
	float kp;
	float ki;
	float kd;
	
	float error;
	float old_error;
	float old_old_error;
	
	float kp_output;
	float ki_output;
	float kd_output;
	float output;
}IncrementPid;

void CalcPositionPid(PositionPid* pid);
void CalcIncrementPid(IncrementPid* pid);
void PositionPidLimit(PositionPid* pid, float min, float max);
void IncrementPidLimit(IncrementPid* pid, float min, float max);

#endif
