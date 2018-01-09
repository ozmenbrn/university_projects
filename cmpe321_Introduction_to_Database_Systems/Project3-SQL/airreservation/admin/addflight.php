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
        <h1><a href="/airreservation/admin"><font color="#983A00">Airline Reservation System</font></a></h1>
        <div id='user'>
        <br>
        <br>
        <br>
        
    </div>


</head>
<body>

  <section id="flight-form">
  <span id=""></span>
      <div class="container">
        
        <div class="row">
          <div id="contact" class="col-md-2"></div>
          <div id="contact" class="col-md-8">
            <form id="flight-form" method="POST" action="actionflight.php">
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="flightID" type="text" id="flightID" class="form-control field" 
                 data-validation="required, socialNum"
                 data-validation-error-msg-required="Primary key"
                 data-validation-error-msg-email="primary Key required"
                 placeholder="flight ID" ><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="takeof" type="text" id="takeof" class="form-control field" data-validation="required"
                placeholder="takeof" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="landson" type="text" id="landson" class="form-control field" data-validation="required"
                placeholder="landson" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="year" type="text" id="year" class="form-control field" data-validation="required"
                placeholder="year" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="month" type="text" id="month" class="form-control field" data-validation="required"
                placeholder="month" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="day" type="text" id="day" class="form-control field" data-validation="required"
                placeholder="day" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="takeoftime" type="text" id="takeofTime" class="form-control field" data-validation="required"
                placeholder="takeoftime" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="traveltime" type="text" id="travelTime(min)" class="form-control field" data-validation="required"
                placeholder="traveltime" /><br>
              </div>
              <div class="col-lg-9 col-md-6 col-sm-12 col-xs-12">
                <input name="pilotID" type="text" id="pilotID" class="form-control field" data-validation="required"
                placeholder="pilotID" /><br>
              </div>
              
              <div class="col-lg-9 col-md-12 col-sm-12 col-xs-12">
                <input type="submit" id="add-flight" class="btn btn-md center-block" value="Add" />
                            
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


<script>
  function addEmployee(){
   
    var name = document.getElementById("name").value;
    var surname = document.getElementById("surname").value;
    var socialNum = document.getElementById("socialNum").value;
    
   
  }

</script>

<?php echo '</div>';?> 

   
   
    
<!-- Footer --> 


