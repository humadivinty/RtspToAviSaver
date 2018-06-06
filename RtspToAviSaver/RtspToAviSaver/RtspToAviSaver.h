// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� RTSPTOAVISAVER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// RTSPTOAVISAVER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef RTSPTOAVISAVER_EXPORTS
#define RTSPTOAVISAVER_API __declspec(dllexport)
#else
#define RTSPTOAVISAVER_API __declspec(dllimport)
#endif

#define CALLING_CONVENTION __stdcall
// �����Ǵ� RtspToAviSaver.dll ������
//class RTSPTOAVISAVER_API CRtspToAviSaver {
//public:
//	CRtspToAviSaver(void);
//	// TODO: �ڴ�������ķ�����
//};
//
//extern RTSPTOAVISAVER_API int nRtspToAviSaver;
//
//RTSPTOAVISAVER_API int fnRtspToAviSaver(void);

#define  RTSP_Save_Handle void*

//RTSP_Save_Handle __cdecl RtspSaverInit(const char* RtspUrl);
//
//bool __cdecl RtspSaverSetFileName(RTSP_Save_Handle playHandle, const char* FileName);
//
//bool  __cdecl RtspSaverStartSave(RTSP_Save_Handle playHandle);
//
//bool   __cdecl RtspSaverStopSave(RTSP_Save_Handle playHandle);
//
//char* __cdecl RtspSaverGetLastError(RTSP_Save_Handle playHandle);

RTSPTOAVISAVER_API RTSP_Save_Handle CALLING_CONVENTION RtspSaverInit(const char* RtspUrl);

RTSPTOAVISAVER_API bool CALLING_CONVENTION RtspSaverSetFileName(RTSP_Save_Handle playHandle, const char* FileName);

RTSPTOAVISAVER_API bool CALLING_CONVENTION  RtspSaverStartSave(RTSP_Save_Handle playHandle);

RTSPTOAVISAVER_API bool CALLING_CONVENTION   RtspSaverStopSave(RTSP_Save_Handle playHandle);

RTSPTOAVISAVER_API char* CALLING_CONVENTION RtspSaverGetLastError(RTSP_Save_Handle playHandle);

RTSPTOAVISAVER_API bool  CALLING_CONVENTION RtspSaverDestroyHandle(RTSP_Save_Handle playHandle);