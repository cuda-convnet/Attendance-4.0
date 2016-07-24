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
		var cards = Util.getCards();
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