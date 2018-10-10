/*************************************************************************
  > File Name: inert_ip.h
  > Author: 
  > Mail: 
  > Created Time: 2018年10月10日 星期三 06时35分29秒
 ************************************************************************/

#include<iostream>
#include <map>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define PARAMS_MAX 100
#define TIME_OUT 60
#define OUT_PUT_NUM 30

struct SrcDstParam{
	char* cDoMain;
	unsigned int iSrcIp;
	unsigned int iDstIp;
	int iFlag;
};

struct SrcCountTime{
	int iCount;
	unsigned int iTimeVal;
};

multimap<unsigned int,SrcDstParam> srcParamsMap;

map<unsigned int, SrcCountTime> srcTimeValMap;

pthread_t pthdId;

//初始化打印输出线程
int init();

//线程函数
void* onThreadFun(void*);

//插入域名接口
int insertParam(char* cDoMain, unsigned int iSrcIp, int iFlag, unsigned int iDstIp);

//打印输出满足条件的src和域名接口
int printExtentParams();
