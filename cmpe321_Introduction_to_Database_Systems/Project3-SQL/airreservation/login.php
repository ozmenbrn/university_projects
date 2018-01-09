
<?php session_start();
$name='';
$surname='';
$socialNum = '';
//include "conn.php";
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
        <h1><a href="/airreservation/"><font color="#983A00">Airline Reservation System</font></a></h1>
        <div id='user'>
        <br>
        <br>
        <br>
        <br>
    </div>


</head>
<body>

  <section id="employee-form">
  <span id=""></span>
      <div class="container">
        
        <div class="row">
          <div id="contact" class="col-md-2"></div>
          <div id="contact" class="col-md-8">
            <form id="employee-form" method="POST" action="employee-validation.php">
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="name" type="text" id="name" class="form-control field" data-validation="required"
                placeholder="Name" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="surname" type="text" id="surname" class="form-control field" data-validation="required"
                placeholder="Surname" /><br>
              </div>
              

              <div class="row">
                <div class="col-sm-4"> </div>
                <div class="col-sm-4">
                  <input type="submit" id="add-employee"  value="Login" />
                </div>
              </div>
                            
              </div>
            </form>
          </div>
        </div>
      </div>
    </section>
    
</body>

<footer>

</footer>
</html>

<?php echo '</div>';?> 

   
   
    
<!-- Footer --> 



