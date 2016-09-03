<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("users.modify");

//Arguments
$args = [
	"id" => null,
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
	//Set the argument
	if(isSet($_POST[$name])) {
		$value = isSet($_POST[$name]) ? $_POST[$name] : null;
		if($value == "") { $value = null; }
	}
}

//Check the user ID
if($args['id'] == null) {
	error("Invalid Request", "Missing target user ID");
}

//Get the actual user
$realuser = new User($args['id'], USER_SELECTOR_ID);
//Check if real user
if($realuser->error !== false) {
	error("Invalid Request", "Unknown user ID");
}

//Check if password change
if($args['password'] != null) {
	$realuser->setPassword($password);
}

//Check the permissions
if($args['permissions'] != null) {
	//Check if permissions are valid
	if(json_decode($args['permissions']) == false) {
		error("Invalid Data", "Given permissions were not valid JSON");
	}
	//Get permissions
	$perms = json_decode($permissions);
	//Check each permission
	foreach($perms as $perm) {
		if(!$user->checkPermission($perm)) {
			error("Security Error", "You are not authorized to grant permission \"" . $perm . "\"");
		}
	}
	//Update permissions
	$stmt = $database->prepare("UPDATE users SET permissions=? WHERE id=?");
	$stmt->bind_param("si",$args['permissions'],intval($args['id']));
	if($stmt->execute() === false) {
		error("Internal Error","SQL returned " . $stmt->error);
	}
}

//Update other fields
update('fname',$args['fname']);
update('lname',$args['lname']);
update('email',$args['email']);
update('pin',$args['pin']);
update('rfid',$args['rfid']);
update('username',$args['username']);

function update($field, $value) {
	//Global database and arguments reference
	global $database, $args;
	//Only do the thing if the value isn't null
	if($value != null) {
		$stmt = $database->prepare("UPDATE users SET ".$field."=? WHERE id=?");
		$stmt->bind_param("si",$value,$args['id']);
		if($stmt->execute() === false) {
			error("Internal Error","SQL returned " . $stmt->error);
		}
		echo("Updated " . $field . PHP_EOL);
	}
}

//Finished
success("Successfuly updated user");
?>