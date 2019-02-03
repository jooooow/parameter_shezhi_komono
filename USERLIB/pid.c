#include "pid.h"
#include "stdlib.h"

IncrementPid fl_pid = {0,0,  35,0.2,0, 0,0,0,0,0,0,0};
IncrementPid fr_pid = {0,0,  35,0.2,0, 0,0,0,0,0,0,0};
IncrementPid lb_pid = {0,0,  0,0,0,    0,0,0,0,0,0,0};
IncrementPid br_pid = {0,0,  0,0,0,    0,0,0,0,0,0,0};

float error_dev = 0;

void CalcPositionPid(PositionPid* pid)
{
	pid->error = pid->tar - pid->cur;
	
	pid->kp_output = pid->kp * pid->error;
	pid->integration += pid->error;
	pid->ki_output = pid->ki * pid->integration;
	pid->kd_output = pid->kd * (pid->error - pid->old_error);
	
	pid->old_error = pid->error;
	pid->output = pid->kp_output + pid->ki_output + pid->kd_output;
}
void CalcIncrementPid(IncrementPid* pid)
{
	pid->error = pid->tar - pid->cur;
	if(abs(pid->error) < 20)
		pid->error = 0;
	
	error_dev = (pid->error - pid->old_error);
	pid->kp_output = pid->kp * error_dev;
	pid->ki_output = pid->ki * pid->error;
	pid->kd_output = pid->kd * (pid->error - 2 * pid->old_error + pid->old_old_error);
	
	pid->old_old_error = pid->old_error;
	pid->old_error = pid->error;
	pid->output += pid->kp_output + pid->ki_output + pid->kd_output;
}

void PositionPidLimit(PositionPid* pid, float min, float max)
{
	if(pid->output > max)
		pid->output = max;
	else if(pid->output < min)
		pid->output = min;
}
void IncrementPidLimit(IncrementPid* pid, float min, float max)
{
	if(pid->output > max)
		pid->output = max;
	else if(pid->output < min)
		pid->output = min;
}
