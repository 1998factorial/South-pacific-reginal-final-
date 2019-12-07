#!/bin/sh

python3 C2.gen.py > samples-C/a.in
g++ C.cpp -o C && ./C < samples-C/a.in > samples-C/a.out
g++ C2.cpp -o C2 && ./C2 < samples-C/a.in > samples-C/a.out.2
diff samples-C/a.out samples-C/a.out.2
