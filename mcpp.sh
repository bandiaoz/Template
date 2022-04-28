#! /bin/bash

name=$1
# input_file="/Users/chenjinglong/cpp_code/io/in.in"
# output_file="/Users/chenjinglong/cpp_code/io/out.out"

g++ -g -O2 -std=gnu++17 \
-DLOCAL \
-D"de(...) W('[', #__VA_ARGS__,\"]==\", __VA_ARGS__)" \
-include /Users/chenjinglong/cpp_code/template/Print.h \
-Wfatal-errors "$1".cpp

time ./a.out <${input_file} >${output_file}

rm a.out 
rm -rf *.dSYM 

# CPH 的编译参数
# -DLOCAL -O2 -std=c++17 -Dde(...)=W('[',#__VA_ARGS__,"]==",__VA_ARGS__) -include /Users/chenjinglong/cpp_code/template/Print.h