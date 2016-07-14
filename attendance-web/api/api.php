<?php
//Disable errors
//error_reporting(0);
error_reporting(E_ALL);

//Load the configuration
include("config.php");

//Define constants
define("TYPE_UNKNOWN", 0);	//Unknown state
define("TYPE_SIGNOUT1", 1);	//Manual user sign out, hours preserved
define("TYPE_SIGNOUT2", 2); //Manual user sign out, hours discarded
define("TYPE_SIGNOUT3", 3); //User signed out via the device
define("TYPE_SIGNIN1", 4);	//User signed in via the device

//Set the content type
header("Content-Type: text/plain");

//Establish a database connection
$database = new mysqli($SQL_HOST, $SQL_USER, $SQL_PASS, $SQL_DATA);
//Check for database connection error
if($database->connect_errno) {
	//Failure
	die(json_encode(array("result"=>"error","message"=>"Failed to connect to database","detail"=>$database->connect_errno),JSON_PRETTY_PRINT));
}

//Get a user by PIN or ID
function getUser($id) {
	//Global variables
	global $database;
	//Create the statement
	$stmt = $database->prepare("SELECT * FROM users WHERE pin=? OR rfid=?");
	//Bind the parameters
	$stmt->bind_param("ss", $id, $id);
	//Execute the statement
	$stmt->execute();
	//Get the result
	$result = $stmt->get_result();
	//Return the result value
	return $result->fetch_array();
}

//User functions
function createCalendarEvent($uid) {
	//Global variables
	global $database;
	//Variables
	$time = time();
	$defaultSpecial = '{"adminEventLog":["0"]}';
	//Create the statement
	$stmt = $database->prepare("INSERT INTO calendar (userid,start,end,special) VALUES (?,?,0,?)");
	//Bind parameters
	$stmt->bind_param("iis", $uid, $time, $defaultSpecial);
	//Execute the statement
	$stmt->execute();
	//Create the new statement
	$stmt = $database->prepare("SELECT LAST_INSERT_ID()");
	//Execute the new statement
	$stmt->execute();
	//Get the result
	$result = $stmt->get_result();
	//Create the new statement
	$stmt = $database->prepare("SELECT * FROM calendar WHERE id=?");
	//Bind the parameters
	$stmt->bind_param("i", $result->fetch_array()['LAST_INSERT_ID()']);
	//Execute the statement
	$stmt->execute();
	//Get the result
	$result = $stmt->get_result();
	//Return the result
	return $result->fetch_array();
}

function updateCalendarEvent($id,$userid,$start,$end,$special) {
	//Global variables
	global $database;
	//Create the statement
	$stmt = $database->prepare("UPDATE calendar SET userid=?,start=?,end=?,special=? WHERE id=?");
	//Bind parameter
	$stmt->bind_param("iiisi", $userid, $start, $end, $special, $id);
	//Execute the statement
	$stmt->execute();
}

function getLastCalendarEvent($uid) {
	//Global variables
	global $database;
	//Create the statement
	$stmt = $database->prepare("SELECT * FROM calendar WHERE userid=? ORDER BY id DESC LIMIT 1");
	//Set the parameters
	$stmt->bind_param("i",$uid);
	//Execute the statement
	$stmt->execute();
	//Get the result
	$result = $stmt->get_result();
	//Get the result value
	return $result->fetch_array();
}

function createAdminEvent($type,$reference,$who,$notes) {
	//Global variables
	global $database;
	//Variables
	$time = time();
	//Create the statement
	$stmt = $database->prepare("INSERT INTO events (type,reference,who,time,notes) VALUES (?,?,?,?,?)");
	//Bind the parameters
	$stmt->bind_param("issis", $type, $reference, $who, $time, $notes);
	//Execute the statement
	$stmt->execute();
	//Create the new statement
	$stmt = $database->prepare("SELECT LAST_INSERT_ID()");
	//Execute the new statement
	$stmt->execute();
	//Get the result
	$result = $stmt->get_result();
	//Get the result value
	return $result->fetch_array()['LAST_INSERT_ID()'];
}
?>