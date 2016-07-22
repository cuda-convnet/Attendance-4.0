class CardManager {
	//Method for loading the card state from a cookie
	static loadState() {
		//Get the state cookie
		var statecookie = Util.getCookie("cardstate");
		//Check if null
		if(statecookie == undefined) { statecookie = "CurrentUsers,RecentEvents,AllUsers"; }
		//Get the state cookie parts
		var statecookieparts =  statecookie.split(",");
		//Process the individual pieces
		for(var i = 0; i < statecookieparts.length; i++) {
			//Get the icon
			var icon = CardManager.getCardIcon(statecookieparts[i]);
			CardManager.maximize(icon);
		}
		//Save the state once again
		CardManager.saveState();
	}

	//Method for saving the card state
	static saveState() {
		//Cookie data
		var cdata = "";
		//Get the cards
		var cards = document.getElementById("cardbox").getElementsByTagName("div");
		//Check each card
		for(var i = 0; i < cards.length; i++) {
			//Check if the card is hidden
			if(cards[i].getAttribute("cardvisible") == "true") {
				//Add the card name to the list
				cdata += cards[i].getAttribute("cardname") + ",";
			}
		}
		//Save the cookie
		document.cookie = "cardstate=" + cdata.substring(0, cdata.length - 1);
		//Log
		console.log("Card state saved as " + cdata.substring(0, cdata.length - 1));
	}

	//Method for minimizing an element to the "taskbar"
	static minimize(card) {
		//Hide the element
		card.style.display = "none";
		//Set the attribute
		card.setAttribute("cardvisible","false");
		//Get the card icon
		var cardicon = CardManager.getCardIcon(card.getAttribute("cardname"));
		//Show the icon
		cardicon.style.display = "inline-block";
		//Save the card state
		CardManager.saveState();
	}

	//Method for showing an element that was hidden
	static maximize(icon) {
		//Validate
		if(!icon) { return; }
		//Hide the icon
		icon.style.display = "none";
		//Get the card
		var card = CardManager.getCard(icon.getAttribute("cardname"));
		//Move the card to the last slot in the box because that's fair
		document.getElementById("cardbox").appendChild(card);
		//Show the card
		card.style.display = "inherit";
		//Set the attribute
		card.setAttribute("cardvisible","true");
		//Save the card state
		CardManager.saveState();
	}

	//Method for getting a card icon element
	static getCardIcon(name) {
		//Get all of the card icons
		var all = document.getElementById("bottombar").getElementsByTagName("div");
		//Find the one
		for(var i = 0; i < all.length; i++) {
			//Check
			if(all[i].getAttribute("cardname") == name) {
				return all[i];
			}
		}
		//Nope
		return false;
	}

	//Method for getting a card by name
	static getCard(name) {
		//Get all of the cards
		var all = document.getElementById("cardbox").getElementsByTagName("div");
		//Find the one
		for(var i = 0; i < all.length; i++) {
			//Check
			if(all[i].getAttribute("cardname") == name) {
				return all[i];
			}
		}
		//Nope
		return false;
	}
}