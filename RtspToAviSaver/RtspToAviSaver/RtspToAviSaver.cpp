// RtspToAviSaver.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "RtspToAviSaver.h"
#include "RtspVedioWriter.h"


// ���ǵ���������һ��ʾ��
//RTSPTOAVISAVER_API int nRtspToAviSaver=0;
//
//// ���ǵ���������һ��ʾ����
//RTSPTOAVISAVER_API int fnRtspToAviSaver(void)
//{
//	return 42;
//}
//
//// �����ѵ�����Ĺ��캯����
//// �й��ඨ�����Ϣ������� RtspToAviSaver.h
//CRtspToAviSaver::CRtspToAviSaver()
//{
//	return;
//}
//RtspVedioWriter* pVedioWriter = NULL;
char g_chFilePath[256] = {};

RTSP_Save_Handle CALLING_CONVENTION RtspSaverInit(const char* RtspUrl)
{
	if (strlen(RtspUrl) < 1)
	{
		return NULL;
	}
	RtspVedioWriter* pVedioWriter = new RtspVedioWriter(RtspUrl);
	if (!pVedioWriter)
	{
		return NULL;
	}
	else
	{
		return (RTSP_Save_Handle)pVedioWriter;
	}
}

bool CALLING_CONVENTION RtspSaverSetFileName(RTSP_Save_Handle playHandle, const char* FileName)
{
	if (!playHandle)
	{
		return false;
	}
	else
	{
		RtspVedioWriter* pVedioWriter = (RtspVedioWriter*)playHandle;
		pVedioWriter->SetFileName(FileName);
		pVedioWriter = NULL;
		return true;
	}
}

bool CALLING_CONVENTION RtspSaverStartSave(RTSP_Save_Handle playHandle)
{
	if (!playHandle)
	{
		return false;
	}
	else
	{
		RtspVedioWriter* pVedioWriter = (RtspVedioWriter*)playHandle;
		return pVedioWriter->StartSaveVedio();
	}
}

bool CALLING_CONVENTION RtspSaverStopSave(RTSP_Save_Handle playHandle)
{
	if (!playHandle)
	{
		return false;
	}
	else
	{
		RtspVedioWriter* pVedioWriter = (RtspVedioWriter*)playHandle;
		return pVedioWriter->StopSave();
	}
}

char* CALLING_CONVENTION RtspSaverGetLastError(RTSP_Save_Handle playHandle)
{
	if (!playHandle)
	{
		return "RtspSaver is init failed. ";
	}
	else
	{
		RtspVedioWriter* pVedioWriter = (RtspVedioWriter*)playHandle;
		return pVedioWriter->My_GetLastError();
	}
}

RTSPTOAVISAVER_API bool CALLING_CONVENTION RtspSaverDestroyHandle(RTSP_Save_Handle playHandle)
{
	if (!playHandle)
	{
		return "RtspSaver is init failed. ";
	}
	else
	{
		RtspVedioWriter* pVedioWriter = (RtspVedioWriter*)playHandle;
		delete pVedioWriter;
		pVedioWriter = NULL;
		return true;
	}
}
