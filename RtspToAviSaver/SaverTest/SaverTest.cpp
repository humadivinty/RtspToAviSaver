// SaverTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../RtspToAviSaver/RtspToAviSaver.h"
#include <stdio.h>
#include <Windows.h>

#pragma  comment(lib, "../debug/RtspToAviSaver.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	RTSP_Save_Handle pHanle = RtspSaverInit("rtsp://172.18.51.59:554/h264ESVideoTest");

	if (pHanle)
	{
		printf("init success.\n");
		char szFileName[MAX_PATH] = {0};
		sprintf_s(szFileName, sizeof(szFileName), "G:\\VS2012Project\\RtspToAviSaver\\Debug\\%ld.avi", GetTickCount());
		RtspSaverSetFileName(pHanle, szFileName);
		RtspSaverStartSave(pHanle);
	}
	else
	{
		printf("init failed.\n");
	}

	 getchar();
	 if (pHanle)
	 {
		 RtspSaverStopSave(pHanle);
		 RtspSaverGetLastError(pHanle);
		 printf("begin to destroy handle.\n");
		 RtspSaverDestroyHandle(pHanle);
		 printf("finish to destroy handle.\n");
		 pHanle = NULL;
	 }
	getchar();
	return 0;
}

