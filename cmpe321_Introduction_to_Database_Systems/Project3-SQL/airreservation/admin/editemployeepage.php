<?php session_start();
$name='';
$surname='';
$socialNum = $_GET['id'];
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
        <h1><a href="/airreservation/admin"><font color="#983A00">Airline Reservation System</font></a></h1>
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
            <form id="employee-form" method="POST" action="editemployee.php?id=<?php echo $socialNum ?>">
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="socialNum" type="text" id="socialNum" class="form-control field" 
                 data-validation="required, socialNum"
                 data-validation-error-msg-required="Primary key"
                 data-validation-error-msg-email="primary Key required"
                 placeholder="Socail Number" ><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="name" type="text" id="name" class="form-control field" data-validation="required"
                placeholder="Name" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="surname" type="text" id="surname" class="form-control field" data-validation="required"
                placeholder="Surname" /><br>
              </div>
              
              <div class="col-lg-9 col-md-12 col-sm-12 col-xs-12">
                <input type="submit" id="add-employee" class="btn btn-md center-block" value="Update" />
                            
              </div>
            </form>
          </div>
        </div>
      </div>
    </section>
    
</body>

<footer>
  <section id="footer" class="bg-light-gray">
  <div class="row">
    <div class="col-sm-8"> </div> 
    <div class="col-sm-4">
      <a href="/airreservation" style="font-family: Smart Kid; font-size: 28px; padding-top: 2px; padding-bottom: 5px; margin-bottom: 10px; line-height:0%"> 
      <font color="#0E1D27">Logout</font></a>
    </div>
    </div>
  </section>

</footer>
</html>



<?php echo '</div>';?> 

   
   
    
<!-- Footer --> 


