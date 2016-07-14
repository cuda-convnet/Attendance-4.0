<?php
//Include the API
include("api.php");

//Check the arguments
if(!isSet($_GET['id'])) {
	die(json_encode(array("result"=>"error","message"=>"User ID not specified"), JSON_PRETTY_PRINT));
}
if(!isSet($_GET['keepcredits'])) {
	die(json_encode(array("result"=>"error","message"=>"Keep Credits not specified"), JSON_PRETTY_PRINT));
}
if(!isSet($_GET['note'])) {
	die(json_encode(array("result"=>"error","message"=>"Note not specified"), JSON_PRETTY_PRINT));
}

//Create the prepared statement
$statement = $database->prepare("UPDATE users SET signedin=0 WHERE id=?");
$statement->bind_param("i",intval($_GET['id']));
$statement->execute();

//Get the users last calendar event
$lastEvent = getLastCalendarEvent(intval($_GET['id']));

//Check if event is real
if(!$lastEvent) {
	die(json_encode(array("result"=>"error","message"=>"No calendar events found for user"), JSON_PRETTY_PRINT));
}
//Check if event is ended
if($lastEvent['end'] != 0) {
	die(json_encode(array("result"=>"error","message"=>"User is not signed in"), JSON_PRETTY_PRINT));
}

//Decode the event special information
$esi = json_decode($lastEvent['special'], true);

//Check weather credits are to be kept or removed
if($_GET['keepcredits'] == "true") {
	//Create the admin event
	$admin = createAdminEvent(TYPE_SIGNOUT_MAN1, "" . $lastEvent['id'], $_SERVER['PHP_AUTH_USER'], $_GET['note']);
	//Add the admin event ID to the special data
	array_push($esi['adminEventLog'],$admin);
	//Update the event
	updateCalendarEvent($lastEvent['id'], $lastEvent['userid'], $lastEvent['start'], time(), json_encode($esi));
} else {
	//Create the admin event
	$admin = createAdminEvent(TYPE_SIGNOUT_MAN2, "" . $lastEvent->id, $_SERVER['PHP_AUTH_USER'], $_GET['note']);
	//Add the admin event ID to the special data
	array_push($esi['adminEventLog'],$admin);
	//Update the event
	updateCalendarEvent($lastEvent['id'], $lastEvent['userid'], $lastEvent['start'], $lastEvent['start'], json_encode($esi));
}

//Encode and return the result
die(json_encode(array("result"=>"success"), JSON_PRETTY_PRINT));
?>