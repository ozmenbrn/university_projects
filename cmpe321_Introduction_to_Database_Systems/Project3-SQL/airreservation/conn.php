<?php


$user = 'root';
$pass = '';
$db = 'airreservation';

//$conn = new mysqli('localhost', $user, $pass, $db) or die("Unable to connect");
$conn = mysql_connect('localhost', $user, $pass, $db) or die("Unable to connect");