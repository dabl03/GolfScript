#ifndef ERROR_C
#define ERROR_C 1
  #include "./header/define.h"
  #include "./header/errors.h"
  const char* RERR_STR_MSG[ERROR_MSG_LIMIT]={
    // R
    "Datos no tratados",
    "Caracteristica no disponible"
  };
  const char* GERR_STR_MSG[ERROR_MSG_LIMIT]={
    // G
    "Sintaxis incorrecta"
  };
  const char* BERR_STR_MSG[ERROR_MSG_LIMIT]={
    // B
    "Se han pasado insuficientes argumentos",
    "La pila está vacia"
  };
  U_INT error_code=NORMAL;
  /// Agregar prinft con valores por defectos, como linea, archivo, etc... y cada error indicará si quiere usar uno de eso, nota: Preparar un formateador para ello con $1 $2 $3,,,
  void show_error(const unsigned long int CODES,const unsigned int line){
    const short R_CODE=CODES>>4;
    const short G_CODE=(CODES<<2)>>4;
    const short B_CODE=(CODES<<4)>>4;
    if (R_CODE){
      msg_color("Error interno: ",RED_COLOR,BLACK_COLOR);
      puts(RERR_STR_MSG[R_CODE]);
    }
    if (G_CODE){
      msg_color("Error: ",RED_COLOR,BLACK_COLOR);
      puts(GERR_STR_MSG[G_CODE]);
    }
    if (B_CODE){
      msg_color("Advertensia: ",ORANGE_COLOR,BLACK_COLOR);
      puts(BERR_STR_MSG[B_CODE]);
    }
  }
#endif