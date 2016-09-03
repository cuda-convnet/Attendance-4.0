<?php
//Disable printing of errors
error_reporting(-1);
//Check for error reporting override
if(isSet($_GET['debug'])) { error_reporting(intval($_GET['debug'])); }

//Include dependencies
include("user.php");

//Define calendar meta values
define("CALENDAR_SUSPENDED", 0b00000001);	// Events that are suspended will not count towards total time
define("CALENDAR_MODIFIED",	 0b00000010);	// Events that were changed from their original values
define("CALENDAR_GIVEN",	 0b00000100);	// Events that were manually created to give a user credit

//Configuration
$SQL = array(
	"HOST" => "localhost",
	"USER" => "root",
	"PASS" => "",
	"NAME" => "attendance"
);

//Global objects
$database;
$user = null;

//Connect to the database
$database = new mysqli($SQL["HOST"], $SQL["USER"], $SQL["PASS"], $SQL["NAME"]);
//Check for connection error
if($database->connect_error) {
	error("Failed to connect to database",$database->connect_error);
}

//Check if credentials were supplied
if(isSet($_SERVER['PHP_AUTH_USER'])) {
	//Get global object
	global $user;
	//Get the user
	$user = new User($_SERVER['PHP_AUTH_USER'], USER_SELECTOR_UNAME);
	//Check if the user exists
	if($user->error) {
		//Failure
		generate401("Invalid Credentials1 " . $user->error);
	}
	//Check the password
	if(!$user->checkPassword($_SERVER['PHP_AUTH_PW'])) {
		//Invalid password
		generate401("Invalid Credentials2");
	}
}

//Error function
function error($message, $detail = false) {
	//Create the response object
	$response = array();
	//Set the result information
	$response["result"] = "error";
	$response["message"] = $message;
	//Add detail if specified
	if($detail !== false) {	$response["detail"] = $detail; }
	//Set headers
	header("Content-Type: text/plain");
	//Return the message
	die(json_encode($response, JSON_PRETTY_PRINT));
}

//Success function
function success($response) {
	//Set headers
	header("Content-Type: text/plain");
	//Check the type
	if(gettype($response) != "object" && gettype($response) != "array") {
		$response = array("result" => "success", "message" => $response);
	}
	//Encode and return
	die(json_encode($response, JSON_PRETTY_PRINT));
}

//Method for setting the required authentication and permission
function setAccess($permission) {
	//Get global object
	global $user;
	//Check if the user even bothered to log in
	if($user==null) {
		//User is not logged in
		generate401("Authentication is required to view the requested resource");
	} else {
		//Check if the user has permission
		if(!$user->checkPermission($permission)) {
			//User does not have permission
			error("Access Denied","Missing required permission " . $permission);
		}
	}
}

//Method for generating a 401 error
function generate401($reason = false) {
	//Send headers
	header('WWW-Authenticate: Basic realm="Ligerbots Attendance"');
	header('HTTP/1.0 401 Unauthorized');
	//Message
	error("Authentication Failure",$reason);
}

?>