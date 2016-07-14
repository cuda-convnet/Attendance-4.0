//Sign out action button
var abSignout = document.createElement("span");
abSignout.title = "Sign the user out (granting credit up to this time)";
abSignout.style.color = "green";
abSignout.className = "action_button";
abSignout.innerHTML = "a";
abSignout.setAttribute("onClick","signOutUser(this.parentElement.getAttribute('user_id'),true)");
//Sign out without credit action button
var abSignoutNC = document.createElement("span");
abSignoutNC.title = "Sign the user out (without granting credit)";
abSignoutNC.style.color = "red";
abSignoutNC.className = "action_button";
abSignoutNC.innerHTML = "d";
abSignoutNC.setAttribute("onClick","signOutUser(this.parentElement.getAttribute('user_id'),false)");
//Show user information action button
var abUserinfo = document.createElement("span");
abUserinfo.title = "Show user information";
abUserinfo.className = "action_button";
abUserinfo.innerHTML = "f";
abUserinfo.setAttribute("onClick","showUserInfo(this.parentElement.getAttribute('user_id'))");

//Loads users
function loadUsers(loadFunction) {
	//Get the request object
	var xmlHttp = new XMLHttpRequest();
	//Set the callback handler
	xmlHttp.onreadystatechange = function() {
		if(xmlHttp.readyState == 4 && xmlHttp.status == 200) {
			loadFunction(JSON.parse(xmlHttp.responseText));
		}
	}
	//Send the request
	xmlHttp.open("GET", "api/getUsers.php", true);
	xmlHttp.send(null);
}
function loadUsers_cb(response) {
	//Populate the current users table
	loadCurrentUsers(response);
	//Populate the all users table
	loadAllUsers(response);
}
function loadCurrentUsers(response) {
	//Check if error
	if(response.result == "error") {
		//Hide other cells
		document.getElementById("culoadingcell").style.display = "none";
		document.getElementById("custatuscell").style.display = "none";
		//Show the error cell
		var ecell = document.getElementById("cuerrorcell");
		ecell.innerHTML = response.message + " (" + response.detail + ")";
		ecell.style.display = "table-cell";
	} else {
		//Hide other cells
		document.getElementById("culoadingcell").style.display = "none";
		document.getElementById("cuerrorcell").style.display = "none;";

		//Get the table object
		var table = document.getElementById("cutable");

		//Delete existing rows
		var rows = table.getElementsByClassName("curow");
		for(var i = 0; i < rows.length; i++) {
			rows.item(i).parentElement.removeChild(rows.item(i));
		}

		//User counter
		var ucount = 0;

		//Process result
		for(var key in response) {
			//Check if this user is signed in
			if(response[key].signedin != "1") { continue; }

			//Increment uesr counter
			ucount++;

			//Create the new row
			var nrow = document.createElement("tr");
			nrow.className = "curow";

			//Create name cell
			var cellName = document.createElement("td");
			//Hide the border on the left
			cellName.style.borderLeft = "none";
			//Create the cell contents
			cellName.innerHTML = response[key].fname + " " + response[key].lname;
			//Add the cell to the row
			nrow.appendChild(cellName);

			//Create the time in cell
			var cellTime = document.createElement("td");
			//Create the cell contents
			cellTime.innerHTML = formatDate(new Date(parseInt(response[key].lastsignin)));
			//Add the cell to the row
			nrow.appendChild(cellTime);

			//Create the actions cell
			var cellActions = document.createElement("td");
			//Set the class name
			cellActions.className = "action_cell";
			//Set the uid attribute
			cellActions.setAttribute("user_id", key);
			//Add the buttons
			cellActions.appendChild(abSignout.cloneNode(true));
			cellActions.appendChild(abSignoutNC.cloneNode(true));
			cellActions.appendChild(abUserinfo.cloneNode(true));
			//Add the cell to the row
			nrow.appendChild(cellActions);

			//Add the row to the table
			table.appendChild(nrow);
		}

		//Make the status cell visible
		var statuscell = document.getElementById("custatuscell");
		statuscell.innerHTML = "There " + (ucount != 1 ? "are " : "is ") + ucount + " user" + (ucount != 1 ? "s " : " ") + "currently signed in";
		statuscell.style.display = "table-cell";
		//Move the status row to the bottom of the table
		table.appendChild(document.getElementById("custatusrow"));
	}
}

