<?php
//Include the API
include("api.php");

//Check the command
if(!isSet($_GET['id'])) {
	die("Bad request");
}

//Get the user
$user = getUser($_GET['id']);
//Check
if(!$user) { return "Bad user"; }

//Get the users last calendar event
$lastEvent = getLastCalendarEvent($user['id']);

//Check the event
if($lastEvent && $lastEvent['end'] == 0) {	//Existing event that needs to be ENDED
	//Decode the event special information
	$esi = json_decode($lastEvent['special'], true);
	//Create the new admin event
	$admin = createAdminEvent(TYPE_SIGNOUT_DEV, "" . $lastEvent['id'], $_SERVER['PHP_AUTH_USER'], "");
	//Add the admin event ID to the special data
	array_push($esi['adminEventLog'],$admin);
	//Update the event
	updateCalendarEvent($lastEvent['id'], $lastEvent['userid'], $lastEvent['start'], time(), json_encode($esi));
	//Create the prepared statement
	$statement = $database->prepare("UPDATE users SET signedin=0 WHERE id=?");
	$statement->bind_param("i",$user['id']);
	$statement->execute();
	//Return the result
	die("Signed Out");
} else {	//New event must be created
	//A new event must be created
	$cevent = createCalendarEvent($user['id']);
	//Decode the event special information
	$esi = json_decode($cevent['special'], true);
	//Create the new admin event
	$admin = createAdminEvent(TYPE_SIGNIN_DEV, "" . $cevent['id'], $_SERVER['PHP_AUTH_USER'], "");
	//Add the admin event ID to the special data
	array_push($esi['adminEventLog'],$admin);
	//Update the event
	updateCalendarEvent($cevent['id'], $cevent['userid'], $cevent['start'], 0, json_encode($esi));
	//Create the prepared statement
	$statement = $database->prepare("UPDATE users SET signedin=1 WHERE id=?");
	$statement->bind_param("i",$user['id']);
	$statement->execute();
	//Return the result
	die("Signed In");
}
?>