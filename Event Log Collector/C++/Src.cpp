#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <winevt.h>
#include <iostream>
#include <wchar.h>
#include <fstream>
#include <string>
#include <fileapi.h>
#include "pugixml.hpp"
#include <jni.h>
#include "demo.h"
using namespace std;

#pragma comment(lib, "wevtapi.lib")
#define TIMEOUT 10000
JNIEXPORT void JNICALL Java_Final_demo_channel(JNIEnv * env, jobject jobj, jstring js)
{   
	
	cout << "JNI isHello World";
	const char *cjs = (env)->GetStringUTFChars(js, NULL);
	if (NULL == cjs)
	{
		cout << "Can't get String";
		return;
	}
	
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hResults = NULL;
	LPWSTR pBookmarkXML = NULL;
	EVT_HANDLE hBookmark = NULL;
	EVT_HANDLE hEvents[1];
	DWORD dwReturned = 0;
	wchar_t* wstr = NULL;
	EVT_HANDLE hContext = NULL;
	DWORD dwBufferSize = 0;
	DWORD dwBufferUsed = 0;
	DWORD dwPropertyCount = 0;
	jclass classString = env->FindClass("java/lang/String");
	jobjectArray outJNIArray = (env)->NewObjectArray(6, classString, NULL);
	
	char temp[100] = "\0", query[2000] = "\0";
	strcpy_s(temp, 100, cjs);
	strcat_s(query, 2000, "<QueryList>");
	strcat_s(query, 2000, "<Query Path='");
	strcat_s(query, 2000, temp);
	strcat_s(query, 2000, "'>");
	strcat_s(query, 2000, "<Select>*</Select></Query></QueryList>");
	string s(query);
	int wchars_num = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wstr = new wchar_t[wchars_num];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, wstr, wchars_num);
	
	hResults = EvtQuery(NULL, NULL, (LPCWSTR)wstr, EvtQueryChannelPath);
	if (NULL == hResults)
	{
		
		status = GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		string message(messageBuffer, size);
		LocalFree(messageBuffer);
		cout << message;
		goto cleanup;
	}
	else {
		if (status == ERROR_SUCCESS)
		{
			
			status = ERROR_SUCCESS;
			pBookmarkXML = NULL;
			hBookmark = NULL;
			wstr = NULL;
			pugi::xml_document doc;
			pugi::xml_parse_result result;
			//loading the xml file
			result = doc.load_file("S:\\Tomcat\\Task\\bookmark.xml");
			pugi::xml_node root = doc.document_element();
			char temp1[200] = "\0";
			//retrieve the persisted string 
			if (result.status != pugi::status_file_not_found) {
				
				pugi::xml_node tools = doc.child("bookmark");

				for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
				{
					for (pugi::xml_attribute attr = tool.first_child().first_attribute(); attr; attr = attr.next_attribute()) {
						if (strcmp(temp, attr.value()) == 0) {

							strcpy_s(temp1, 200, "<BookmarkList>");
							strcat_s(temp1, 200, "<Bookmark ");


							//for (pugi::xml_node tool1 = tool; tool1; tool1 = tool1.next_sibling())

							for (pugi::xml_attribute attr = tool.first_child().first_attribute(); attr; attr = attr.next_attribute())
							{
								strcat_s(temp1, 200, attr.name());
								strcat_s(temp1, 200, "=\"");
								strcat_s(temp1, 200, attr.value());
								strcat_s(temp1, 200, "\"");
								if (attr.next_attribute() != NULL) {
									strcat_s(temp1, 200, " ");
								}
							}

							strcat_s(temp1, 200, "/></BookmarkList>");
							string s(temp1);
							int wchars_num = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
							wstr = new wchar_t[wchars_num];
							MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, wstr, wchars_num);
							hBookmark = EvtCreateBookmark(LPWSTR(wstr));
							break;
						}

					}
					if (hBookmark != NULL)
					{
						break;
					}
				}
			}

			if (hBookmark != NULL)
			{
				
				EvtSeek(hResults, 0, hBookmark, 0, EvtSeekRelativeToBookmark);
			}
			else if (hBookmark == NULL)
			{
				
				EvtSeek(hResults, 0, 0, TIMEOUT, 0);
			}
			else
			{
				
				status = GetLastError();
				LPSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
				string message(messageBuffer, size);
				LocalFree(messageBuffer);
				cout << message;
				goto cleanup;
			}

			//enumerating the events
			while (true)
			{
				if (!EvtNext(hResults, 1, hEvents, TIMEOUT, 0, &dwReturned))
				{
					if (ERROR_NO_MORE_ITEMS != (status = GetLastError()))
					{
						status = GetLastError();
						LPSTR messageBuffer = nullptr;
						size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
						string message(messageBuffer, size);
						LocalFree(messageBuffer);
						cout << message;
					}
					goto save;

				}
				else {
					status = ERROR_SUCCESS;
					hContext = NULL;
					dwBufferSize = 0;
					dwBufferUsed = 0;
					dwPropertyCount = 0;
					PEVT_VARIANT pRenderedValues = NULL;
					ULONGLONG ullTimeStamp = 0;
					SYSTEMTIME st;
					FILETIME ft;
					LPCWSTR ppValues[] = { L"Event/System/Provider/@Name", L"Event/System/EventID",L"Event/System/Level",L"Event/System/EventRecordID",L"Event/System/TimeCreated/@SystemTime",L"Event/System/Channel" };
					hContext = EvtCreateRenderContext(6, ppValues, EvtRenderContextValues);
					if (NULL == hContext)
					{
						status = GetLastError();
						LPSTR messageBuffer = nullptr;
						size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
						string message(messageBuffer, size);
						LocalFree(messageBuffer);
						cout << message;
						goto cleanup;
					}

					if (!EvtRender(hContext, hEvents[0], EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount))
					{
						if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
						{
							dwBufferSize = dwBufferUsed;
							pRenderedValues = (PEVT_VARIANT)malloc(dwBufferSize);
							if (pRenderedValues)
							{
								EvtRender(hContext, hEvents[0], EvtRenderEventValues, dwBufferSize, pRenderedValues, &dwBufferUsed, &dwPropertyCount);
							}
							else
							{
								wprintf(L"malloc failed\n");
								status = ERROR_OUTOFMEMORY;
								goto cleanup;
							}
						}

						if (ERROR_SUCCESS != (status = GetLastError()))
						{
							wprintf(L"EvtRender failed with %d\n", GetLastError());
							goto cleanup;
						}
					}
					if (ERROR_SUCCESS == (status = GetLastError())) {
						/*fstream f;
						if (strcmp(temp, "Application") == 0) {
							f.open("E:\\Task1\\Application.csv", ios::out | ios::app);
						}
						else if (strcmp(temp, "Setup") == 0) {
							f.open("E:\\Task1\\Setup.csv", ios::out | ios::app);
						}
						else if (strcmp(temp, "Windows PowerShell") == 0) {
							f.open("E:\\Task1\\Windows_Powershell.csv", ios::out | ios::app);
						}
						else if (strcmp(temp, "Kaspersky Event Log") == 0) {
							f.open("E:\\Task1\\Kaspersky_Event_log.csv", ios::out | ios::app);
						}*/
						

						ullTimeStamp = pRenderedValues[4].FileTimeVal;
						ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
						ft.dwLowDateTime = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
						FileTimeToSystemTime(&ft, &st);
						LPSTR st1 = NULL, st2 = NULL;
						LPCWSTR temp1 = (pRenderedValues[0].StringVal);
						LPCWSTR temp2 = (pRenderedValues[5].StringVal);
						int len = WideCharToMultiByte(CP_UTF8, 0, temp1, -1, NULL, 0, 0, 0);
						int len1 = WideCharToMultiByte(CP_UTF8, 0, temp2, -1, NULL, 0, 0, 0);
						if (len > 0)
						{
							st1 = new char[len + 1];
							st2 = new char[len1 + 1];
							if (st1)
							{
								int resLen = WideCharToMultiByte(CP_UTF8, 0, temp1, -1, &st1[0], len, 0, 0);
								int reslen1 = WideCharToMultiByte(CP_UTF8, 0, temp2, -1, &st2[0], len1, 0, 0);
								/*f << st1 << ", " << pRenderedValues[1].UInt16Val << ", " << to_string(pRenderedValues[2].ByteVal) << ", ";
								f << st.wDay << "/" << st.wMonth << "/" << st.wYear << "  ";
								f << st.wHour << ":" << st.wMinute << ":" << st.wSecond << ", " << pRenderedValues[3].UInt64Val << ", " << st2 << "\n";
								f.close();*/
								jclass thisClass = env->GetObjectClass(jobj);
								string s2 = to_string(pRenderedValues[1].UInt16Val);
								string s3 = to_string(pRenderedValues[3].UInt64Val);
								string s = to_string(pRenderedValues[2].ByteVal);
								jstring level = env->NewStringUTF(s.c_str());
								jstring provider = env->NewStringUTF(st1);
    							jstring channel = env->NewStringUTF(st2);
								jstring eventid = env->NewStringUTF(s2.c_str());
								jstring recordid = env->NewStringUTF(s3.c_str());
								string s1;
								char buffer[256];
								sprintf_s(buffer, "%d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
								s1 = buffer;
								jstring date = env->NewStringUTF(s1.c_str());
								
								(env)->SetObjectArrayElement(outJNIArray, 0, provider);
								(env)->SetObjectArrayElement(outJNIArray, 1, eventid);
								(env)->SetObjectArrayElement(outJNIArray, 2, level);
								(env)->SetObjectArrayElement(outJNIArray, 3, date);
								(env)->SetObjectArrayElement(outJNIArray, 4, recordid);
								(env)->SetObjectArrayElement(outJNIArray, 5, channel);
								jmethodID cinsert = env->GetStaticMethodID(thisClass,"insert","([Ljava/lang/String;)V");
								if (cinsert == NULL)
								{
									return;
								}
								env->CallStaticVoidMethod(thisClass, cinsert, outJNIArray);
								delete[] st1;
								delete[] st2;
							}
						}

					}
					EvtClose(hEvents[0]);
					hEvents[0] = NULL;
					if (pRenderedValues)
						free(pRenderedValues);
				}
			}
			//}
			//if (status == ERROR_SUCCESS)
			//{
		save:
			status = ERROR_SUCCESS;
			dwBufferSize = 0;
			dwBufferUsed = 0;
			dwPropertyCount = 0;
			DWORD dwReturned = 0;
			LPWSTR pBookmarkXml = NULL;
			LPWSTR pB = NULL;
			EVT_HANDLE hBookmark = NULL;
			EVT_HANDLE hEvent = NULL;
			LPSTR str = NULL;

			// Seek to the last event in the result set and get the event.
			if (!EvtSeek(hResults, -5, NULL, 0, EvtSeekRelativeToLast))
			{
				status = GetLastError();
				LPSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
				string message(messageBuffer, size);
				LocalFree(messageBuffer);
				cout << message;
				goto cleanup;
			}

			if (!EvtNext(hResults, 1, &hEvent, INFINITE, 0, &dwReturned))
			{
				status = GetLastError();
				LPSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
				string message(messageBuffer, size);
				LocalFree(messageBuffer);
				cout << message;
				goto cleanup;
			}
			//cout << dwReturned << endl;

			hBookmark = EvtCreateBookmark(NULL);
			if (NULL == hBookmark)
			{
				status = GetLastError();
				LPSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
				string message(messageBuffer, size);
				LocalFree(messageBuffer);
				cout << message;
				goto cleanup;
			}

			if (!EvtUpdateBookmark(hBookmark, hEvent))
			{
				status = GetLastError();
				LPSTR messageBuffer = nullptr;
				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
				string message(messageBuffer, size);
				LocalFree(messageBuffer);
				cout << message;
				goto cleanup;
			}

			if (!EvtRender(NULL, hBookmark, EvtRenderBookmark, dwBufferSize, pBookmarkXml, &dwBufferUsed, &dwPropertyCount))
			{

				if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
				{
					dwBufferSize = dwBufferUsed;
					pBookmarkXml = (LPWSTR)malloc(dwBufferSize);
					if (pBookmarkXml)
					{
						EvtRender(NULL, hBookmark, EvtRenderBookmark, dwBufferSize, pBookmarkXml, &dwBufferUsed, &dwPropertyCount);
						//cout << dwPropertyCount;	
					}
					else
					{
						wprintf(L"malloc failed\n");
						status = ERROR_OUTOFMEMORY;
						goto cleanup;
					}
				}

				if (ERROR_SUCCESS != (status = GetLastError()))
				{
					status = GetLastError();
					LPSTR messageBuffer = nullptr;
					size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, status, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
					string message(messageBuffer, size);
					LocalFree(messageBuffer);
					cout << message;
					goto cleanup;
				}
			}

			if (ERROR_SUCCESS == status) {
				pugi::xml_document doc;
				pugi::xml_parse_result result;
				result = doc.load_file("S:\\Tomcat\\Task\\bookmark.xml");
				if (result.status == pugi::status_file_not_found) {
					auto declarationNode = doc.append_child(pugi::node_declaration);
					declarationNode.append_attribute("version") = "1.0";
					auto root = doc.append_child("bookmark");
					doc.save_file("S:\\Tomcat\\Task\\bookmark.xml");
				}
				result = doc.load_file("S:\\Tomcat\\Task\\bookmark.xml");
				pugi::xml_node root = doc.first_child();
				pugi::xml_document doc1;
				int len = WideCharToMultiByte(CP_UTF8, 0, pBookmarkXml, -1, NULL, 0, 0, 0);
				str = new char[len + 1];
				WideCharToMultiByte(CP_UTF8, 0, pBookmarkXml, -1, &str[0], len, 0, 0);
				doc1.load_string(str);
				pugi::xml_node node = doc1.first_child();
				pugi::xml_node nodeChild = root.prepend_child("BookmarkList");
				nodeChild.append_attribute("Direction") = node.attribute("Direction").value();
				node = doc1.first_child().first_child();
				pugi::xml_node nodeChild1 = nodeChild.append_child("Bookmark");
				nodeChild1.append_attribute("IsCurrent") = node.attribute("IsCurrent").value();
				nodeChild1.append_attribute("RecordId") = node.attribute("RecordId").value();
				nodeChild1.append_attribute("Channel") = node.attribute("Channel").value();
				doc.save_file("S:\\Tomcat\\Task\\bookmark.xml");
			}
			delete[] str;
			if (pBookmarkXml)
				free(pBookmarkXml);

			if (hBookmark)
				EvtClose(hBookmark);
		}
	}

cleanup:

	if (hResults)
		EvtClose(hResults);
	delete[] wstr;
	if (NULL != hEvents[0])
		EvtClose(hEvents[0]);
	if (hContext)
		EvtClose(hContext);
	
}


