<?php

//Permissions class
class Permissions {
	//User permissions
	var $permissions = false;
	var $rawpermissions = false;

	//Constructs the permissions tree
	function __construct($raw) {
		//Load the default permission tree
		$this->permissions = json_decode(file_get_contents("permissions.json"));
		//Get the permissions
		$this->rawpermissions = explode(";",$raw);
		//Set every permission
		walk($this->permissions, function(&$element,$path) {
			//Check this permission
			if(in_array($path, $this->rawpermissions)) {
				//If this is an object or array, set all of its children to true
				makeTrue($element);
				//Match
				return false;  //False = don't continue
			}
			//No match
			return true;	//True = do continue
		});
	}

	//Checks if the given permission is listed
	function checkPermission($permission) {

	}

	//Compile the permissions into a string
	function compile() {
		//Copy the permissions array
		$permcopy = json_decode(json_encode($this->permissions),true);  //TODO: Is there a better way to do this?
		//Collapse the array
		collapse($permcopy,"");
		//Result
		$result = "";
		//Convert the array
		walk($permcopy, function(&$element,$path) use(&$result) {
			//Get type
			$type = gettype($element);
			//Check type
			if($type == "object" || $type == "array") { return true; }
			//Check for wildcard
			if($element == "*") {
				$result .= $path . ".*;";
			} else {
				//Encode
				$result .= $path . ";";
			}
		});
		//Return
		return rtrim($result,";");
	}
}

//Collapser
function collapse(&$element,$path) {
	//Get the type
	$type = gettype($element);
	//Check type
	if($type == "object" || $type == "array") {
		//Result counter
		$trueCount = 0;
		$falseCount = 0;
		$failCount = 0;
		//Check all the children
		foreach($element as $key=>&$child) {
			//Collapse the children
			$result = collapse($child, $path . "." . $key);
			//Check the result
			if($result == 1) {
				//Collapsed the child to "true"
				$trueCount++;
			} else if($result == 2) {
				//Collapsed the child to "false"
				$falseCount++;
			} else {
				//Child could not be collapsed :(
				$failCount++;
			}
			//Check if the child is worthless
			if($child == false) { unset($element[$key]); }
		}
		//Check and return
		if($failCount >0) {
			return 3;
		} if($trueCount == 0 && $falseCount > 0) {
			//I am false
			$element = false;
			return 2;
		} else if($trueCount > 0 && $falseCount == 0) {
			//I am true
			$element = "*";
			return 1;
		}
	} else {
		//Return the value
		if($element === true) {
			return 1;
		} else {
			return 2;
		}
	}
}

//Array walker
function walk(&$array,$callback,$path = "") {
	//Process each element
	foreach($array as $key=>&$element) {
		//Get type
		$type = gettype($element);
		//Check type
		if($type == "object" || $type == "array") {
			//Recurse
			if($callback($element, $path . $key . ".*")) {
				//Only continue walk if callback returns true
				walk($element,$callback,$path . $key . ".");
			}
		} else {
			//Fire the callback
			$callback($element,$path . $key);
		}
	}
}

//Makes everything true
function makeTrue(&$element) {
	//Check the type
	$type = gettype($element);
	//Process
	if($type == "object" || $type == "array") {
		//Make true
		foreach($element as &$child) {
			makeTrue($child);
		}
	} else {
		//Make true
		$element = true;
	}
}

//test
header("Content-Type: text/plain");
$permissions = new Permissions("users.list;users.delete;calendar.*");
print(json_encode($permissions->permissions, JSON_PRETTY_PRINT));
print(PHP_EOL . PHP_EOL . PHP_EOL);
$permissions->compile();
?>