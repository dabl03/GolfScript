#ifndef ERROR_H
#define ERROR_H 0x0F0000
	#include <stdio.h>
	#include "./stack.h"
	#define NORMAL 0x0
	// R -- Errores internos de la APPs
	#define APP_UNKNOWN_DATA 0x010000
	#define FEATURE_NOT_AVAILABLE 0x020000
	#define INSUFFICIENT_MEMORY 0x030000
	#define ITEM_COPYING_FAILED 0x040000
	// G -- Errores de sintaxis
	#define NOT_FOUND 0x00100
	// B -- Advertencias
	#define INSUFFICIENT_ARGUMENTS 0x000001
	#define EMPTY_STACK 0x000002
	#define INDEX_NOT_FOUND 0x000003
	#define CONVERTION_NOT_FOUND 0x000004
	#define INVALID_ASSIGNMENT 0x000005
	// El sufijo "A_" es para referirse a mensaje interno de la APP
	//TODO: USAR HEXADECIMAL con formato rgb
	//El tipo 1-F(F mas grave) tiene subtipos... //Descartado por ahora..
	// Por ejemplo: 01 seria uno error grave lijero o algo asi. //Descartado por ahora..
	// Color:
	#define RED_COLOR 0xF00
	#define ORANGE_COLOR 0xFF0
	#define BLACK_COLOR 0x000
	#ifndef ANSI_COLOR
		#define msg_color printf
	#endif
	#define ERROR_MSG_LIMIT 50
	// These are the errors menssage.
	extern const char* RERR_STR_MSG[ERROR_MSG_LIMIT];
	extern const char* GERR_STR_MSG[ERROR_MSG_LIMIT];
	extern const char* BERR_STR_MSG[ERROR_MSG_LIMIT];
	/**
	 * Convert all passed type_values into a string to see their values.
	 * @param  count_parameters Number of type_values present.
	 *  Note: If 0 is passed, the loop will execute until null is reached.
	 * @param  parameters       List of parameters to convert to string
	 * @return                  String with the list of parameters separated by commas.
	 */
	char* get_str_param(const uint count_parameters, struct type_value** parameters);
	/**
	 * Show the error made in the function
	 * @param func Name of the function.
	 * @param count_parameters Number of type_values present.
	 * @param parameters List of parameters to convert to string
		Note: call get_str_param(count_parameters,parameters)
	 */
	void func_error(
		const char* func,
		const uint count_parameters,
		struct type_value** parameters, ...
	);
	/**
	 * It displays the error that has occurred.
	 * @param CODES  The error code.
	 * @param line, col   Where the error occurred
	*/
	void show_error(
		const unsigned long int CODES,
		const uint line,
		const uint col
	);
	void init_get_time();
	char* end_time();
#endif