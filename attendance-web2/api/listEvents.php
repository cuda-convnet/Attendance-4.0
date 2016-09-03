<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("calendar.list");

//Optional selector parameters
$start = isSet($_GET['start']) ? $_GET['start'] : 0;
$end   = isSet($_GET['end'])   ? $_GET['end']   : PHP_INT_MAX;
$limit = isSet($_GET['limit']) ? $_GET['limit'] : 150;

//Get all of the callendar events
$stmt = $database->prepare(file_get_contents("sql/listEvents.sql"));
$stmt->bind_param("iii", $start, $end, $limit);
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}
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

//Finished
success($result);
?>