function loadAllUsers(response) {
	//Check if error
	if(response.result == "error") {
		//Hide other cells
		document.getElementById("auloadingcell").style.display = "none";
		document.getElementById("austatuscell").style.display = "none";
		//Show the error cell
		var ecell = document.getElementById("auerrorcell");
		ecell.innerHTML = response.message + " (" + response.detail + ")";
		ecell.style.display = "table-cell";
	} else {
		//Hide other cells
		document.getElementById("auloadingcell").style.display = "none";
		document.getElementById("auerrorcell").style.display = "none;";

		//Get the table object
		var table = document.getElementById("autable");

		//User counter
		var ucount = 0;

		//Process result
		for(var key in response) {
			//Increment uesr counter
			ucount++;

			//Create the new row
			var nrow = document.createElement("tr");

			//Create ID cell
			var cellId = document.createElement("td");
			//Hide the border on the left
			cellId.style.borderLeft = "none";
			//Create the cell contents
			cellId.innerHTML = key;
			//Add the cell to the row
			nrow.appendChild(cellId);

			//Create fname cell
			var cellFname = document.createElement("td");
			//Create the cell contents
			cellFname.innerHTML = response[key].fname;
			//Add the cell to the row
			nrow.appendChild(cellFname);

			//Create the lname cell
			var cellLname = document.createElement("td");
			//Create the cell contents
			cellLname.innerHTML = response[key].lname;
			//Add the cell to the row
			nrow.appendChild(cellLname);

			//Create the pin cell
			var cellPin = document.createElement("td");
			//Create the cell contents
			cellPin.innerHTML = response[key].pin;
			//Add the cell to the row
			nrow.appendChild(cellPin);

			//Create the RFID cell
			var cellRfid = document.createElement("td");
			//Create the cell contents
			cellRfid.innerHTML = response[key].rfid;
			//Add the cell to the row
			nrow.appendChild(cellRfid);

			//Create the actions cell
			var cellActions = document.createElement("td");
			//Set the class name
			cellActions.className = "action_cell";
			//Set the uid attribute
			cellActions.setAttribute("user_id", key);
			//Add the buttons
			cellActions.appendChild(abUserinfo.cloneNode(true));
			//Add the cell to the row
			nrow.appendChild(cellActions);

			//Add the row to the table
			table.appendChild(nrow);
		}

		//Make the status cell visible
		var statuscell = document.getElementById("austatuscell");
		statuscell.innerHTML = "There " + (ucount != 1 ? "are " : "is ") + ucount + " user" + (ucount != 1 ? "s" : "");
		statuscell.style.display = "table-cell";
		//Move the status row to the bottom of the table
		table.appendChild(document.getElementById("austatusrow"));
	}
}

function signOutUser(id,giveCredit) {
	//Get the request object
	var xmlHttp = new XMLHttpRequest();
	//Set the callback handler
	xmlHttp.onreadystatechange = function() {
		if(xmlHttp.readyState == 4 && xmlHttp.status == 200) {
			loadCurrentUsers(JSON.parse(xmlHttp.responseText));
		}
	}
	//Send the request
	xmlHttp.open("GET", "api/signOut.php?id=" + id + "&keepcredits=" + (giveCredit ? "true" : "false") + "&note=", true);
	xmlHttp.send(null);
}

function showUserInfo(id) {
	alert("Show user information for " + id);
}

loadUsers(loadUsers_cb);

function formatDate(d) {
	//Result string
	var result = "";
	//Add the date
	result += (d.getMonth() + 1);
	result += "/";
	result += d.getDate();
	result += " ";
	result += ((d.getHours() < 12) ? d.getHours() : d.getHours() - 12);
	result += ":";
	result += ((d.getMinutes() < 10) ? "0" + d.getMinutes() : d.getMinutes());
	result += " ";
	result += ((d.getHours() < 12) ? "AM" : "PM");
	//Return the result
	return result;
}