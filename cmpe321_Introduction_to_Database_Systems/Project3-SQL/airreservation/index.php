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
        <h1>Airline Reservation System</h1>
        <div id='user'>
        <hr>
        <a href="adminlogin.php" style="font-size: 20px"><font color="#2D5772">Admin Login</font></a><br>
        <a href="login.php" style="font-size: 20px"><font color="#2D5772">Employee Login</font></a> 
        <hr> 
    </div>


</head>
<body>

  <section id="main" class="bg-light-gray">
  <div class="row">
    <div class="col-sm-3">
      <a href="flights.php" class="btn btn-primary btn-overlap" 
        style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">FLIGHTS</a>
    </div>
    <div class="col-sm-4">
      <a href="reservations.php" class="btn btn-primary btn-overlap" 
        style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">RESERVATIONS</a>
    </div> 
    <div class="col-sm-3">
      <a href="customers.php" class="btn btn-primary btn-overlap" 
        style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">CUSTOMERS</a>
    </div>
    <div class="col-sm-2">
      <a href="pilots.php" class="btn btn-primary btn-overlap" 
        style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">PILOTS</a>
    </div>
    
  </div>
  </section>

</body>
</html>


<?php echo '</div>';?> 

   
   
    
<!-- Footer --> 


