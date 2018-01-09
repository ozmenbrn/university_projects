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

  <form id="main" class="bg-light-gray">
   
    <br> <br><br><br>

    <div class="row">
      <div class="col-sm-5"> </div>
      <div class="col-sm-4">
        <a href="login.php" class="btn btn-primary btn-overlap" 
          style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">Turn Back</a>
      </div>
    </div>

</form>

</body>
</html>

   
   <?php 

$servername = "localhost";
$user = "root";
$pass = "";
$db = "airreservation";

//$conn = new mysqli('localhost', $user, $pass, $db) or die("Unable to connect");
$conn = new mysqli($servername, $user, $pass, $db);

if($conn->connect_error){
  die("Connection Failed: " . $conn->connect_error);
}
else{

  $username = $_POST['name'];
  $password = $_POST['surname'];

  $sql = "SELECT * FROM employees WHERE name='$username' AND surname='$password'";
  $result = $conn->query($sql);

  if($result->num_rows > 0){ 
    
    ob_start(); // ensures anything dumped out will be caught

// do stuff here
    $url = 'http://localhost/airreservation/employee/'; // this can be set based on whatever

    // clear out the output buffer
    while (ob_get_status()) 
    {
        ob_end_clean();
    }

// no redirect
header( "Location: $url" );
    
  }
  else{
    echo "Wrong username or password";
  }
}


$conn->close();

?>
   
    
<!-- Footer --> 


<?php echo '</div>';?> 

