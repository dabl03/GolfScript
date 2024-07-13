#include <stdio.h>
#include <gmp.h>

#ifndef P
#define P 10
#endif
int main(void){
  mpf_t ldPI;
	mp_exp_t mp_exponent;
  char* sOut;
  mpf_init (ldPI);
  mpf_set_si(ldPI,3.14156);
  puts("Vemos el flotante...");
  puts(mpf_get_str(NULL,&mp_exponent,10,30,ldPI));
  puts("End...");
  return 0;
}
