class Actions {

	//Show user information dialog
	static showUserInformationDialog(element) {
		//Log
		console.log("Showing \"Current User Information\" dialog...");
		//Get ID and name
		var id = element.parentElement.parentElement.getAttribute("raw_id");
		var name = element.parentElement.parentElement.getAttribute("raw_name");

		//Create dialog
		var container = document.createElement("div");
		container.className = "user_info_box";
		//Create title
		var title = document.createElement("div");
		title.className = "cardname";
		title.innerHTML = "User Information";
		container.appendChild(title);
		//Create loading frame
		var loading = document.createElement("img");
		loading.className = "centered";
		loading.src = "res/img/loading.svg";
		container.appendChild(loading);
		//Create the dialog
		var dialog = new Dialog(container, true);
		dialog.expand();

		//Request the user information
		API.getUserInformation(id, function(data) {
			var response = data[0];
			//Check for error
			if(response["result"] == "error") {
				//Hide the loading symbol
				loading.style.display = "none";
				//Create debug message
				var message = "This is an advanced debug message - it probably won't mean a whole lot to you unless you're a developer.\n\n";
					message += "----------------------------------------\n\n";
					message += "Request URL: " + data[1].responseURL + "\n";
					message += "Status: " + data[1].status + "\n";
					message += "Status Text: " + data[1].statusText + "\n\n";
					message += "----------------------------------------\n\n";
					message += "Raw Response Body:\n\n";
					message += data[1].responseText;
				//Create the error
				var error = Util.generateErrorMessage(response["message"],response["detail"],message);
				error.className += " centered";
				//Create the retry button
				var retry = document.createElement("button");
				retry.innerHTML = "Retry";
				retry.onclick = function() {
					//Close the dialog
					dialog.close();
					//Try again
					Actions.showUserInformationDialog(element);
				};
				//Create the close button
				var close = document.createElement("button");
				close.innerHTML = "Close";
				close.onclick = function() {
					//Close the dialog
					dialog.close();
				}
				//Add the buttons to the error
				error.appendChild(retry);
				error.appendChild(close);
				//Add the error
				container.appendChild(error);
			} else {
				//Hide the loading symbol
				loading.style.display = "none";
				//Create a chart box
				var chartcontainer = document.createElement("div");
				//Create the title
				var ctitle = document.createElement("div");
				ctitle.innerHTML = "Hours Breakdown";
				chartcontainer.appendChild(ctitle);
				//Create a chart
				var chart = document.createElementNS("http://www.w3.org/2000/svg", "svg");
				chart.style.width = "210px";
				chart.style.height = "210px";
				//Calculate the slice angles
				var slice0 = (parseInt(response['timesource']['source0']) / parseInt(response['totaltime']) * 360);
				var slice1 = (parseInt(response['timesource']['source1']) / parseInt(response['totaltime']) * 360) + slice0;
				var slice2 = (parseInt(response['timesource']['source2']) / parseInt(response['totaltime']) * 360) + slice1;
				//Calculate the pies
				var pie1 = Pie.generatePie(105,105,0,slice0,100);
				var pie2 = Pie.generatePie(105,105,slice0,slice1,100);
				var pie3 = Pie.generatePie(105,105,slice1,slice2,100);
				var pie4 = Pie.generatePie(105,105,slice2,360,100);
				//Set the ids
				pie1.id = "pie1";
				pie2.id = "pie2";
				pie3.id = "pie3";
				pie4.id = "pie4";
				//Add the slices
				chart.appendChild(pie1);
				chart.appendChild(pie2);
				chart.appendChild(pie3);
				chart.appendChild(pie4);
				//Add the chart to the container
				chartcontainer.appendChild(chart);
				//Add it all to the main thing
				container.appendChild(chartcontainer);
			}
		});
	}

	//Show the dialog for deleting a current event
	static showDeleteCurrentEventDialog(element) {
		//Log
		console.log("Showing \"Delete Current Event\" dialog...");
		//Get ID and name
		var id = element.parentElement.parentElement.getAttribute("raw_id");
		var name = element.parentElement.parentElement.getAttribute("raw_name");
		//Create the alert
		var container = document.createElement("div");
		//Create the title
		var title = document.createElement("div");
		title.className = "cardname";
		title.innerHTML = "Delete Event";
		//Create the message
		var message = document.createElement("div");
		message.className = "alert_message";
		message.innerHTML = "Are you sure you want to delete this event?<br><b>" + name + "</b> will be signed out immediately and will <b>not</b> receive credit for this event.";
		//Create the buttons
		var yesbutton = document.createElement("button");
		var nobutton = document.createElement("button");
		yesbutton.className = "half_button";
		nobutton.className = "half_button";
		yesbutton.innerHTML = "Yes";
		nobutton.innerHTML = "No";
		//Add the elements to the container
		container.appendChild(title);
		container.appendChild(message);
		container.appendChild(yesbutton);
		container.appendChild(nobutton);
		//Create the dialog
		var dialog = new Dialog(container,true);

		//Create the handler for the yes button
		yesbutton.onclick = function() {
			//Log
			console.log("User clicked YES: Deleting current event...");
			//Close the dialog
			dialog.close();
			//Delete the event
			Actions.executeAction("api/signOut.php?id=" + id + "&delete=true");
		};

		//Create the handler for the no button
		nobutton.onclick = function() {
			//Log
			console.log("User clicked NO: Closing dialog...");
			//Close the dialog
			dialog.close();
		};
	}

