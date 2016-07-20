class Util {
	static generateErrorMessage(emessage, edetail, eadvanced) {
		//Create the error message container
		var container = document.createElement("div");
		container.className = "error_container";
		//Create the error message
		var message = document.createElement("div");
		message.className = "error_message";
		message.innerHTML = emessage;
		container.appendChild(message);
		//Create the error detail
		var detail = document.createElement("div");
		detail.className = "error_detail";
		detail.innerHTML = edetail;
		container.appendChild(detail);
		//Create advanced button
		var advanced = document.createElement("div");
		advanced.className = "error_advanced";
		advanced.innerHTML = "Advanced";
		advanced.onclick = function() {
			Dialog.showAdvancedDetailDialog(eadvanced);
		}
		container.appendChild(advanced);
		//Return the generated error message container
		return container;
	}
}