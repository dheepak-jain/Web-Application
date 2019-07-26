
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
    </head>
   <script>  
  $(document).ready(function (){
      $('#ok').click(function(){
    var cname=$("#cname").val();
    if (cname==null || cname==""){  
    alert("Channel Name can't be blank");    
    }
     else{
        $.ajax({
           type:'POST',
           url:'channelServlet',
          data:{'channel_name':cname},
          success:function(data) {
            var t= $("#mytbl").DataTable();
            $.each(data,function(a,b) {          
                t.row.add([
                   data[a].vprovider,
                   data[a].veventid,
                   data[a].vlevel,
                   data[a].vcreatedtime,
                   data[a].vrecordid,
                   data[a].vchannel,
               ]);
           });
               t.draw();
			   $("#mtable").show();
         },
          error:function(){
              alert('Error');
          }    
          });   
      }
        });
    });
</script> 
    <body>
  <div name="add">
         Channel name: <input id="cname" type = "text" name = "channel_name" />
         <br>
         <button id="ok">OK</button>
</div>   
<div id="mtable"> 
        <br><br>
        <table id="mytbl">
           <thead>
                <tr>
                    <th>Provider Name</th>
                    <th>Event ID</th>
                    <th>Level</th>
                    <th>Time Created</th>
                    <th>Record ID</th>
                    <th>Channel</th>
                </tr>
            </thead>         
        </table>
</div>		
    </body>
</html>
