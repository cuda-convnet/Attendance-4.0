<?php
//Include the API
include("api.php");

//Query the database for users
$queryResult = $database->query("SELECT * FROM users");
//Create the result object
$result = array();
//Populate the result object
while($obj = $queryResult->fetch_object()) {
	//Add the details to the result
	$result[intval($obj->id)] = array(
		"id" =>		$obj->id,
		"fname" =>	$obj->fname,
		"lname" =>	$obj->lname,
		"pin" =>	$obj->pin,
		"rfid" =>	$obj->rfid,
		"lastsignin" =>	$obj->lastsignin,
		"totaltime" =>	$obj->totaltime,
		"signedin" => 	$obj->signedin
	);
}

//Encode and return the result
die(json_encode($result, JSON_PRETTY_PRINT));
?>