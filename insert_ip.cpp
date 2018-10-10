/*************************************************************************
	> File Name: inert_ip.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年10月08日 星期一 12时09分17秒
 ************************************************************************/
#include "insert_ip.h"

int init()
{
	pthread_create(&pthdId, NULL, onThreadFun, NULL);
}

void* onThreadFun(void*)
{
	while(1)
	{
		printExtentParams();
		sleep(5);
	}
}

int insertParam(char* cDoMain, unsigned int iSrcIp, int iFlag, unsigned int iDstIp)
{
	if(iFlag == 0)
	{
		SrcCountTime timeval;
		struct timeval nowtime;
		gettimeofday(&nowtime, NULL);
		map<unsigned int, SrcCountTime>::iterator it = srcTimeValMap.find(iSrcIp);
		if(srcTimeValMap.end() != it)
		{
			it->second.iTimeVal = nowtime.tv_sec;
			it->second.iCount++;
		}
		else
		{
			SrcCountTime tval;
			tval.iCount = 1;
			tval.iTimeVal = nowtime.tv_sec;
			srcTimeValMap.insert(make_pair(iSrcIp, tval));
		}

		SrcDstParam param;
		param.cDoMain = cDoMain;
		param.iFlag = iFlag;
		param.iDstIp = iDstIp;
		param.iSrcIp = iSrcIp;

		bool berase = true;
		if(srcParamsMap.size() >= PARAMS_MAX)
		{
			berase = false;
			for(it = srcTimeValMap.begin(); it != srcTimeValMap.end();)
			{
				if((nowtime.tv_sec - it->second.iTimeVal) > TIME_OUT )
				{
					srcTimeValMap.erase(it++);
					multimap<unsigned int,SrcDstParam>::iterator mit = srcParamsMap.find(it->first);
					if(mit != srcParamsMap.end())
						srcParamsMap.erase(mit);
					berase = true;
				}
				else
					it++;
			}
		}
		if(berase)
		{
			srcParamsMap.insert(make_pair(iSrcIp,param));
			return 0;
		}
	}

	return -1;
}

int printExtentParams()
{
	int k;
	for(multimap<unsigned int,SrcDstParam>::iterator it = srcParamsMap.begin(); it != srcParamsMap.end();)
	{   
		int n = srcParamsMap.count(it->first);
		if(n > OUT_PUT_NUM)
		{   
			cout << "------------------------------------------------------------------------" << endl;
			for(k = 0;k < n;k++,it++)
				cout << k+1 << " ,SrcIp:"<< it->first << ", DstIp:" << it->second.iDstIp << ", Domain:"<< it->second.cDoMain << ", Flag:" << it->second.iFlag << endl;
		}   
		else
			it++;
	}
}

//g++ insert_ip.cpp insert_ip.h -pthread -fPIC -shared -o libinsert_ip.so
#if 0
int main()
{
	char domain[] = "www.baidu.com";
	int i = 0;

	//// 接口调用，设置PARAMS_MAX为最大插入条目，TIME_OUT为过期时间
	for(int iout = 0; iout < 35; iout++)
		insertParam(domain, i, 0, i);
	for(i;i < 10; i++)
	{
		insertParam(domain, i, 0, i);
		insertParam(domain, i, 0, i+1);
	}
	for(int iout = 0; iout < 35; iout++)
	insertParam(domain, i, 0, i);

	
	//// 根据条件输出打印，设置OUT_PUT_NUM为输出阀值
	init();
	while(1)
	{
		sleep(1);
	}
}
#endif
