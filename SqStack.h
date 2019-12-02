//#ifndef _SqStack_
//#define _SqStack_
#include "StatusNum.h"
#define MAXSIZE 100

//˳��ջ�ṹ
typedef struct{
	ElemType *base;
	ElemType *top;
	int size;
}SqStack; 

//ջ�ĳ�ʼ��,��ʼ����Ϊm��
Status InitStack(SqStack &S, int m){
	S.base = (ElemType*)malloc(m * sizeof(ElemType));
	if(!S.base) exit(OVERFLOW);
	S.top = S.base;
	S.size = m;
	return OK;
}

//ջ�ṹ���١�
Status DestroyStack(SqStack &S){
	free(S.base);
	S.base = S.top = NULL;
	S.size = 0;
	return OK;
}

//���ջ��
Status ClearStack(SqStack &S){
	S.top = S.base;
	return OK;
}

//���ջ�Ƿ�Ϊ�ա�
Status StackEmpty(SqStack S){
	if(S.top == S.base) return TRUE;
	return FALSE;
}

//��ջ�Ĵ�С��
int StackLength(SqStack S){
	return S.top - S.base;
}

//��ջ������e��ջ����
Status Push(SqStack &S, ElemType e){
	if(S.top - S.base >= S.size) return ERROR;
	*(S.top)++ = e;
	return OK;
}

//��ջ���Ⱦ�������ջ�ǿա�
Status Pop(SqStack &S, ElemType &e){
	if(StackEmpty(S)) return ERROR;
	e = *--S.top;
	return OK;
}

//�ж�ջ�Ƿ�����
Status IsFull(SqStack S){
	if(S.top - S.base == S.size) return TRUE;
	return FALSE;
}

//����ջ��
Status StackTraverse(SqStack S, Status(*visit)(ElemType)){
	ElemType* p;
	for(p = S.base; p != S.top; p++){
		visit(*p);
	}
	return OK;
}

//#endif
