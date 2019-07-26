<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <style>
            table, th, td {
                 border: 1px solid black;
                 border-collapse: collapse;
                }
                #mtable{
                    display:none;
                }   
        </style>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
        <link rel="stylesheet" type="text/css" href="//cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css"/>
        <script type="text/javascript" src="//cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
         <script>
             $(document).ready(function(){
                 $.ajax({
                     type:'POST',
                     url:'channelInfoServlet',
                     success:function(data){
                      var t= $("#mytbl").DataTable();
               $.each(data,function(a,b) {          
               t.row.add([
                     data[a].cname,
                     data[a].lsize,
                     data[a].nor,
                     data[a].wf,
               ]);
           });
               t.draw();
               $("#ab").hide();
               $("#mtable").show();   
                     },
                     error:function(){
                         alert("Error")
                     }
                     
                 });
             });
         </script>    
    </head>
    <body>
        <h1>Channel Information</h1>
        <br><br>
        <div id="ab">
            <h3>The Data is loading. It might take some while. Please Wait</h3>
        </div>
        <div id="mtable">
        <table id="mytbl">
            <thead>
                <tr>
                    <th>Channel Name</th>
                    <th>Log Size(in KB)</th>
                    <th>No of Records</th>
                    <th>Workflow (in sec)</th>
                </tr>
            </thead>  
        </table> 
<div> 
    </body>
</html>
