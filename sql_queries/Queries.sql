-- client queries
SELECT * FROM client;
SELECT * FROM client WHERE Name = 'Blue' LIMIT 1;

DELETE FROM client WHERE account_number = '1625143203';


-- account queries
SELECT * FROM account;


-- transaction queries
SELECT * FROM transaction;