	//Show the dialog for signing out a user
	static showSignOutUserDialog(element) {
		//Log
		console.log("Showing \"Sign Out User\" dialog...");
		//Get ID and name
		var id = element.parentElement.parentElement.getAttribute("raw_id");
		var name = element.parentElement.parentElement.getAttribute("raw_name");
		//Create the alert
		var container = document.createElement("div");
		//Create the title
		var title = document.createElement("div");
		title.className = "cardname";
		title.innerHTML = "Sign Out";
		//Create the message
		var message = document.createElement("div");
		message.className = "alert_message";
		message.innerHTML = "Are you sure you want to sign this user out?<br><b>" + name + "</b> will receive some credit for this event, up until the current time.";
		//Create the buttons
		var yesbutton = document.createElement("button");
		var nobutton = document.createElement("button");
		yesbutton.className = "half_button";
		nobutton.className = "half_button";
		yesbutton.innerHTML = "Yes";
		nobutton.innerHTML = "No";
		//Add the elements to the container
		container.appendChild(title);
		container.appendChild(message);
		container.appendChild(yesbutton);
		container.appendChild(nobutton);
		//Create the dialog
		var dialog = new Dialog(container,true);

		//Create the handler for the yes button
		yesbutton.onclick = function() {
			//Log
			console.log("User clicked YES: Signing user out...");
			//Close the dialog
			dialog.close();
			//Delete the event
			Actions.executeAction("api/signOut.php?id=" + id + "&delete=false");
		};

		//Create the handler for the no button
		nobutton.onclick = function() {
			//Log
			console.log("User clicked NO: Closing dialog...");
			//Close the dialog
			dialog.close();
		};
	}

	//Execute an API request
	static executeAction(url) {
		//Log
		console.log("Executing API request \"" + url + "\"...");
		//Create a loading dialog
		var loading = Dialog.createLoadingDialog();
		//Create the request object
		var request = new XMLHttpRequest();
		//Set the callback handler
		request.onreadystatechange = function() {
			//Check the state
			if(request.readyState == 4) {
				//Response
				var response;
				//Catch any server errors
				try {
					//Decode the response
					response = JSON.parse(request.responseText);
				} catch(e) {
					//JSON decode failure
					response = {"result": "error", "message": "Invalid Response", "detail": "Server returned invalid JSON"};
					//Save details
					Actions.rawResponse = request;
				}
				//Close the loading icon
				loading.close();
				//Check for error
				if(response["result"] == "error") {
					//Log
					console.log("Request failure: Displaying error: Message: \"" + response["message"] + "\" Detail: \"" + response["detail"] + "\"");
					//Create erorr dialog
					var container = document.createElement("div");
					//Title
					var title = document.createElement("div");
					title.className = "cardname";
					title.innerHTML = "Error";
					//Advanced message
					var message = "This is an advanced debug message - it probably won't mean a whole lot to you unless you're a developer.\n\n";
					message += "----------------------------------------\n\n";
					message += "Request URL: " + request.responseURL + "\n";
					message += "Status: " + request.status + "\n";
					message += "Status Text: " + request.statusText + "\n\n";
					message += "----------------------------------------\n\n";
					message += "Raw Response Body:\n\n";
					message += request.responseText;
					//Error Message
					var errorMessage = Util.generateErrorMessage(response["message"], response["detail"], message);
					//Retry button
					var retry = document.createElement("button");
					retry.innerHTML = "Retry";
					retry.className = "half_button";
					//Close button
					var close = document.createElement("button");
					close.innerHTML = "Close";
					close.className = "half_button";
					//Add to container
					container.appendChild(title);
					container.appendChild(errorMessage);
					container.appendChild(retry);
					container.appendChild(close);
					//Create dialog
					var dialog = new Dialog(container, true);
					//Set retry handler
					retry.onclick = function() {
						//Close this dialog
						dialog.close();
						//Re-execute action
						Actions.executeAction(url);
					}
					//Set close handler
					close.onclick = function() { dialog.close(); }
				}
			}
		}
		//Send the request
		request.open("GET", url, true);
		request.send(null);
	}
}