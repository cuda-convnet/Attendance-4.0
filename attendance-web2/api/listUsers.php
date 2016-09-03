<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("users.list");

//Create the statement
$stmt = $database->prepare(file_get_contents("sql/listUsers.sql"));
//Execute the statement
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}
//Get sql result
$result = $stmt->get_result();
//User list
$users = [];
//Process result
while(($row = $result->fetch_object()) != NULL) {
	//Create the user object
	$user = [];
	//Set the user data
	$user['id'] = $row->id;
	$user['fname'] = $row->fname;
	$user['lname'] = $row->lname;
	$user['email'] = $row->email;
	$user['pin'] = $row->pin;
	$user['rfid'] = $row->rfid;
	$user['username'] = $row->username;
	$user['permissions'] = json_decode($row->permissions);
	$user['time'] = $row->time;
	$user['signedin'] = $row->signedin;
	//Push to the list
	array_push($users, $user);
}

//Encode and return result
success($users);
?>