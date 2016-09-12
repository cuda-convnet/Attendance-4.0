<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("event.getuser");

//Check for a user identifier
$id = isSet($_GET['id']) ? $_GET['id'] : null;
if($id == null) {
	error("Invalid Request", "No user ID specified");
}

//Create the statement
$stmt = $database->prepare(file_get_contents("sql/getUsersEvents.sql"));
//Bind the parameters
$stmt->bind_param("i",$id);
//Execute the statement
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}
//Get the result
$qresult = $stmt->get_result();
//Result
$result = array();
//Process result
while(($row = $qresult->fetch_object()) != NULL) {
	//Create the event object
	$event = [];
	//Set the user data
	$event['id'] = $row->id;
	$event['user'] = $row->user;
	$event['start'] = $row->start;
	$event['end'] = $row->end;
	$event['meta'] = dechex($row->meta);
	$event['isopen'] = $row->isopen;
	$event['name'] = $row->name;
	//Push to the list
	array_push($result, $event);
}
//Return the result
success($result);
?>