<?php

//ID types
define("USER_SELECTOR_ID",		0b00000001);
define("USER_SELECTOR_PIN",		0b00000010);
define("USER_SELECTOR_RFID",	0b00000100);
define("USER_SELECTOR_UNAME",	0b00001000);
define("USER_SELECTOR_EMAIL",	0b00010000);

//User class
class User {
	//Error
	var $error = false;
	//Data
	var $udata = false;
	
	//User constructor
	function __construct($identifier, $idtype) {
		//Create the query
		$query = "SELECT *,(SELECT IF(calendar.end IS NOT NULL,'1','0') FROM calendar WHERE calendar.user=users.id AND calendar.end=0 ) AS 'signedin' FROM users WHERE ";
		//Identifiers
		$identifiers = array();
		$bindstring = "";
		//Add selectors as specified
		if($idtype & USER_SELECTOR_ID)		{ $identifiers[] = & $identifier;  $bindstring .= "i";  $query .= "id=? AND "; }
		if($idtype & USER_SELECTOR_PIN)		{ $identifiers[] = & $identifier;  $bindstring .= "s";  $query .= "pin=? AND "; }
		if($idtype & USER_SELECTOR_RFID)	{ $identifiers[] = & $identifier;  $bindstring .= "s";  $query .= "rfid=? AND "; }
		if($idtype & USER_SELECTOR_UNAME)	{ $identifiers[] = & $identifier;  $bindstring .= "s";  $query .= "username=? AND "; }
		if($idtype & USER_SELECTOR_EMAIL)	{ $identifiers[] = & $identifier;  $bindstring .= "s";  $query .= "email=? AND "; }
		//Finish off the query
		$query .= "1=1 LIMIT 1";
		//Finish off the bind
		array_unshift($identifiers, $bindstring);
		//Get global object
		global $database;
		//Get the database statement
		$stmt = $database->prepare($query);
		//Bind parameters the hacky way
		call_user_func_array(array($stmt, 'bind_param'), $identifiers);
		//Execute the statement
		if(!$stmt->execute()) {
			//Failure
			$this->error = $stmt->error;
			return;
		}
		//Fetch user information
		$result = $stmt->get_result();
		//Check if the user exists
		if($result->num_rows == 0) {
			$this->error = "SQL server returned 0 results";
			return;
		}
		//Fetch the user information
		$this->udata = $result->fetch_object();
}

	//Method for checking a password
	function checkPassword($password) {
		//Return the password verification result
		return password_verify($password, $this->udata->passhash);
	}

	//Method for setting a password
	function setPassword($password) {
		//Get global object
		global $database;
		//Generate the new password hash
		$hash = password_hash($password, PASSWORD_BCRYPT);
		//Get the database statement
		$stmt = $database->prepare("UPDATE users SET passhash=? WHERE id=?");
		//Bind the parameters
		$stmt->bind_param("si",$hash,$this->udata->id);
		//Execute the statement
		if(!$stmt->execute()) {
			//Failure
			return $stmt->error;
		}
		//Finished
		return true;
	}

	//Method for getting user permissions
	function getPermission() {
		//Return decoded permission
		return json_decode($this->udata->permissions);
	}

	//Method for setting user permissions
	function setPermissions($permission) {
		//Create the statement
		$stmt = $database->prepare("UPDATE users SET permissions=? WHERE id=?");
		//Bind the parameters
		$stmt->bind_param("si",json_encode($permission),$this->udata->id);
		//Execute the statement
		if(!$stmt->execute()) {
			//Failure
			return $stmt->error;
		}
		//Finished
		return true;
	}

	//Method for checking the user permission
	function checkPermission($permission) {
		//Check for super admin
		if($this->isSuperAdmin()) { return true; }
		//Check the permission
		return in_array($permission, $this->getPermission());
	}

	//Method for checking if the user is a super admin
	function isSuperAdmin() {
		//Super Admins
		$superAdmins = ["1"];
		//Check if this user is a super admin
		return in_array($this->udata->id,$superAdmins);
	}

	//Method for toggling the user state
	function signToggle() {
		//Check the current state
		if($this->udata->signedin == "1") {
			//Sign the user out
			return $this->signOut();
		} else {
			//Sign the user in
			return $this->signIn();
		}
	}

	//Method for signing the user out
	function signOut() {
		//Get the global database object
		global $database;
		//Time variable (because something about statment bind security)
		$time = time();
		//Create the statement
		$stmt = $database->prepare("UPDATE calendar SET end=? WHERE user=? AND end=0 LIMIT 1");
		//Bind the parameters
		$stmt->bind_param("ii", $time, $this->udata->id);
		//Execute the query
		if(!$stmt->execute()) {
			//Failure
			return $stmt->error;
		}
		//Finished
		return "signedOut";
	}

	//Method for signing the user in
	function signIn() {
		//Get the global database object
		global $database;
		//Time variable (because something about statment bind security)
		$time = time();
		//Create the statement
		$stmt = $database->prepare("INSERT INTO calendar (start,end,user,meta) VALUES (?,0,?,b'00000000')");
		//Bind the parameters
		$stmt->bind_param("ii", $time, $this->udata->id);
		//Execute the query
		if(!$stmt->execute()) {
			//Failure
			return $stmt->error;
		}
		//Finished
		return "signedIn";
	}
}

?>