#include "stdafx.h"
#include "RtspVedioWriter.h"

#ifdef __cplusplus 
extern "C" {
#endif

#include "include/libavcodec/avcodec.h"
#include "include/libavdevice/avdevice.h"
#include "include/libavformat/avformat.h" 
#include "include/libavfilter/avfilter.h"
#include "include/libavutil/avutil.h"
#include "include/libswscale/swscale.h"

#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <math.h>
#include <windows.h>
#include <process.h>


#ifdef __cplusplus 
}
#endif
#include <string>

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "lib/avcodec.lib")
//#pragma comment(lib, "lib/avdevice.lib")
#pragma comment(lib, "lib/avformat.lib")
//#pragma comment(lib, "lib/avfilter.lib")
#pragma comment(lib, "lib/avutil.lib")
//#pragma comment(lib, "lib/swscale.lib")
//#pragma comment(lib, "lib/SDL2.lib")
//#pragma comment(lib, "lib/SDL2main.lib")
//#pragma comment(lib, "lib/postproc.lib")
//#pragma comment(lib, "lib/swresample.lib")


RtspVedioWriter::RtspVedioWriter(const char* chUrl)
	:m_bStop(false),
	m_hThreadSave(NULL)
{
	strcpy_s(m_chUrlPath, chUrl);
	memset(m_chErrorMsg, '\0', 512);
	memset(m_chFileName, '\0', 256);

	InitializeCriticalSection(&m_csCritical);
}

RtspVedioWriter::~RtspVedioWriter()
{
	StopSave();
	DeleteCriticalSection(&m_csCritical);
}

inline void copyContex(AVStream* SrcStream, AVCodecContext** DestContex)
{
	(*DestContex)->bit_rate = 400000;
	(*DestContex)->codec_id = SrcStream->codec->codec_id;
	(*DestContex)->codec_type = SrcStream->codec->codec_type;
	//(*DestContex)->time_base.num = SrcStream->time_base.num;
	//(*DestContex)->time_base.den = SrcStream->time_base.den;
	fprintf(stderr, "time_base.num = %d time_base.den = %d\n", (*DestContex)->time_base.num, (*DestContex)->time_base.den);
	(*DestContex)->width = SrcStream->codec->width;
	(*DestContex)->height = SrcStream->codec->height;
	(*DestContex)->pix_fmt = SrcStream->codec->pix_fmt;
	printf("%d %d %d", (*DestContex)->width, (*DestContex)->height, (*DestContex)->pix_fmt);
	(*DestContex)->flags = SrcStream->codec->flags;
	(*DestContex)->flags |= CODEC_FLAG_GLOBAL_HEADER;
	(*DestContex)->me_range = SrcStream->codec->me_range;
	(*DestContex)->max_qdiff = SrcStream->codec->max_qdiff;

	(*DestContex)->qmin = SrcStream->codec->qmin;
	(*DestContex)->qmax = SrcStream->codec->qmax;

	(*DestContex)->qcompress = SrcStream->codec->qcompress;

	//(*DestContex)->codec_id = AV_CODEC_ID_H264;
	(*DestContex)->time_base.num = 1;
	(*DestContex)->time_base.den = 30;
}

