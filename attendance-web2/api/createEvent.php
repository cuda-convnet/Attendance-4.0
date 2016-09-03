<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("event.create");

//Arguments
$user =  isSet($_GET['id'])    ? $_GET['id']    : null;
$start = isSet($_GET['start']) ? $_GET['start'] : null;
$end =   isSet($_GET['end'])   ? $_GET['end']   : null;

//Check for error
if($user == null || $start == null || $end == null) {
	error("Missing required parameters");
}

//Create statement
$stmt = $database->prepare("INSERT INTO calendar (start,end,user,meta) VALUES (?,?,?,b'00000100')");
//Bind parameters
$stmt->bind_param("iii", $start, $end, $user);
//Execute the query
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}

//Finished
success("Created new calendar event");

?>