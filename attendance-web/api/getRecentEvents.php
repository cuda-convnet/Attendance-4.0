<?php
//Include the api
include("api.php");

//Check permission
if(!$user->checkPermission("admin.getRecentEvents")) {
	fatalError("Access Denied", "Missing required permission \"admin.getRecentEvents\"");
}

//Final results object
$recent_events = array();

//Get the users
$result = $database->query("SELECT * FROM events ORDER BY id DESC LIMIT 150");
//Create the result
while($uresult = $result->fetch_array(MYSQLI_ASSOC)) {
	//Entry
	$entry = array(
		"id" => $uresult['id'],
		"type" => $uresult['type'],
		"who" => $uresult['who'],
		"time" => $uresult['time'],
		"notes" => $uresult['notes']
	);
	//Push
	array_push($recent_events, $entry);
}

//Encode and return
die(json_encode($recent_events, JSON_PRETTY_PRINT));
?>