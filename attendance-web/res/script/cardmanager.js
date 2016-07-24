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
		var cards = CardManager.getCardsOrdered();
		//Check each card
		for(var i = 0; i < cards.length; i++) {
			//Add the card name to the list
			cdata += cards[i].getAttribute("cardname") + ",";
		}
		//Save the cookie
		document.cookie = "cardstate=" + cdata.substring(0, cdata.length - 1);
		//Log
		console.log("Card state saved as " + cdata.substring(0, cdata.length - 1));
	}

	//Method for minimizing an element to the "taskbar"
	static minimize(card) {
		//Move the element to the inactive card box
		document.getElementById("inactivecards").appendChild(card);
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
		CardManager.placeCard(card);
		//Set the attribute
		card.setAttribute("cardvisible","true");
		//Save the card state
		CardManager.saveState();
		//Get the number of cards
		var cardcount = CardManager.getCardsByNumber().length;
		//Set the card number
		card.setAttribute("number",cardcount+1);
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
		var all = document.getElementById("inactivecards").children;
		//Find the one
		for(var i = 0; i < all.length; i++) {
			console.log(all[i].getAttribute("cardname"));
			//Check
			if(all[i].getAttribute("cardname") == name) {
				return all[i];
			}
		}
		//Nope
		return false;
	}

	//Method for placing a card
	static placeCard(card) {
		//Get the screen size
		if(window.innerWidth <= 600) {
			//Mobile mode
			document.getElementById("cardbox").appendChild(card);
		} else {
			//Get the three columns
			var c1 = document.getElementById("masoncol1");
			var c2 = document.getElementById("masoncol2");
			var c3 = document.getElementById("masoncol3");
			//Get the width (same for all)
			var width = c1.offsetWidth;
			//Get the height of each
			var h1 = c1.offsetHeight;
			var h2 = c2.offsetHeight;
			var h3 = c3.offsetHeight;
			//Find the shortest distance
			var d1 = Math.sqrt(width*width + h1*h1);
			var d2 = Math.sqrt(width*width + h2*h2);
			var d3 = Math.sqrt(width*width + h3*h3);
			//Find the smallest one
			if(d1 <= d2 && d1 <= d3) {
				//Add to first column
				c1.appendChild(card);
			} else if(d2 <= d1 && d2 <= d3) {
				//Add to second column
				c2.appendChild(card);
			} else {
				//Add to third column
				c3.appendChild(card);
			}
		}
	}

	//This method finds and returns every card
	static getCards() {
		//Cards
		var cards = [];
		//Get cards from the card box
		cards = cards.concat([].slice.call(document.getElementById("cardbox").children));
		//Get cards from the three mason columns
		cards = cards.concat([].slice.call(document.getElementById("masoncol1").children));
		cards = cards.concat([].slice.call(document.getElementById("masoncol2").children));
		cards = cards.concat([].slice.call(document.getElementById("masoncol3").children));
		//Get cards from the holding box
		cards = cards.concat([].slice.call(document.getElementById("inactivecards").children));
		//Return the cards
		return cards;
	}

	//This method finds and returns every card in order
	static getCardsOrdered() {
		//Cards
		var cards = [];
		//Get cards from the three columns
		var c1 = [].slice.call(document.getElementById("masoncol1").children);
		var c2 = [].slice.call(document.getElementById("masoncol2").children);
		var c3 = [].slice.call(document.getElementById("masoncol3").children);
		//Shuffle the cards in
		for(var i = 0; i < c1.length || i < c2.length || i < c3.length; i++) {
			//Add the next item from each column
			if(c1[i] != null) { cards.push(c1[i]); }
			if(c2[i] != null) { cards.push(c2[i]); }
			if(c3[i] != null) { cards.push(c3[i]); }
		}
		//Cardbox cards
		var cbcards = [].slice.call(document.getElementById("cardbox").children);
		//Add the cardbox cards
		cards = cards.concat(cbcards);
		//Return the cards
		return cards;
	}

	//This method returns an array of each visible card sorted by number
	static getCardsByNumber() {
		//Get the cards
		var cards = CardManager.getCards();
		//Result
		var result = [];
		//Iterate
		for(var i = 0; i < cards.length; i++) {
			//Check the card number
			if(cards[i].getAttribute("number") == result.length) {
				//Add the card to the array
				result.push(cards[i]);
				//Reset the counter and continue
				i = -1; continue;
			}
		}
		//Finished
		return result;
	}
}