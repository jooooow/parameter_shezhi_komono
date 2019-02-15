#include <iostream>
using namespace std;

#include "KomonoParser.h"

int main()
{
	double p = 0.0f;
	double i = 0.0f;
	double d = 0.0f;

	KomonoCmdParser parser = new_parser();

	char temp_path1[] = "$SET /chassis/pid/p 7.12300B\r\n";
	char temp_path2[] = "$SET /chassis/pid/i 0.21280U\r\n";
	char temp_path3[] = "$SET /chassis/pid/d 101.501K\r\n$SET /chassis/pid/i 0.2120Q\r\n";

	/*********** 逐字节写入测试 ***********/
	char* path_ptr = temp_path1;
	parser.Reset(&parser);
	while (*path_ptr != '\0')
	{
		char data_ready = parser.PushCmdChar(&parser, *path_ptr);
		path_ptr++;
		if (data_ready)
		{
			MatchThenSet(&parser, "/chassis/pid/p", &p);
			MatchThenSet(&parser, "/chassis/pid/i", &i);
			MatchThenSet(&parser, "/chassis/pid/d", &d);

			printf("%.7f\t%.7f\t%.7f\n", p, i, d);
			parser.Reset(&parser);
		}
	}
	/*************************************/


	/*********** 一次性写入测试 ***********/
	parser.Reset(&parser);
	parser.SetCmd(&parser, temp_path2);
	char data_ready = parser.DataCheck(&parser);
	if (data_ready)
	{
		MatchThenSet(&parser, "/chassis/pid/p", &p);
		MatchThenSet(&parser, "/chassis/pid/i", &i);
		MatchThenSet(&parser, "/chassis/pid/d", &d);

		printf("%.7f\t%.7f\t%.7f\n", p, i, d);
		parser.Reset(&parser);
	}
	/*************************************/


	/*********** 连续逐字节写入测试 ***********/
	path_ptr = temp_path3;
	parser.Reset(&parser);
	while (*path_ptr != '\0')
	{
		char data_ready = parser.PushCmdChar(&parser, *path_ptr);
		path_ptr++;
		if (data_ready)
		{
			MatchThenSet(&parser, "/chassis/pid/p", &p);
			MatchThenSet(&parser, "/chassis/pid/i", &i);
			MatchThenSet(&parser, "/chassis/pid/d", &d);

			printf("%.7f\t%.7f\t%.7f\n", p, i, d);
			parser.Reset(&parser);
		}
	}
	/***************************************/

	system("pause");
	return 0;;
}