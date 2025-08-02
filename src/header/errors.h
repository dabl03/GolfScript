#ifndef ERROR_H
  #include <stdio.h>
  #define ERROR_H 0x0F0000
  #define NORMAL 0x0
  // R -- Errores internos de la APPs
  #define APP_UNKNOWN_DATA 0x010000
  #define FEATURE_NOT_AVAILABLE 0x020000
  // G -- Errores de sintaxis
  #define NOT_FOUND 0x00100
  // B -- Advertencias
  #define INSUFFICIENT_ARGUMENTS 0x000001
  #define EMPTY_STACK 0x000002
  // El sufijo "A_" es para referirse a mensaje interno de la APP
  //TODO: USAR HEXADECIMAL con formato rgb
  //El tipo 1-F(F mas grave) tiene subtipos... //Descartado por ahora..
  // Por ejemplo: 01 seria uno error grave lijero o algo asi. //Descartado por ahora..
  #ifndef ANSI_COLOR
    #define msg_color printf
  #endif
  // These are the errors menssage.
  extern const char** RERR_STR_MSG;
  extern const char** GERR_STR_MSG;
  extern const char** BERR_STR_MSG;
#endif