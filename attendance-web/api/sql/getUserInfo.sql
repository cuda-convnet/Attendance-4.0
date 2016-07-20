SELECT
	-- Get all of the user.* fields
	*,
	
	-- Get the users total time from the calendar table
	IFNULL((
		SELECT SUM(calendar.end - calendar.start)
		FROM calendar
		WHERE
			calendar.userid=users.id AND
			calendar.end!=0
	),0) AS 'totaltime',

	-- Get user times by source --
		-- Get source 0 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.source=0
		),0) AS 's_0',
		
		-- Get source 1 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.source=1
		),0) AS 's_1',

		-- Get source 2 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.source=2
		),0) AS 's_2',

		-- Get source 3 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.source=3
		),0) AS 's_3',

	-- Get user times by type --
		-- Get type 0 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.type=0
		),0) as 't_0',

		-- Get type 1 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.type=1
		),0) as 't_1',

		-- Get type 2 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.type=2
		),0) as 't_2',

		-- Get type 3 time
		IFNULL((
			SELECT SUM(calendar.end - calendar.start)
			FROM calendar
			WHERE
				calendar.userid=users.id AND
				calendar.end!=0 AND
				calendar.type=3
		),0) as 't_3',
	
	-- Check if the user is signed in (weather or not there is an "open" calendar event with their ID)
	IFNULL((
		SELECT 1
		FROM calendar
		WHERE
			calendar.userid=users.id AND
			calendar.end=0
	),0) AS 'signedin'
FROM users WHERE id=?