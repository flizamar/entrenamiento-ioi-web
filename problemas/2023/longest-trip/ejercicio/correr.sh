#!/usr/bin/env bash
# Compila ejercicio.cpp junto al grader de ejemplo y lo corre con los ejemplos.
# Uso:  ./correr.sh          (corre el ejemplo 01)
#       ./correr.sh 02       (corre el ejemplo 02)
set -euo pipefail

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SAMPLE="$DIR/../sample"
EJ="${1:-01}"

# GCC real si esta; si no, clang (g++) sirve igual para este programa.
# OJO: NADA de -static -> en macOS falla con "library not found for -lcrt0.o".
GXX=g++
for c in /opt/homebrew/bin/g++-* /usr/local/bin/g++-*; do
    [ -x "$c" ] && GXX="$c"
done

echo "Compilando con: $GXX"
"$GXX" -std=gnu++17 -O2 -Wall -Wextra -I "$SAMPLE/cpp" \
    "$DIR/ejercicio.cpp" "$SAMPLE/cpp/grader.cpp" -o "$DIR/ejercicio"

echo "== Ejecutando ejemplo ${EJ} (salida del programa abajo) =="
"$DIR/ejercicio" < "$SAMPLE/examples/${EJ}.in"
