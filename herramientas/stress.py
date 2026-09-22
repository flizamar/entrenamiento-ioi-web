#!/usr/bin/env python3
"""Compara ./sol con ./bruta usando ./gen SEMILLA; conserva el primer fallo."""
import argparse
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--casos', type=int, default=1000)
    parser.add_argument('--timeout', type=float, default=2)
    args = parser.parse_args()
    if args.casos < 1 or args.timeout <= 0:
        parser.error('casos y timeout deben ser positivos')
    for seed in range(1, args.casos + 1):
        # Sobrescribir solo estos artefactos de diagnóstico en el directorio de trabajo.
        Path('semilla.txt').write_text(str(seed) + '\n')
        for name, command, input_path, output_path in (
            ('gen', ['./gen', str(seed)], None, 'caso.txt'),
            ('sol', ['./sol'], 'caso.txt', 'salida_sol.txt'),
            ('bruta', ['./bruta'], 'caso.txt', 'salida_bruta.txt'),
        ):
            try:
                data = Path(input_path).read_bytes() if input_path else None
                with open(output_path, 'wb') as out, open(name + '.stderr.txt', 'wb') as err:
                    result = subprocess.run(command, input=data, stdout=out, stderr=err,
                                            timeout=args.timeout, check=False)
                if result.returncode:
                    raise RuntimeError(f'código de salida {result.returncode}')
            except (OSError, subprocess.TimeoutExpired, RuntimeError) as exc:
                print(f'FALLO de {name}, semilla {seed}: {exc}. Ver {name}.stderr.txt.', file=sys.stderr)
                return 1
        if Path('salida_sol.txt').read_bytes() != Path('salida_bruta.txt').read_bytes():
            print(f'DIFERENCIA, semilla {seed}: conserva caso.txt y ambas salidas.', file=sys.stderr)
            return 1
    print(f'OK: {args.casos} casos; esto no sustituye una demostración.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
