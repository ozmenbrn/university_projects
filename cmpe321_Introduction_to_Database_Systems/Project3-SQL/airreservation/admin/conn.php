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


$conn->close();
?>