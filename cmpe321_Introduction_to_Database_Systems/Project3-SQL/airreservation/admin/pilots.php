
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
        <h1><a href="/airreservation/admin"><font color="#983A00">Airline Reservation System</font></a></h1>
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
        $sql = "SELECT * FROM pilots";
        $result = $conn->query($sql);
          
        ?>
          <?php 
           while($row = mysqli_fetch_assoc($result)) {
                ?> <a href = "deletepilot.php?id=<?php echo $row["pilotID"]; ?>"><img src = "/airreservation/img/delete.png" width="2%"></a> 
                <a href = "updatepilotpage.php?id=<?php echo $row["pilotID"]; ?>"><img src = "/airreservation/img/edit.png" width="2%"></a> <?php
                echo "pilotID: " . $row["pilotID"]. "  |  Name: " . $row["pilotName"]. "  | Surname: " . $row["pilotSurname"]. " | isFree: " . $row["isFree"]. " <br>";
            }
         } 
        $conn->close();
    ?>
  </div>
 <hr>
  <section id="main" class="bg-light-gray">
  <div class="row">  
  <div class="col-sm-5">
    </div>
    <div class="col-sm-2">
      <a href="addpilot.php" class="btn btn-primary btn-overlap" 
        style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:100%">Add Pilot</a>
    </div>
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

   