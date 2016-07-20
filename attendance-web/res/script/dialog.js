//Modal dialog box class
class Dialog {
	//Dialog constructor
	constructor(content,iscloseable) {
		//Check if the current dialog array exists
		if(Dialog.current == null) { Dialog.current = []; }
		//Set closeability
		this.closeable = iscloseable;
		//Create the alert container
		this.container = document.createElement("div");
		this.container.className = "alert_background";
		//Reference to self
		var self = this;
		//Append the close handler to the container
		this.container.onclick = function() { if(this.closeable) { self.close(); } };
		this.container.onkeydown = function() { }
		//Create the dialog box
		this.dialog = document.createElement("div");
		this.dialog.onclick = function(e) { e.stopPropagation(); }
		this.dialog.className = "alert_box";
		//Add the content to the dialog
		this.dialog.appendChild(content);
		//Add the dialog to the container
		this.container.appendChild(this.dialog);
		//Add the entire box to the document body
		document.getElementsByTagName("body")[0].appendChild(this.container);
		//Set current dialog to this
		Dialog.current.unshift(this);
	}

	//Close method
	close() {
		//Remove the container from the body
		this.container.parentElement.removeChild(this.container);
		//Destroy reference to this dialog
		Dialog.current.shift();
		//Debug
		console.log("Dialog closed, display stack is now ");
		console.log(Dialog.current);
	}

	//Makes the dialog big
	expand() {
		//Change the container properties
		this.dialog.className = "large_box";
	}

	//Alert method
	static alert(title, message) {
		//Create the alert container
		var container = document.createElement("div");
		//Create the alert title
		var etitle = document.createElement("div");
		etitle.className = "cardname";
		etitle.innerHTML = title;
		container.appendChild(etitle);
		//Set the alert content
		var emessage = document.createElement("div");
		emessage.className = "alert_message";
		emessage.innerHTML = message;
		container.appendChild(emessage);
		//Create the close button
		var close = document.createElement("button");
		close.className = "full_button";
		close.innerHTML = "OK";
		container.appendChild(close);
		//Create a new dialog
		var dialog = new Dialog(container,true);
		//Set close handlers
		close.onclick = function() { dialog.close() };
	}

	//Generate a loading dialog
	static createLoadingDialog() {
		//Create the container
		var container = document.createElement("div");
		//Create title
		var title = document.createElement("div");
		title.className = "cardname";
		title.innerHTML = "Loading...";
		container.appendChild(title);
		//Create the icon
		var icon = document.createElement("img");
		icon.className = "alert_message";
		icon.src = "res/img/loading.svg";
		container.appendChild(icon);
		//Create the dialog
		return new Dialog(container,false);
	}

	//Show advanced debug detail dialog
	static showAdvancedDetailDialog(detail) {
		//Create the container
		var container = document.createElement("div");
		//Create the title
		var title = document.createElement("div");
		title.className = "cardname";
		title.innerHTML = "Advanced Failure Detail";
		//Create the close button
		var close = document.createElement("button");
		close.innerHTML = "Close";
		close.className = "full_button";
		//Create the content
		var content = document.createElement("div");
		content.className = "debug_advanced";
		//Encode detail
		content.innerHTML = detail.replace(/&/g, '&amp;')
								  .replace(/"/g, '&quot;')
								  .replace(/'/g, '&#39;')
								  .replace(/</g, '&lt;')
								  .replace(/>/g, '&gt;')
								  .replace(/\n/g, '<br>')
								  .replace(/\s/g, '&nbsp;')
								  .replace(/\t/g, '&nbsp;&nbsp;&nbsp;&nbsp;');
		//Add to the container
		container.appendChild(title);
		container.appendChild(content);
		container.appendChild(close);
		//Create the dialog
		var dialog = new Dialog(container, true);
		dialog.expand();
		//Set the close button handler
		close.onclick = function() { dialog.close(); }
	}
}

//Create a listener for the escape key
document.onkeydown = function(evt) {
	//Get the event if not specified
	evt = evt || window.event;
	var isEscape = false;
	//Check if this is an escape key event
	if("key" in evt) {
		isEscape = evt.key == "Escape";
	} else {
		isEscape = evt.keyCode == 27;
	}
	//Final check for escape
	if(isEscape) {
		//Check if there is a current dialog
		if(Dialog.current[0] != null) {
			//Check if the dialog is closeable
			if(Dialog.current[0].closeable) {
				//Close the current dialog
				Dialog.current[0].close();
			}
		}
	}
};