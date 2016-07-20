<?php
class User {
	//User data
	var $udata = array();
	var $permissions = array();

	//ID can be either username, pin, or RFID
	function __construct($id) {
		//Get global database
		global $database, $udata;
		//Create the statement
		$stmt = $database->prepare("SELECT * FROM users WHERE username=? OR pin=? OR rfid=?");
		$stmt->bind_param("sis",$id,$id,$id);
		$stmt->execute();
		//Get the result
		$result = $stmt->get_result();
		$udata = $result->fetch_array();
	}

	//Check if the user exists
	function exists() {
		global $udata;
		return !($udata === false);
	}

	//Check the users password
	function checkPassword($password) {
		//Check
		global $udata;
		return password_verify($password, $udata['passhash']);
	}

	//Check the users permision
	function checkPermission($permission) {
		//Global variables
		global $udata;
		//Get the users permissions
		$uperms = explode(";", $udata['permissions']);
		//Get the permission parts
		$permissionParts = explode(".",$permission);
		//Iterate through the user permissions
		foreach($uperms as $uperm)  {
			//Break up the user permission into parts
			$upermParts = explode(".",$uperm);
			//Compare parts
			for($i = 0; $i < sizeof($upermParts); $i++) {
				//Check if user permission is a wildcard
				if($upermParts[$i] == "*") {
					//User has permission
					return true;
				} else if($upermParts[$i] != $permissionParts[$i]) {
					//Mismatch
					continue 2;
				}
			}
			//Has permission
			return true;
		}
		//User does not have permission
		return false;
	}
}
?>