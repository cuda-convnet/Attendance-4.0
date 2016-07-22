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

	static getCookie(name) {
		//Convert to cookie name
		var cname = name + "=";
		//Get the cookies
		var cookies = document.cookie.split(';');
		//Search for the cookie
		for(var i = 0; i < cookies.length; i++) {
			//Get the cookie
			var cookie = cookies[i];
			//Do this thing I found on the internet
			while(cookie.charAt(0) == ' ') {
				cookie = cookie.substring(1);
			}
			//Check the cookie
			if(cookie.indexOf(cname) == 0) {
				//Return the cookie data
				return cookie.substring(cname.length,cookie.length);
			}
		}
		//Cookie does not exist
		return undefined;
	}
}