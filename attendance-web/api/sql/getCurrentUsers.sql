-- Retrieves a list of current users
SELECT
	calendar.userid as 'id',
	(SELECT users.fname FROM users WHERE users.id=calendar.userid) as 'fname',
	(SELECT users.lname FROM users WHERE users.id=calendar.userid) as 'lname',
	calendar.start,
	calendar.end
FROM calendar
WHERE calendar.end = 0