SELECT
	-- Basic event information
	calendar.id,
	calendar.user,
	calendar.start,
	calendar.end,
	calendar.meta,
	-- Checks if the event is open
	IF(calendar.end = 0, 1, 0) AS 'isopen',
	-- Gets the name of the user
	(
		SELECT
			CONCAT(users.fname, " ", users.lname)
		FROM users
		WHERE users.id = calendar.user
	) AS 'name'
WHERE user=?