// Verifica que los números que afirma la lección del módulo 10 sean ciertos:
// las trazas de dígitos, de Euclides y de potmod, y el caso de los negativos.
#include <bits/stdc++.h>
using namespace std;
long long a = 48, b = 18;
#include "ruta_m10-mod-positivo.inc"
#include "ruta_m10-es-primo.inc"
#include "ruta_m10-mcd-mcm.inc"
#include "ruta_m10-potmod.inc"
int main(){
  assert(mod_positivo(-7,3) == 2);
  assert(es_primo(36) == false && es_primo(37) == true && es_primo(1) == false);
  assert(mcd(48,18) == 6);
  assert(potmod(3,13,1000) == 323);
  assert(mcm == 144);
  assert(4025 % 10 == 5 && 4025 / 10 == 402);
  assert(-7 % 3 == -1 && -7 / 3 == -2);
  printf("\n>>> las afirmaciones del modulo 10 son ciertas\n");
}