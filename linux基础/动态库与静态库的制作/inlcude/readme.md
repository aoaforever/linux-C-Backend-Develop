---
## 静态库的制作
```bash
g++ hello.cpp  -c
ar rcs libhello.a hello.o

## 将生成的libhello.a静态库拷贝到../lib/下
```

## 动态库的制作
```bash
g++ -shared -fpic hello.cpp -o libhello.so

## 将生成的libhello.so动态库拷贝到../lib/下
```
