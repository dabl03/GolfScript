#IFNDEF DEFINE_TEST_H
#define DEFINE_TEST_H 1
  /**
  * Macro para crear función para testear app.
  * Se usa:
  * FUNC_TEST(NAME_FUNC_TEST);
  *   // You code personalize...
  *   return false;
  * } // Cerrar la función.
  */
  #DEFINE FUNC_TEST(NAME_FUNC)\
    bool NAME_FUNC(bool view_name){\
      if (view_name)\
        printf("Testeando la función \"%s\"...\n",#NAME_FUNC);

  #DEFINE EXECUTE_TEST(NAME_FUNC,view_name)\
    if (NAME_FUNC(view_name))\
      printf("-----La función \"%s\" no ha pasado la prueba-----");
    
#ENDIF