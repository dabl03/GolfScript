#ifndef TEST_H
#define TEST_H 1
	#include <stdbool.h>
	#include "./memory.h"
	#define HEADER_SRC(path) "../../src/header/path"##path
	#include HEADER_SRC("define.h")

	typedef struct SimpleStack{
		struct SimpleStack* next;
		void* ptr;
	}SimpleStack;
	typedef struct FuncTest{
		struct FuncTest* next;
		bool (*func)(const char** out);
		char* name;
		bool is_name_dinamyc;
		bool is_stop;
	}FuncTest;
	void push_function_test(bool (*func)(const char** out));
	int ExecuteTest();
	void show_repeat_chr(char for_repeat, uint count, bool is_end_line);
	void show_tr_center(char separator, uint max_len, char* msg, bool is_end_line);
#endif