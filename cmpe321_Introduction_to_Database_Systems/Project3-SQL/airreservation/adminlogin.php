<?php session_start();
?>
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Air Reservation System</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>

  <link href='style.css' rel='stylesheet'/>
    <div id='index'>
        <br>
        <h1><a href="/airreservation"><font color="#983A00">Airline Reservation System</font></a></h1>
        <div id='user'>
        <br>
        <br>
        <br>
        <br>
    </div>


</head>
<body>

  <form id="employee-form" method="POST">
   
    <div class="row">
      <div class="col-lg-8 col-lg-offset-2">
         <!--<form id="kayit-formu" method="POST" action="register.php">-->
          <div class="col-lg-6 col-md-6 col-sm-12 col-xs-12">
          <input name="username" id="username" type="text" class="form-control field"
                 data-validation="required"
                 data-validation-error-msg-required="This field is obligatory"
                 placeholder="username" >
          </div>
          <div class="col-lg-6 col-md-6 col-sm-12 col-xs-12">
          <input name="password" id="password" type="text" class="form-control field"
                 data-validation="required"
                 data-validation-error-msg-required="This field is obligatory"
                 placeholder="password" ><br>
          </div>
        </form>
      </div>
      
    </div>
<br><br>
    <div class="row">
      <div class="col-sm-5"> </div>
      <div class="col-sm-4">
        <a class="btn btn-primary btn-overlap" 
          style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:90%" onclick="submitAdmin()">Login</a>
      </div>
    </div>

</form>

</body>
</html>

<script>
 

  function submitAdmin(){
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;
    if(username === "ozmenbrn" && password === "123"){
      window.location.replace("http://localhost/airreservation/admin/");
    }
    else{
      alert("Wrong Username or Password");
    }

  }

</script>

 
  

<?php echo '</div>';?> 

   
   