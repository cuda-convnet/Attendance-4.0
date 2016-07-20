<?php
//Include the api
include("api.php");

//Check permission
if(!$user->checkPermission("admin.getUserInfo")) {
	fatalError("Access Denied", "Missing required permission \"admin.getUserInfo\"");
}

//Check argument
if(!isSet($_GET['id'])) {
	fatalError("Invalid Request", "Missing required request parameter \"id\"");
}

//Get the user info with the power of this massive abomination of a query
$statement = $database->prepare(file_get_contents("sql/getUserInfo.sql"));
//Bind parameters
$statement->bind_param("i",$_GET['id']);
//Execute the statement
$statement->execute();
//Get the user info
$uresult = $statement->get_result()->fetch_array();
//Create the result
$result = array(
	"id" =>		$uresult['id'],
	"fname" =>	$uresult['fname'],
	"lname" =>	$uresult['lname'],
	"email" =>	$uresult['email'],
	"pin" =>	$uresult['pin'],
	"rfid" =>	$uresult['rfid'],
	"username" =>	$uresult['username'],
	"permissions" =>$uresult['permissions'],
	"totaltime" =>	$uresult['totaltime'],
	"timesource" => array (
		"source0" =>	$uresult['s_0'],
		"source1" =>	$uresult['s_1'],
		"source2" =>	$uresult['s_2'],
		"source3" =>	$uresult['s_3']
	),
	"timetype" => 	array (
		"type0" =>		$uresult['t_0'],
		"type1" =>		$uresult['t_1'],
		"type2"	=>		$uresult['t_2'],
		"type3" =>		$uresult['t_3']
	)
);

//Encode and return
die(json_encode($result, JSON_PRETTY_PRINT));
?>