//#ifndef _SqStack_
//#define _SqStack_
#include "StatusNum.h"
#define MAXSIZE 100

//顺序栈结构
typedef struct{
	ElemType *base;
	ElemType *top;
	int size;
}SqStack; 

//栈的初始化,初始长度为m。
Status InitStack(SqStack &S, int m){
	S.base = (ElemType*)malloc(m * sizeof(ElemType));
	if(!S.base) exit(OVERFLOW);
	S.top = S.base;
	S.size = m;
	return OK;
}

//栈结构销毁。
Status DestroyStack(SqStack &S){
	free(S.base);
	S.base = S.top = NULL;
	S.size = 0;
	return OK;
}

//清空栈。
Status ClearStack(SqStack &S){
	S.top = S.base;
	return OK;
}

//辨别栈是否为空。
Status StackEmpty(SqStack S){
	if(S.top == S.base) return TRUE;
	return FALSE;
}

//求栈的大小。
int StackLength(SqStack S){
	return S.top - S.base;
}

//入栈，插入e到栈顶。
Status Push(SqStack &S, ElemType e){
	if(S.top - S.base >= S.size) return ERROR;
	*(S.top)++ = e;
	return OK;
}

//出栈，先决条件是栈非空。
Status Pop(SqStack &S, ElemType &e){
	if(StackEmpty(S)) return ERROR;
	e = *--S.top;
	return OK;
}

//判断栈是否满。
Status IsFull(SqStack S){
	if(S.top - S.base == S.size) return TRUE;
	return FALSE;
}

//遍历栈。
Status StackTraverse(SqStack S, Status(*visit)(ElemType)){
	ElemType* p;
	for(p = S.base; p != S.top; p++){
		visit(*p);
	}
	return OK;
}

//#endif
