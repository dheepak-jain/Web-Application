#include <windows.h>
#include <stdio.h>
#include <winevt.h>
#include <string>
#include <iostream>
#include <wchar.h>
#include <time.h>
#include <jni.h>
#include "Final_demo1.h"
using namespace std;
#pragma comment(lib, "wevtapi.lib")

JNIEXPORT void JNICALL Java_Final1_demo1_channelinfo(JNIEnv * env, jobject jobj)
{

	DWORD status = ERROR_SUCCESS;
	DWORD dwBufferSize = 0;
	DWORD dwBufferUsed = 0;
	DWORD temporary = 0;
	DWORD dwReturned;
	DWORD dwPropertyCount;
	EVT_HANDLE hChannels = NULL;
	EVT_HANDLE log = NULL;
	EVT_HANDLE cha = NULL;
	EVT_HANDLE hresult = NULL;
	EVT_HANDLE hResults = NULL;
	EVT_HANDLE hEvent1 = NULL;
	EVT_HANDLE hEvent2 = NULL;
	EVT_HANDLE hContext = NULL;
	PEVT_VARIANT lDetails = NULL;
	PEVT_VARIANT temp = NULL;
	PEVT_VARIANT pRenderedValues = NULL;
	LPWSTR pBuffer = NULL;
	LPWSTR pTemp = NULL;
	LPSTR tstr = NULL;
	ULONGLONG ullTimeStamp = 0;
	SYSTEMTIME st, st1, st2;
	FILETIME ft, ft1, ft2;
	double seconds;

	jclass classString = env->FindClass("java/lang/String");
	jobjectArray outJNIArray = (env)->NewObjectArray(4, classString, NULL);
	jclass thisClass = env->GetObjectClass(jobj);
	jmethodID cinsert = env->GetStaticMethodID(thisClass, "insert", "([Ljava/lang/String;)V");
	if (cinsert == NULL)
	{
		cout << "Can't get the function";
		return;
	}

	hChannels = EvtOpenChannelEnum(NULL, 0);
	if (NULL == hChannels)
	{
		status = GetLastError();
		cout << status;
		goto cleanup;
	}
	else
	{
		while (true)
		{
			pBuffer = NULL;
			dwBufferSize = 0;
			dwBufferUsed = 0;
			if (!EvtNextChannelPath(hChannels, dwBufferSize, pBuffer, &dwBufferUsed))
			{
				status = GetLastError();
				if (ERROR_NO_MORE_ITEMS == status)
				{
					goto cleanup;
				}
				else if (ERROR_INSUFFICIENT_BUFFER == status)
				{
					dwBufferSize = dwBufferUsed;
					pTemp = (LPWSTR)realloc(pBuffer, dwBufferSize * sizeof(WCHAR));
					if (pTemp)
					{
						pBuffer = pTemp;
						pTemp = NULL;
						if (EvtNextChannelPath(hChannels, dwBufferSize, pBuffer, &dwBufferUsed))
						{
							dwBufferSize = 0;
							temporary = 0;
							dwBufferUsed = 0;
							lDetails = NULL;
							status = ERROR_SUCCESS;
							log = NULL;
							temp = NULL;
						    tstr = NULL;
							LPWSTR tpBuffer = pBuffer;
							int len = WideCharToMultiByte(CP_UTF8, 0, tpBuffer, -1, NULL, 0, 0, 0);
							tstr = new char[len + 1];
							int resLen = WideCharToMultiByte(CP_UTF8, 0, tpBuffer, -1, &tstr[0], len, 0, 0);
							int a[] = { 3,5 };
							//cout << "Channel Name: " << tstr << "\n";
							jstring cname = env->NewStringUTF(tstr);
							(env)->SetObjectArrayElement(outJNIArray, 0,cname);
							log = EvtOpenLog(NULL, pBuffer, EvtOpenChannelPath);
							for (int i = 0; i < 2; i++)
							{
								dwBufferSize = 0;
								dwBufferUsed = 0;
								lDetails = NULL;
								EvtGetLogInfo(log, (EVT_LOG_PROPERTY_ID)a[i], dwBufferSize, lDetails, &dwBufferUsed);
								status = GetLastError();
								if (status == ERROR_INSUFFICIENT_BUFFER) {
									dwBufferSize = dwBufferUsed;
									temp = (PEVT_VARIANT)malloc(dwBufferSize);
									if (temp)
									{
										lDetails = temp;
										temp = NULL;
										EvtGetLogInfo(log, (EVT_LOG_PROPERTY_ID)a[i], dwBufferSize, lDetails, &dwBufferUsed);
										switch (a[i])
										{
										case EvtLogFileSize:
											if (lDetails[0].Type != NULL )
											{
												//std::cout << "Log Size:" << lDetails[0].UInt64Val << "\n";
												string tstr1 = to_string(lDetails[0].UInt64Val);
												jstring lsize = env->NewStringUTF(tstr1.c_str());
												(env)->SetObjectArrayElement(outJNIArray, 1, lsize);
											}
											else
											{
												/*string tstr2 = "NULL";
												jstring lsize = env->NewStringUTF(tstr2.c_str());
												(env)->SetObjectArrayElement(outJNIArray, 1, lsize);
												//std::cout << "Log Size:" << "NULL\n";*/
												status = 1;
												break;
											}
											break;
										case EvtLogNumberOfLogRecords:
											if (lDetails[0].Type != NULL)
											{
												temporary = lDetails[0].UInt64Val;
												string tstr3= to_string(lDetails[0].UInt64Val);
												jstring nof = env->NewStringUTF(tstr3.c_str());
												(env)->SetObjectArrayElement(outJNIArray, 2, nof);
												//std::cout << "Total Number of Records:" << lDetails[0].UInt64Val << "\n";
												status = 0;
												if (temporary == 0)
												{
													string tstr4 = "0";
													jstring wf = env->NewStringUTF(tstr4.c_str());
													(env)->SetObjectArrayElement(outJNIArray, 3, wf);
													env->CallStaticVoidMethod(thisClass, cinsert, outJNIArray);
													//cout << "Workflow: 0 \n";
													status = 1;
													
												}
												
											}
											else
											{
												/*string tstr3 = "NULL";
												jstring nof = env->NewStringUTF(tstr3.c_str());
												(env)->SetObjectArrayElement(outJNIArray, 2, nof);
												string tstr4 = "NULL";
												jstring wf = env->NewStringUTF(tstr4.c_str());
												(env)->SetObjectArrayElement(outJNIArray, 3, wf);
												env->CallStaticVoidMethod(thisClass, cinsert, outJNIArray);
												//std::cout << "Total Number of Records:" << "NULL\n";
												//cout << "Workflow: NULL \n";*/
												status = 1;
												break;
											}
											break;
										}

									}
								}
							}
							//status = GetLastError();
							if (status == ERROR_SUCCESS)
							{
								WCHAR query[2000];
								wcscpy_s(query, 2000, L"<QueryList>");
								wcscat_s(query, 2000, L"<Query Path='");
								wcscat_s(query, 2000, pBuffer);
								wcscat_s(query, 2000, L"'>");
								wcscat_s(query, 2000, L"<Select>*</Select></Query></QueryList>");
								hResults = EvtQuery(NULL, NULL, (LPCWSTR)query, EvtQueryChannelPath);
								if (NULL == hResults)
								{
									goto cleanup;
								}
								else
								{
									pRenderedValues = NULL;
									ullTimeStamp = 0;
									LPCWSTR ppValues[] = { L"Event/System/TimeCreated/@SystemTime" };
									EvtSeek(hResults, 0, NULL, 0, EvtSeekRelativeToFirst);
									EvtNext(hResults, 1, &hEvent1, INFINITE, 0, &dwReturned);
									EvtSeek(hResults, 0, NULL, 0, EvtSeekRelativeToLast);
									EvtNext(hResults, 1, &hEvent2, INFINITE, 0, &dwReturned);
									hContext = EvtCreateRenderContext(1, ppValues, EvtRenderContextValues);
									if (NULL == hContext)
									{
										goto cleanup;
									}
									dwBufferSize = 0;
									dwBufferUsed = 0;
									ullTimeStamp = 0;
									if (!EvtRender(hContext, hEvent1, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount))
									{
										if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
										{
											dwBufferSize = dwBufferUsed;
											pRenderedValues = (PEVT_VARIANT)malloc(dwBufferSize);
											if (pRenderedValues)
											{
												EvtRender(hContext, hEvent1, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount);
											}
											else
											{
												goto cleanup;
											}
										}
										if (ERROR_SUCCESS == (status = GetLastError()))
										{
											ullTimeStamp = pRenderedValues[0].FileTimeVal;
											ft1.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
											ft1.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
											FileTimeToSystemTime(&ft1, &st1);
										}
									}
									dwBufferSize = 0;
									dwBufferUsed = 0;
									ullTimeStamp = 0;
									if (!EvtRender(hContext, hEvent2, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount))
									{
										if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
										{
											dwBufferSize = dwBufferUsed;
											pRenderedValues = (PEVT_VARIANT)malloc(dwBufferSize);
											if (pRenderedValues)
											{
												EvtRender(hContext, hEvent2, EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount);
											}
											else
											{
												goto cleanup;
											}
										}
										if (ERROR_SUCCESS == (status = GetLastError()))
										{

											ullTimeStamp = pRenderedValues[0].FileTimeVal;
											ft2.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
											ft2.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
											FileTimeToSystemTime(&ft2, &st2);
										}
									}
									struct tm t1 = { 0 }, t2 = { 0 };
									t1.tm_hour = st1.wHour;
									t1.tm_min = st1.wMinute;
									t1.tm_sec = st1.wSecond;
									t1.tm_mon = st1.wMonth;
									t1.tm_mday = st1.wDayOfWeek;
									t1.tm_year = st1.wYear - 1900;
									t2.tm_hour = st2.wHour;
									t2.tm_min = st2.wMinute;
									t2.tm_sec = st2.wSecond;
									t2.tm_mon = st2.wMonth;
									t2.tm_mday = st2.wDayOfWeek;
									t2.tm_year = st2.wYear - 1900;
									time_t t3, t4;
									t3 = mktime(&t1);
									t4 = mktime(&t2);
									seconds = difftime(t4, t3);
									if (seconds != 0) {
										if (seconds < 0)
										{
											seconds *= -1;
										}
										//std::cout << "Workflow: ";
										//std::wcout << (temporary / seconds) << "\n";
										double twf = (double)temporary / seconds;
										string tstr4 = to_string(twf);
										cout << tstr4<<"\n";
										jstring wf = env->NewStringUTF(tstr4.c_str());
										(env)->SetObjectArrayElement(outJNIArray, 3, wf);
										env->CallStaticVoidMethod(thisClass, cinsert, outJNIArray);
									}
									else
									{
										std::cout << "Workflow: NULL\n";
									}

								}
							}

						}
					}

				}
			}
			
		}
	}

	
cleanup:
	if (hChannels)
		EvtClose(hChannels);
	if (pBuffer)
		free(pBuffer);
	if (hEvent1)
		EvtClose(hEvent1);
	if (hEvent2)
		EvtClose(hEvent2);
	if (hContext)
		EvtClose(hContext);
	if (temp)
		free(temp);
	if (hresult)
		EvtClose(hresult);
	if (cha)
		EvtClose(cha);
	if (log)
		EvtClose(log);
	if (lDetails)
		free(lDetails);
	if (pTemp)
		free(pTemp);
	if (hResults)
		EvtClose(hResults);

}