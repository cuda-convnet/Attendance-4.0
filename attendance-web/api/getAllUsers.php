<?php
//Include the api
include("api.php");

//Check permission
if(!$user->checkPermission("admin.getAllUsers")) {
	fatalError("Access Denied", "Missing required permission \"admin.getAllUsers\"");
}

//Final result object
$current_users = array();

//Get the users
$result = $database->query(file_get_contents("sql/getAllUsers.sql"));
//Create the result
while($uresult = $result->fetch_array(MYSQLI_ASSOC)) {
	//Entry
	$entry = array(
		"id" => $uresult['id'],
		"name" => $uresult['fname'] . " " . $uresult['lname'],
		"pin" => $uresult['pin'],
		"rfid" => $uresult['rfid']
	);
	//Push
	array_push($current_users, $entry);
}

//Encode and return
die(json_encode($current_users, JSON_PRETTY_PRINT));
?>