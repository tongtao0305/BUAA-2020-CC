#pragma once

#include "Global.h"

// 以下为外部引用的变量和函数
extern string CODEN[40];
extern string CODES[40];

extern ifstream ifile;
extern ofstream ofile;

extern void error(char errorCode);		// 错误处理函数

// 以下为内部定义的变量和函数
int isFirst = 1;			// 标记是否为第一次调用

int symbol;					// 保存当前所识别单词的类型
char CHAR;					// 存放当前读入的字符
char TOKEN[256];			// 存放单词字符串
char buffer[256];			// 文件输入缓冲区

int cnt;					// 记录下一个读取的字符的位置
int cnt_len;				// 记录当前缓冲区字符串的长度
int cnt_line;				// 记录当前缓冲区在源文件中的行数，以便进行错误输出
int lastCountLine;

int lastBegin;				// 上一个Token起始位置
int lastSymbol;				// 上一个Token的类型
char lastTOKEN[256];		// 上一个Token的字符串

int lastlastBegin;			// 上上个Token起始位置
int lastlastSymbol;			// 上上个Token的类型
char lastlastTOKEN[256];	// 上上个Token的字符串

void getsym();				// 获取下一个 symbol
void getsym(bool isOutput);
void trysym();				// 获取下一个 symbol，但不输出
void ungetsym();			// 回退一个 symbol
void outputsym();			// 输出当前的 symbol

int isDigit(char c);		// 判断字符是否为 数字
int isLetter(char c);		// 判断字符是否为 字母
int isChar(char c);			// 判断字符是否为 字符中的字符，即 '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
int isString(char c);		// 判断字符是否为 字符串中的字符，即 "｛十进制编码为32,33,35-126的ASCII字符｝"

void cat(char s[], char c); // 将字符插入到字符串的末尾
int getnbc();				// 从字符缓冲区获取下一个非空字符，返回 1 表示成功，返回 0 表示失败
void getch();				// 从字符缓冲区获取一个字符 
void ungetch();				// 从字符缓冲区回退一个字符 
void reserve(char s[]);		// 判断获取到的字符串是否为保留字
void tolowercase(char s[]);	// 将字符串中的大写英文字符转化为小写