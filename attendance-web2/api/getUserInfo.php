<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("users.getinfo");

//Check for a user identifier
$id = isSet($_GET['id']) ? $_GET['id'] : null;
if($id == null) {
	error("Invalid Request", "No user ID specified");
}

//Create the statement
$stmt = $database->prepare(file_get_contents("sql/getUserInfo.sql"));
//Bind the parameters
$stmt->bind_param("i",$id);
//Execute the statement
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}
//Get the result
$qresult = $stmt->get_result();
//Get the object
$object = $qresult->fetch_object();
//Check for error
if($object == NULL) {
	error("Invalid User", "No user found with that ID");
}

//Create the end result
$result = array(
	"id" => $object->id,
	"fname" => $object->fname,
	"lname" => $object->lname,
	"email" => $object->email,
	"pin" => $object->pin,
	"rfid" => $object->rfid,
	"username" => $object->username,
	"permissions" => json_decode($object->permissions),
	"time" => $object->time,
	"abstime" => $object->abstime,
	"signedin" => $object->signedin
);
//Return the result
success($result);
?>