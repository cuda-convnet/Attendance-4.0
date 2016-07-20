class API {

	static getCurrentUsers(table) {
		//Send the request
		API.sendTableRequest("api/getCurrentUsers.php", table);
	}

	static getRecentEvents(table) {
		//Send the request
		API.sendTableRequest("api/getRecentEvents.php", table);
	}

	static getAllUsers(table) {
		//Send the request
		API.sendTableRequest("api/getAllUsers.php", table);
	}

	static getUserInformation(id,callback) {
		//Send the request
		API.sendRawRequest("api/getUserInfo.php?id=" + id, function(rawresponse) {
			//Result
			var response;
			//Try to parse the json
			try {
				response = JSON.parse(rawresponse.responseText);
			} catch(e) {
				//Failed to decode json
				response = {"result": "error", "message": "Invalid Response", "detail": "Server returned invalid JSON"};
			}
			//Send the response
			callback([response,rawresponse]);
		});
	}

	static processAndPopulate(rawresponse, table) {
		//Result
		var response;
		//Try to parse the json
		try {
			response = JSON.parse(rawresponse.responseText);
		} catch(e) {
			//Failed to decode json
			response = {"result": "error", "message": "Invalid Response", "detail": "Server returned invalid JSON"};
		}
		//Check for error
		if(response["result"] == "error") {
			//Create advanced message
			//Create debug message
			var message = "This is an advanced debug message - it probably won't mean a whole lot to you unless you're a developer.\n\n";
			message += "----------------------------------------\n\n";
			message += "Request URL: " + rawresponse.responseURL + "\n";
			message += "Status: " + rawresponse.status + "\n";
			message += "Status Text: " + rawresponse.statusText + "\n\n";
			message += "----------------------------------------\n\n";
			message += "Raw Response Body:\n\n";
			message += rawresponse.responseText;
			//Failed to copmlete request, display error
			table.setError(response["message"], response["detail"], message);
		} else {
			//Display the request data
			table.populate(response);
			//Temporary
			table.setStatus("Loaded " + response.length + " result" + (response.length != 1 ? "s" : ""));
		}
	}

	static sendTableRequest(url, table) {
		//Send request
		API.sendRawRequest(url, function(response) {
			//Pass the response to the callback handler
			API.processAndPopulate(response, table);
		});
	}

	static sendRawRequest(url, callback) {
		//Create the request object
		var request = new XMLHttpRequest();
		//Set the callback handler
		request.onreadystatechange = function() {
			//Check the state
			if(request.readyState == 4) {
				//Pass the response to the callback
				callback(request);
			}
		}
		//Send the request
		request.open("GET", url, true);
		request.send(null);
	}
}