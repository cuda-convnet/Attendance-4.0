<!DOCTYPE html>
<html>
	<head>
		<title>Ligerbots Attendance Management System</title>
		<link rel="stylesheet" type="text/css" href="res/style/pagestyle.css">
		<link rel="stylesheet" type="text/css" href="res/style/tablestyle.css">
		<link rel="stylesheet" type="text/css" href="res/style/alertstyle.css">
		<link rel="stylesheet" type="text/css" href="res/style/util.css">
		<link rel="stylesheet" type="text/css" href="res/style/pie.css">
		<link rel="stylesheet" type="text/css" href="res/style/cardmanager.css">
		<script src="res/script/dialog.js"></script>
		<script src="res/script/smartTable.js"></script>
		<script src="res/script/api.js"></script>
		<script src="res/script/actions.js"></script>
		<script src="res/script/util.js"></script>
		<script src="res/script/pie.js"></script>
		<script src="res/script/cardmanager.js"></script>
		<meta name="viewport" content="width=500">
	</head>
	<body>
		<!-- Top bar -->
		<div class="box topbar desktop">Ligerbots Attendance Management System</div>
		<div class="box topbar mobile">Ligerbots Attendance Management System [M]</div>

		<!-- Mason box -->
		<div class="masonbox" id="masonbox">
			<div class="column" id="masoncol1"></div>
			<div class="column" id="masoncol2"></div>
			<div class="column" id="masoncol3"></div>
		</div>

		<!-- Card Box -->
		<div class="cardbox" id="cardbox"></div>

		<!-- Inactive Card Box -->
		<div class="inactivecardbox" id="inactivecards">
			<!-- Current Users Box -->
			<div class="box card" cardname="CurrentUsers" style="height:400px">
				<!-- Card Title -->
				<div class="cardname">Current Users<img class="minicon" src="res/img/icon/down.svg" onClick="CardManager.minimize(this.parentElement.parentElement)"></div>
				<!-- User Table -->
				<table id="current_users_table" statusRow>
					<tr>
						<th min-width="150px" fieldname="name">Name</th>
						<th width="100px" fieldname="time">Time In</th>
						<th fieldname="_action">
							Actions
							<actions>
								<action tip="More Information"	icon="external.svg"	action="Actions.showUserInformationDialog(this);" />
								<action tip="Delete Event"		icon="rubbish.svg"	action="Actions.showDeleteCurrentEventDialog(this)" />
								<action tip="Sign Out"			icon="exit.svg"		action="Actions.showSignOutUserDialog(this)" />
							</actions>
						</th>
					</tr>
				</table>
			</div>

			<!-- Recent Events Box -->
			<div class="box card" cardname="RecentEvents" style="height:600px">
				<!-- Card Title -->
				<div class="cardname">Recent Events<img class="minicon" src="res/img/icon/down.svg" onClick="CardManager.minimize(this.parentElement.parentElement)"></div>
				<!-- Events Table -->
				<table id="recent_events_table" statusRow>
					<tr>
						<th fieldname="id">ID</th>
						<th width="100px" fieldname="type">Type</th>
						<th width="100px" fieldname="who">Who</th>
						<th fieldname="time">Time</th>
						<th fieldname="_action">
							Actions
							<actions>
								<action tip="More Information" icon="external.svg" action="newAlert('More Information','hello world')" />
							</actions>
						</th>
					</tr>
				</table>
			</div>

			<!-- All Users Box -->
			<div class="box card" cardname="AllUsers">
				<!-- Card Title -->
				<div class="cardname">All Users<img class="minicon" src="res/img/icon/down.svg" onClick="CardManager.minimize(this.parentElement.parentElement)"></div>
				<!-- Users Table -->
				<table id="all_users_table" statusRow>
					<tr>
						<th fieldname="id">ID</th>
						<th fieldname="name">Name</th>
						<th fieldname="pin">PIN</th>
						<th fieldname="rfid">RFID</th>
						<th fieldname="_action">
							Actions
							<actions>
								<action tip="More Information"	icon="external.svg"	action="newAlert('More Information','hello world')" />
								<action tip="Delete User"		icon="rubbish.svg"	action="newAlert('Delete User','hello world')" />
							</actions>
						</th>
					</tr>
				</table>
			</div>

			<!-- Add User Box -->
			<div class="box card" cardname="AddUser">
				<!-- Card Title -->
				<div class="cardname">Add User<img class="minicon" src="res/img/icon/down.svg" onClick="CardManager.minimize(this.parentElement.parentElement)"></div>
				<!-- Form -->
				<table class="addusertable">
					<tr>
						<td>First Name:</td>
						<td class="inputcell"><input name="fname"></td>
					</tr>
					<tr>
						<td>Last Name:</td>
						<td class="inputcell"><input name="lname"></td>
					</tr>
					<tr>
						<td>Email:</td>
						<td class="inputcell"><input name="email"></td>
					</tr>
				</table>
			</div>
		</div>

		<!-- Card list box -->
		<div class="box bottombar" id="bottombar">
			<div class="cardicon" cardname="CurrentUsers" onClick="CardManager.maximize(this)"><g>Current Users</g></div>
			<div class="cardicon" cardname="RecentEvents" onClick="CardManager.maximize(this)"><g>Recent Events</g></div>
			<div class="cardicon" cardname="AllUsers"	  onClick="CardManager.maximize(this)"><g>All Users</g></div>
			<div class="cardicon" cardname="AddUser"	  onClick="CardManager.maximize(this)"><g>Add User</g></div>
		</div>

		<!-- Scripts to run -->
		<script>
			//Generate smart tables
			var current_users = new SmartTable(document.getElementById("current_users_table"));
			var recent_events = new SmartTable(document.getElementById("recent_events_table"));
			var all_users = new SmartTable(document.getElementById("all_users_table"));
			//Load the cards
			CardManager.loadState();
			//API load
			API.getCurrentUsers(current_users);
			API.getRecentEvents(recent_events);
			API.getAllUsers(all_users);
			//Test alert
			//newAlert("Test Message","You are ugly");
		</script>
	</body>
</html>