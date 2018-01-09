  
<!DOCTYPE html>
<html>
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
        <h1><a href="/airreservation/admin"><font color="#983A00">Airline Reservation System</font></a></h1>
        <div id='user'>
        <br>
        <br>
        <br>
        <br>
    </div>


</head>
<body>

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

	$flightID = $_POST['flightID'];
	$takeof = $_POST['takeof'];
	$landson = $_POST['landson'];
  $year = $_POST['year'];
  $month = $_POST['month'];
  $day = $_POST['day'];
  $takeoftime = $_POST['takeoftime'];
  $traveltime = $_POST['traveltime'];
  $pilotID = $_POST['pilotID'];

	$sql = "insert into flights values($flightID, '$takeof', '$landson', $year, $month, $day, '$takeoftime', $traveltime, $pilotID)";
	$result = $conn->query($sql);

	if($result){
		echo "Successfuly created flight";
    $sql = "insert into airreserve values($flightID, 'free', 0, 0, 0, 0, 0, 0, 0, 0)";
    $conn->query($sql);
	}
	else{
		echo "Error!!!";
	}
}


$conn->close();

?>