void RtspVedioWriter::SaveVedio()
{
	char chUrl[256] = {0};
	char chFileName[256] = {0};
	EnterCriticalSection(&m_csCritical);
	strcpy_s(chUrl, m_chUrlPath);
	strcpy_s(chFileName, m_chFileName);
	LeaveCriticalSection(&m_csCritical);

	if (strlen(chUrl) < 1)
	{
		fprintf(stderr, "The url is null\n");
		SetLastError("The url is null\n");
		return;
	}

	MakePathExist(chFileName);

	AVFormatContext *i_fmt_ctx = NULL;
	
	AVFormatContext *o_fmt_ctx = NULL;
	AVStream *o_video_stream = NULL;

	avcodec_register_all();
	av_register_all();
	avformat_network_init();
	
	if (avformat_open_input(&i_fmt_ctx, chUrl, NULL, NULL) != 0)
	{
		fprintf(stderr, "could not open input file\n");
		SetLastError("could not open input file\n");
		return;
	}
	else
	{
		printf("avformat_open_input success.\n");
	}

	if (avformat_find_stream_info(i_fmt_ctx, NULL) < 0)
	{
		fprintf(stderr, "could not find stream info\n");
		SetLastError("could not find stream info\n");
		return ;
	}
	else
	{
		printf("avformat_find_stream_info success.\n");
	}

	AVStream *i_video_stream = NULL;
	for (unsigned i = 0; i < i_fmt_ctx->nb_streams; i++)
	{
		if (i_fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			i_video_stream = i_fmt_ctx->streams[i];
			printf("video stream id = %d\n",i );
			break;
		}
	}
	if (i_video_stream == NULL)
	{
		fprintf(stderr, "didn't find any video stream\n");
		SetLastError("didn't find any video stream\n");
		return ;
	}

	int iRet = avformat_alloc_output_context2(&o_fmt_ctx, NULL, NULL, chFileName);
	if (iRet < 0)
	{
		printf("avformat_alloc_output_context2 failed.\n");
		return;
	}
	/*
    * since all input files are supposed to be identical (framerate, dimension, color format, ...)
    * we can safely set output codec values from first input file
    */
	o_video_stream = avformat_new_stream(o_fmt_ctx, NULL);
	{
		AVCodecContext* pContex = NULL;
		pContex = o_video_stream->codec;
		copyContex(i_video_stream, &pContex);

		//pContex->bit_rate = 400000;
		//pContex->codec_id = i_video_stream->codec->codec_id;
		//pContex->codec_type = i_video_stream->codec->codec_type;
		//pContex->time_base.num = i_video_stream->time_base.num;
		//pContex->time_base.den = i_video_stream->time_base.den;
		//fprintf(stderr, "time_base.num = %d time_base.den = %d\n", pContex->time_base.num, pContex->time_base.den);
		//pContex->width = i_video_stream->codec->width;
		//pContex->height = i_video_stream->codec->height;
		//pContex->pix_fmt = i_video_stream->codec->pix_fmt;
		//printf("%d %d %d", pContex->width, pContex->height, pContex->pix_fmt);
		//pContex->flags = i_video_stream->codec->flags;
		//pContex->flags |= CODEC_FLAG_GLOBAL_HEADER;
		//pContex->me_range = i_video_stream->codec->me_range;
		//pContex->max_qdiff = i_video_stream->codec->max_qdiff;

		//pContex->qmin = i_video_stream->codec->qmin;
		//pContex->qmax = i_video_stream->codec->qmax;

		//pContex->qcompress = i_video_stream->codec->qcompress;
	}
	avio_open(&o_fmt_ctx->pb, chFileName, AVIO_FLAG_WRITE);
	avformat_write_header(o_fmt_ctx, NULL);

	int iLast_pts = 0;
	int iLast_dts = 0;

	int64_t ipts = 0, idts = 0;
	bool bExit = false;
	int inum = 0;
	while (!bExit)
	{
		EnterCriticalSection(&m_csCritical);
		bExit = m_bStop;
		LeaveCriticalSection(&m_csCritical);

		AVPacket i_pkt;
		av_init_packet(&i_pkt);
		i_pkt.size = 0;
		i_pkt.data = NULL;
		if (av_read_frame(i_fmt_ctx, &i_pkt) < 0)
		{
			break;
		}
		/*
        * pts and dts should increase monotonically
        * pts should be >= dts
        */

		if (i_pkt.flags &AV_PKT_FLAG_KEY)
		{
			i_pkt.flags |= AV_PKT_FLAG_KEY;
		}
		else
		{
			i_pkt.flags = 0;
		}
		i_pkt.dts = i_pkt.pts = AV_NOPTS_VALUE;

		ipts = i_pkt.pts;
		i_pkt.pts += iLast_pts;

		idts = i_pkt.dts;
		i_pkt.dts += iLast_dts;
		i_pkt.stream_index  = 0;
		printf("frame %d\n", inum++);

		av_interleaved_write_frame(o_fmt_ctx, &i_pkt);

		Sleep(10);
	}
	iLast_dts += idts;
	iLast_pts += ipts;

	avformat_close_input(&i_fmt_ctx);
	av_write_trailer(o_fmt_ctx);

	avcodec_close(o_fmt_ctx->streams[0]->codec);
	av_freep(&o_fmt_ctx->streams[0]->codec);
	av_freep(&o_fmt_ctx->streams[0]);

	avio_close(o_fmt_ctx->pb);
	av_freep(o_fmt_ctx);

	return ;
}

