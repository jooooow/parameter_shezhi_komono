#include "chassis.h"

extern short chassis_motor_speed[6];
extern IncrementPid fl_pid;
extern IncrementPid fr_pid;
extern IncrementPid lb_pid;
extern IncrementPid br_pid;

void InitChassisControlTask(void)
{

}

void ChassisControlTask(void)
{
	fr_pid.cur = chassis_motor_speed[1];
	fr_pid.tar = 0;
	CalcIncrementPid(&fr_pid);
	IncrementPidLimit(&fr_pid,-20000,20000);
	Can1SendMsg2Chassis(0,fr_pid.output,0,0);
}

