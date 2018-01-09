
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
        <hr>
    </div>


</head>
<body>
  <div id= 'menu'>
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
        $sql = "SELECT * FROM airreserve WHERE planeID = " . $_GET['id'];
        $result = $conn->query($sql);
          
        ?>
          <?php 
           while($row = mysqli_fetch_assoc($result)) {
                echo "1a: " . $row["1a"]. " | 1b: " . $row["1b"]. " 
                | 1c: " . $row["1c"]. " | 1d: " . $row["1d"]. " | 2a: " . $row["2a"]. " | 2b: " . $row["2b"]. " 
                | 2c: " . $row["2c"]. "  <br>";
            }
         } 
        $conn->close();
    ?>
 
  <section id="main" class="bg-light-gray">
  <div class="row">  
  </div>
</section>

<section id="main" class="bg-light-gray">
  <div class="row">
    <div class="col-sm-5">
    </div>
    
    
  </div>
  </section>

</body>

<footer>
 

</footer>
</html>


<?php echo '</div>';?> 

   