bool RtspVedioWriter::StopSave()
{
	EnterCriticalSection(&m_csCritical);
	m_bStop = true;
	LeaveCriticalSection(&m_csCritical);

	DWORD dwRet = -1;
	MSG msg;
	while (NULL != m_hThreadSave && WAIT_OBJECT_0 != dwRet) // INFINITE
	{
		dwRet = MsgWaitForMultipleObjects(1, &m_hThreadSave, FALSE, 100, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0 + 1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	if (m_hThreadSave)
	{
		CloseHandle(m_hThreadSave);
		m_hThreadSave = NULL;
	}
	//if(WaitForSingleObject(m_hThreadSave, INFINITE) == WAIT_OBJECT_0) 
	//{
	//	CloseHandle(m_hThreadSave);
	//	m_hThreadSave = NULL;
	//}
	return true;
}

bool RtspVedioWriter::StartSaveVedio()
{
	//SetFileName(chFileName);
	if (m_hThreadSave)
	{
		StopSave();
	}
	unsigned  uiThreadID = 0;  

	if (!m_hThreadSave)
	{
		m_hThreadSave = (HANDLE)_beginthreadex( NULL,         // security  
			0,            // stack size  
			ThreadSaveVedio,  
			this,           // arg list  
			0,  //CREATE_SUSPENDED so we can later call ResumeThread()  
			&uiThreadID );
		return true;
	}
	else
	{
		return false;
	}	
}

char* RtspVedioWriter::My_GetLastError()
{
	static char chError[512] = {0};
	EnterCriticalSection(&m_csCritical);
	strcpy_s(chError, m_chErrorMsg);
	LeaveCriticalSection(&m_csCritical);
	return chError;
}

void RtspVedioWriter::SetRtspUrl(const char* chUrl)
{
	EnterCriticalSection(&m_csCritical);
	strcpy_s(m_chUrlPath, chUrl);
	LeaveCriticalSection(&m_csCritical);
}

void RtspVedioWriter::SetFileName(const char* chFileName)
{
	EnterCriticalSection(&m_csCritical);
	if (!chFileName)
	{
		sprintf_s(m_chFileName, ".\\Vedio\\");
	}
	else
	{
		strcpy_s(m_chFileName, chFileName);
	}
	LeaveCriticalSection(&m_csCritical);
}

bool RtspVedioWriter::MakePathExist(const char* chFilePath)
{
	if (NULL == chFilePath)
	{
		//WriteLog("the path is null ,Create Dir failed.");
		return false;
	}
	std::string tempFile(chFilePath);
	size_t iPosition = tempFile.rfind("\\");
	std::string tempDir = tempFile.substr(0, iPosition + 1);
	if (MakeSureDirectoryPathExists(tempDir.c_str()))
	{
		return true;
	}
	else
	{
		//WriteLog("Create Dir failed.");
		return false;
	}
}

void RtspVedioWriter::SetLastError(const char* errorMsg)
{
	EnterCriticalSection(&m_csCritical);
	strcpy_s(m_chErrorMsg, errorMsg);
	LeaveCriticalSection(&m_csCritical);
}
