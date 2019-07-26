NOTE:

The Java Folder consists of jsp, servlet, java class and dll file(s).

The C++ folder consists of native windows c++ code for getting the channel information.

Workflow

When the channel.jsp file is executed we need to enter the name of the channel from which you want to retrieve the logs, the jsp file 
will pass the channel value to the channelservlet which will then call the demo.java with the value that was passed to it.
The demo.java loads the jni.dll . The dynamic link library(dll) contains the code for Java Native Interface (JNI). The JNI 
enables the java application to run the c++ code to get the log information. The native c++ code uses Windows API's to open the respective 
channel and then it checks the xml file whether there is a bookmark for the channel. If it is present then it retrives the bookmark
using pugixml and reads the logs that are recorded after the bookmarked record. If the bookmark is not present then it reads the logs 
from the first. The read logs are stored the in a database created using postgreSql. After this the last record is persisted using pugixml
for future reference. Then the information that is stored in the database is retrived and converted to json and returned to the jsp file. 
In the jsp file I am using DataTables to display the returned data.

Reference:

pugixml: https://pugixml.org/

Windows Event Log: https://docs.microsoft.com/en-us/windows/win32/wes/windows-event-log

DataTables: https://datatables.net/
