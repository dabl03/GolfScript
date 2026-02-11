#ifndef MEMORY_H_
	#define MEMORY_H_ 1
	#include <stdlib.h>
	/**
	 * @brief Asignamos memoria dinamica.
	 * 
	 * @param size Cuanto asignar.
	 * @param file Archivo desde donde se asigna.
	 * @param line Linea desde donde se asigna.
	 * @param func Función desde donde se asigna.
	 * @Param is_normal Si es true, solo retorna
	 *  malloc y no monitorea nada
	 * @return void* Memoria asignada.
	 */
	void* test_malloc(size_t size, const char *file, int line, const char *func, unsigned char is_normal);
	/**
	 * @brief Usamos realloc para aumentar o disminuir la memoria.
	 *  Si no existe ptr se crea uno con malloc.
	 * 
	 * @param ptr Memoria anterior.
	 * @param size Tamaño a cambiar.
	 * @param file Desde donde se llamo
	 * @param line Desde donde se llamo
	 * @param func Desde donde se llamo
	 * @return void* Memoria asignada. 
	 */
	void* test_realloc(void* ptr,size_t size, const char *file, int line, const char *func);
	/**
	 * @brief Liberamos la memoria asignada.
	 * 
	 * @param ptr Memoria a liberar.
	 * @param file Cadena donde se llamo a la función.
	 * @param line Linea donde se llamo.
	 * @param func Función desde donde se llamo.
	 * @param is_normal Indica que solo se quiere liberar y no comprobar.
	 * Nota: No usar con otras biblioteca que no usan test_malloc, sino malloc directamente.
	 */
	void test_free(void* ptr, const char *file, int line, const char *func,unsigned char is_normal);
	/** @brief Muestra la memoria dinamica todavia asignada y donde se asigno. */
	void viewStack();
	#define malloc(size) test_malloc( size, __FILE__, __LINE__, __FUNCTION__, 0)
	#define realloc(ptr,size) test_realloc( ptr,size, __FILE__, __LINE__, __FUNCTION__)
	#define free(ptr) test_free( ptr, __FILE__, __LINE__, __FUNCTION__,0)
	// Evitamos los warning de los test
	#ifdef FREE__
		#undef FREE__
	#endif
	#define FREE__(ptr) test_free( ptr, __FILE__, __LINE__, __FUNCTION__, 1)
	#ifdef MALLOC__
		#undef MALLOC__
	#endif
	#define MALLOC__(size) test_malloc( size, __FILE__, __LINE__, __FUNCTION__, 1)
#endif