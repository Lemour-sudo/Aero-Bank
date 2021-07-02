CREATE TABLE client(
	account_number VARCHAR(20) PRIMARY KEY,
	name VARCHAR(20) NOT NULL,
	surname VARCHAR(20),
    email VARCHAR(30) NOT NULL UNIQUE,
    password VARCHAR(40)
);
DESCRIBE client;

CREATE TABLE account(
	account_number VARCHAR(20) PRIMARY KEY,
    cheque_balance DECIMAL(12, 2),
    savings_balance DECIMAL(12, 2),
    FOREIGN KEY(account_number) REFERENCES client(account_number) ON DELETE CASCADE
);
DESCRIBE account;

CREATE TABLE transaction(
	transaction_id INT PRIMARY KEY AUTO_INCREMENT,
	account_number VARCHAR(20) NOT NULL,
    cheque_deposit DECIMAL(12, 2),
    cheque_withdrawal DECIMAL(12, 2),
    cheque_balance DECIMAL(12, 2),
    savings_deposit DECIMAL(12, 2),
    savings_withdrawal DECIMAL(12, 2),
    savings_balance DECIMAL(12, 2),
    date_time DATETIME,
    FOREIGN KEY(account_number) REFERENCES account(account_number) ON DELETE CASCADE
);
DESCRIBE transaction;

DROP TABLE client;
DROP TABLE account;
DROP TABLE transaction;


