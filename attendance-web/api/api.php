<?php
//Configuration
$SQL_HOST = "localhost";
$SQL_USER = "root";
$SQL_PASS = "";
$SQL_DATA = "ligerbots_attendance";

//Disable errors
error_reporting(0);
error_reporting(E_ALL);

//Set content type
header("Content-Type: text/plain");

//Dependencies
include("user.php");

//Establish a database connection
$database = new mysqli($SQL_HOST, $SQL_USER, $SQL_PASS, $SQL_DATA);
//Check for connection error
if($database->connect_errno) {
	fatalError("Database Connection Failure","Database error " . $database->connect_errno);
}

//Check if a user was specified
if(!isSet($_SERVER['PHP_AUTH_USER'])) {
	header('WWW-Authenticate: Basic realm="Ligerbots Attendance Management System"');
	fatalError("Authentication Failure", "No Credentials Given");
}

//Get the user
$user = new User($_SERVER['PHP_AUTH_USER']);
if(!$user->exists()) {
	header('WWW-Authenticate: Basic realm="Ligerbots Attendance Management System"');
	header('HTTP/1.0 401 Unauthorized');
	fatalError("Authentication Failure", "Invalid Credentials");
}

//Check the password
if(!$user->checkPassword($_SERVER['PHP_AUTH_PW'])) {
	header('WWW-Authenticate: Basic realm="Ligerbots Attendance Management System"');
	header('HTTP/1.0 401 Unauthorized');
	fatalError("Authentication Failure", "Invalid Credentials");
}

//Fatal error function
function fatalError($message,$detail) {
	//Create the error object
	$error = array(
		"result" => "error",
		"message" => $message,
		"detail" => $detail
	);
	//Encode and return the result
	die(json_encode($error, JSON_PRETTY_PRINT));
}
?>