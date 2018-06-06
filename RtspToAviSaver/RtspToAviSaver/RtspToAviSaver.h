// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 RTSPTOAVISAVER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// RTSPTOAVISAVER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef RTSPTOAVISAVER_EXPORTS
#define RTSPTOAVISAVER_API __declspec(dllexport)
#else
#define RTSPTOAVISAVER_API __declspec(dllimport)
#endif

#define CALLING_CONVENTION __stdcall
// 此类是从 RtspToAviSaver.dll 导出的
//class RTSPTOAVISAVER_API CRtspToAviSaver {
//public:
//	CRtspToAviSaver(void);
//	// TODO: 在此添加您的方法。
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