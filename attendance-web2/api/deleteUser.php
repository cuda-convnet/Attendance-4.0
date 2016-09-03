<?php
//Include the API
include("include/api.php");

//Required permission
setAccess("users.delete");

//Get the user id
if(!isSet($_GET['id'])) {
	error("Invalid Data","No user was specified");
}

//Get the user
$damneduser = new User($_GET['id'], USER_SELECTOR_ID);

//Users aren't actually removed, just in case somebody fucks up big time
$stmt = $database->prepare("INSERT INTO delusers (id,fname,lname,email,pin,rfid,username,passhash,permissions) VALUES (?,?,?,?,?,?,?,?,?)");
$stmt->bind_param("issssssss",
	$damneduser->udata['id'],
	$damneduser->udata['fname'],
	$damneduser->udata['lname'],
	$damneduser->udata['email'],
	$damneduser->udata['pin'],
	$damneduser->udata['rfid'],
	$damneduser->udata['username'],
	$damneduser->udata['passhash'],
	$damneduser->udata['permissions']);
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}

//Create the statement
$stmt = $database->prepare("DELETE FROM users WHERE id=?");
//Bind the parameters
$stmt->bind_param("i", $damneduser->udata['id']);
//Execute the query
if($stmt->execute() === false) {
	error("Internal Error","SQL returned " . $stmt->error);
}

//Finished
success("Successfuly deleted user");
?>