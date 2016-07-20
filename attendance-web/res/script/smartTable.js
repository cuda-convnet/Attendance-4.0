class SmartTable {
	//Define the table constructor
	constructor(element) {
		//Check the element type
		if(element.constructor.name != "HTMLTableElement") {
			console.log("Invalid base element type: Expected HTMLTableElement");
			return;
		}
		//Save the base table
		this.baseTable = element;

		//Get the headers
		var rawHeaders = this.baseTable.getElementsByTagName("th");
		this.headers = [];
		for(var i = 0; i < rawHeaders.length; i++) {
			this.headers.push(rawHeaders[i].getAttribute("fieldname"));
		}

		//Get the actions
		var rawActions = this.baseTable.getElementsByTagName("action");
		this.actionCell = document.createElement("td");
		console.log(rawActions);
		for(var i = 0; i < rawActions.length; i++) {
			//Create the action icon
			var action = document.createElement("img");
			action.setAttribute("class", "action_icon");
			action.setAttribute("type", "image/svg+xml");
			action.setAttribute("src", "res/img/icon/" + rawActions[i].getAttribute("icon"));
			action.setAttribute("title", rawActions[i].getAttribute("tip"));
			action.setAttribute("onClick", rawActions[i].getAttribute("action"));
			//Add the icon to the action div
			this.actionCell.appendChild(action);
		}

		//Create the status row
		this.statusRow = document.createElement("tr");
		this.statusRow.className = "status_row";
		//Create the cells
		var statusContainerCell = document.createElement("td");
		statusContainerCell.className = "container_cell";
		statusContainerCell.colSpan = rawHeaders.length;
		this.statusCell = document.createElement("div");
		this.errorCell = document.createElement("div");
		this.loadingCell= document.createElement("div");
		//Add empty cells to the row
		this.statusRow.appendChild(statusContainerCell);
		statusContainerCell.appendChild(this.statusCell);
		statusContainerCell.appendChild(this.errorCell);
		statusContainerCell.appendChild(this.loadingCell);

		//Create the status cell
		this.statusCell.style.display = "none";
		this.statusCell.className = "status_cell";

		//Create the error placeholder
		this.errorCell.className = "error_cell";

		//Create theloading cell
		this.loadingCell.className = "loading_cell";
		var loadingImage = document.createElement("img");
		loadingImage.style.height = "40px";
		loadingImage.src= "res/img/loading.svg";
		this.loadingCell.appendChild(loadingImage);

		//Add the row to the table
		this.baseTable.appendChild(this.statusRow);
	}

	//Method for populating the table with data
	populate(data) {
		//Delete any stale data row
		var elements = this.baseTable.getElementsByTagName("tr");
		for(var i = 0; i < elements.length; i++) {
			//Check the element
			if(elements[i].hasAttribute("datarow")) {
				this.baseTable.removeChild(elements[i]);
			}
		}

		//Iterate through the data
		for(var row = 0; row < data.length; row++) {
			//Create the row
			var nrow = document.createElement("tr");
			//Set the row id
			nrow.setAttribute("datarow","true");
			//Add keys to the row as attributes
			for(var key in data[row]) {
				//Set as attribute
				nrow.setAttribute("raw_" + key, data[row][key]);
			}
			//Create cells
			for(var field = 0; field < this.headers.length; field++) {
				//Create the ecll
				var cell = document.createElement("td");
				//Get the field name
				var fieldname = this.headers[field];
				//Check for action
				if(fieldname == "_action") {
					//Special field: action
					cell = this.actionCell.cloneNode(true);
					cell.className = "action_cell";
				} else {
					//Fill the cell
					cell.innerHTML = data[row][fieldname];
				}
				//Add the cell to the row
				nrow.appendChild(cell);
			}
			//Add the row to the table
			this.baseTable.appendChild(nrow);
		}

		//Put the status row back at the bottom of the table
		this.baseTable.appendChild(this.statusRow);
	}

	//Method for displaying an error message
	setError(message, detail, advanced) {
		//Hide the other cells
		this.statusCell.style.display = "none";
		this.loadingCell.style.display = "none";
		//Create the error message
		this.errorCell.innerHTML = "";
		this.errorCell.appendChild(Util.generateErrorMessage(message, detail, advanced));
		//Show the error cell
		this.errorCell.style.display = "block";
	}

	//Method for setting the status message
	setStatus(message) {
		//Hide the other cells
		this.errorCell.style.display = "none";
		this.loadingCell.style.display = "none";
		//Set the status message
		this.statusCell.innerHTML = message;
		//Show the status cell
		this.statusCell.style.display = "block";
	}

	//Method for showing the loading icon on the table
	setLoading() {
		//Hide the other cells
		this.statusCell.style.display = "none";
		this.errorCell.style.display = "none";
	}
}