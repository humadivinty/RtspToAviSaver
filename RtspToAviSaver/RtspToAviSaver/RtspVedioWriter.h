#ifndef RTSPVEDIOWRITER_H
#define RTSPVEDIOWRITER_H

//#include "stdafx.h"
#include <windows.h>

class RtspVedioWriter
{
public:
	RtspVedioWriter(const char* chUrl);
	~RtspVedioWriter();

	void SaveVedio();
	bool StopSave();


	bool StartSaveVedio();
	char* My_GetLastError();

	void SetFileName(const char* chFileName);
private:

	void SetRtspUrl(const char* chUrl);
	bool MakePathExist(const char* chFilePath);
	void SetLastError(const char* errorMsg);

	static unsigned __stdcall ThreadSaveVedio(void * param)
	{
		if (!param)
		{
			return 0;
		}
		RtspVedioWriter* pSaver = (RtspVedioWriter*)param;
		pSaver->SaveVedio();

		return 0;
	}

private:
	char m_chUrlPath[256];
	char m_chFileName[256];
	char m_chErrorMsg[512];
	 bool m_bStop;
	 HANDLE m_hThreadSave;

	 CRITICAL_SECTION m_csCritical;
};

#endif