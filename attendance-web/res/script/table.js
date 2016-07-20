//Table namespace
var Table = {
	"generateStatusRow": function(table) {
		//Create the row
		var status_row = document.createElement("tr");
		status_row.id = "status_row";

		//Create the container cell
		var container_cell = document.createElement("td");
		container_cell.colSpan = table.rows[0].cells.length;
		status_row.appendChild(container_cell);

		//Create the loading cell
		var loading_cell = document.createElement("div");
		loading_cell.id = "loading_cell";
		var loading_image = document.createElement("img");
		loading_image.style.height = "40px";
		loading_image.src = "res/img/loading.svg"
		loading_cell.appendChild(loading_image);
		container_cell.appendChild(loading_cell);

		//Create the error cell
		var error_cell = document.createElement("div");
		error_cell.id = "error_cell";
		var error_message = document.createElement("div");
		error_message.id = "error_message";
		error_cell.appendChild(error_message);
		var error_detail = document.createElement("div");
		error_detail.id = "error_detail";
		error_cell.appendChild(error_detail);
		container_cell.appendChild(error_cell);

		//Create the status cell
		var status_cell = document.createElement("div");
		status_cell.id = "status_cell";
		container_cell.appendChild(status_cell);

		//Debug
		error_message.innerHTML = "Test Message";
		error_detail.innerHTML = "Test Detail";
		status_cell.innerHTML = "Status is Statistical";

		//Append the row to the table
		table.appendChild(status_row);
	},

	//Generate status rows
	"generateStatusRows": function() {
		//Log
		console.log("Generating status rows...");
		//Find all tables
		var tables = document.getElementsByTagName("table");
		//Log
		console.log("Found " + tables.length + " tables...");
		//Iterate through each table
		for(var i = 0; i < tables.length; i++) {
			//Check if the table desires a status row
			if(tables[i].hasAttribute("statusrow")) {
				//Log
				console.log("Generating status row for table " + tables[i].id + "...");
				//Generate a status row
				Table.generateStatusRow(tables[i]);
			}
		}
	},
	
	//Populate a table
	"populate": function(table,data) {
		//Delete all existing rows
		var rows = table.getElementsByTagName("tr");
		//Iterate trough
		for(var i = 0; i < rows.length; i++) {
			//Check
			if(rows[i].hasAttribute("datarow")) {
				table.removeChild(rows[i]);
			}
		}
		//Check for error
		if(data['result'] == "error") {
			//Show the error
			Table.showError(table,data['message'],data['detail']);
			return -1;
		}
		//Get header row mapping
		var headercells = table.getElementsByTagName("th");
		var headers = [];
		//Calculate the headers
		for(var i = 0; i < headercells.length; i++) {
			headers.push(headercells[i].getAttribute("fieldname"));
		}
		//Populate the table
		for(var i = 0; i < data.length; i++) {
			//Create the row
			var row = document.createElement("tr");
			row.setAttribute("datarow","true");
			//Iterate through the headers
			for(var a = 0; a < headers.length; a++) {
				//Skip undefined headers
				if(headers[a] == undefined) { continue; }
				//Create the cell
				var cell = document.createElement("td");
				cell.innerHTML = data[i][headers[a]];
				row.appendChild(cell);
			}
			//Add the row to the table
			table.appendChild(row);
		}
		//Move the footer row to the bottom
		table.appendChild(table.getElementById("status_row"));
		//Return the number of entries processed
		return data.length;
	},

	//Set the error message
	"showError": function(table,message,detail) {
		//Get the error cell
		var error_cell = table.getElementById("error_cell");
		var error_message = error_cell.getElementById("error_message");
		var error_detail = error_cell.getElementById("error_detail");
		//Get the status and loading cells
		var status_cell = table.getElementById("status_cell");
		var loading_cell = table.getElementById("loading_cell");
		//Set the visibility
		error_cell.style.display = "block";
		status_cell.style.display = "none";
		loading_cell.style.display = "none";
		//Set the message and detail
		error_message.innerHTML = message;
		error_detail.innerHTML = detail;
	},

	//Set the status message
	"showStatus": function(table,message) {
		//Get the cells
		var error_cell = table.getElementById("error_cell");
		var status_cell = table.getElementById("status_cell");
		var loading_cell = table.getElementById("loading_cell");
		//Set the visibility
		error_cell.style.display = "none";
		status_cell.style.display = "block";
		loading_cell.style.display = "none";
		//Set the message
		status_cell.innerHTML = message;
	},

	//Reset to loading
	"showLoading": function(table) {
		//Get the cells
		var error_cell = table.getElementById("error_cell");
		var status_cell = table.getElementById("status_cell");
		var loading_cell = table.getElementById("loading_cell");
		//Set the visibility
		error_cell.style.display = "none";
		status_cell.style.display = "none";
		loading_cell.style.display = "block";
	}
}