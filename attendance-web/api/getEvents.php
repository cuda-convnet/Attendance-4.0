<?php
//Include the api
include("api.php");

//Check the parameters
if(!isSet($_GET['count'])) {
	//Failure
	die(json_encode(array("result"=>"error","message"=>"Event count not specified"),JSON_PRETTY_PRINT));
}

//Create the statement
$stmt = $database->prepare("SELECT * FROM events ORDER BY id DESC LIMIT ?");
//Get the count
$count = intval($_GET['count']);
//Bind the parameter
$stmt->bind_param("i", $count);
//Execute the statement
$stmt->execute();
//Get the result
$queryResult = $stmt->get_result();

//Event results
$events = array();
//Populate the events
while($obj = $queryResult->fetch_object()) {
	//Add the details to the result
	$events[intval($obj->id)] = array(
		"id" =>		$obj->id,
		"type" =>	$obj->type,
		"reference" =>	$obj->reference,
		"who" =>	$obj->who,
		"time" =>	$obj->time,
		"notes" =>	$obj->notes
	);
}

//Encode and return the result
die(json_encode($events, JSON_PRETTY_PRINT));
?>