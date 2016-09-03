<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("users.create");

//Arguments
$args = [
	"fname" => null,
	"lname" => null,
	"email" => null,
	"pin" => null,
	"rfid" => null,
	"username" => null,
	"password" => null,
	"permissions" => null
];

//Get arguments
foreach($args as $name=>&$value) {
	//Check if the argument is set
	if(!isSet($_POST[$name])) {
		error("Invalid Request","Missing required argument '" . $name . "'");
	}
	//Set the value
	$value = $_POST[$name];
}

//Check if the pin is unique
$fakeuser = new User($args['pin'], USER_SELECTOR_PIN);
if($fakeuser->error == false) {
	error("Invalid Data","The specified PIN number is already in use");
}
//Check if the RFID is unique
$fakeuser = new User($args['rfid'], USER_SELECTOR_RFID);
if($fakeuser->error == false) {
	error("Invalid Data","The specified RFID serial is already in use");
}
//Check if the permissions field is valid json
if(json_decode($args['permissions']) == NULL) {
	error("Invalid Data","The specified Permissions were not valid json");
}

//Hash the password
$args['password'] = password_hash($args['password'], PASSWORD_BCRYPT);

//Create the statement
$stmt = $database->prepare("INSERT INTO users (fname,lname,email,pin,rfid,username,passhash,permissions) VALUES (?,?,?,?,?,?,?,?)");
//Bind the parameters
$stmt->bind_param("ssssssss", $args['fname'],$args['lname'],$args['email'],$args['pin'],$args['rfid'],$args['username'],$args['password'],$args['permissions']);
//Execute the query
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}

//Finished
success("Successfuly created user");
?>