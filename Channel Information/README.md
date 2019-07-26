NOTE:

The Java Folder consists of jsp, servlet, java class and dll file(s).

The C++ folder consists of native windows c++ code for getting the channel information.


Workflow

When the channelInfo.jsp file is executed the jsp file calls the channelInfoservlet which will call the demo1.java which loads 
the jni_info.dll . The dynamic link library(dll) contains the code for Java Native Interface (JNI). The JNI enables the java 
application to run the c++ code to get the channel information. The native c++ code gets the channel information using Windows API's
and stores the information in a database created using postgreSql. Then the information that is stored in the database is retrived and 
converted to json and returned to the jsp file. In the jsp file I am using DataTables to display the returned data.

Reference

DataTables: https://datatables.net/

Windows API: https://docs.microsoft.com/en-us/windows/win32/apiindex/windows-api-list
