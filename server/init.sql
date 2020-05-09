DROP DATABASE IF EXISTS test;
CREATE DATABASE test;
USE test;

DELIMITER //
DROP TABLE IF EXISTS rooms;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS roles;
//
DELIMITER ;

DELIMITER //
DROP TABLE IF EXISTS roles;
CREATE TABLE roles(
	roleID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
	role VARCHAR(30) NOT NULL UNIQUE
);
INSERT INTO roles (role) VALUES ('ADMIN');
INSERT INTO roles (role) VALUES ('USER');
//
DELIMITER ;

DELIMITER //
DROP TABLE IF EXISTS users;
CREATE TABLE users(
	username VARCHAR(25) NOT NULL PRIMARY KEY,
	password VARCHAR(25) NOT NULL,
	state VARCHAR(30),
	role INT NOT NULL,
	FOREIGN KEY (role) REFERENCES roles(roleID)
);
INSERT INTO users (username, password, role) VALUES ('admin', 'admin', 1);
INSERT INTO users (username, password, role) VALUES ('test1', '123', 2);
INSERT INTO users (username, password, role) VALUES ('test2', '123', 2);
INSERT INTO users (username, password, role) VALUES ('test3', '123', 2);
//
DELIMITER ;

DELIMITER //
DROP TABLE IF EXISTS rooms;
CREATE TABLE rooms(
	roomID VARCHAR(25) NOT NULL PRIMARY KEY,
	capacity INT NOT NULL
);
INSERT INTO rooms VALUES ('CBA', 20);
INSERT INTO rooms VALUES ('IMS', 30);
INSERT INTO rooms VALUES ('SBG', 40);
//
DELIMITER ;

DELIMITER //
DROP TABLE IF EXISTS messages;
CREATE TABLE messages(
	id INT NOT NULL AUTO_INCREMENT,
	sender VARCHAR(25) NOT NULL,
	receiver VARCHAR(25) NOT NULL,
	message TEXT NOT NULL,
	time DATETIME NOT NULL,
	PRIMARY KEY (id),
	FOREIGN KEY (sender) REFERENCES users(username),
	FOREIGN KEY (receiver) REFERENCES users(username)
);
//
DELIMITER ;

DELIMITER //
DROP FUNCTION IF EXISTS ValidateLogin;
CREATE FUNCTION ValidateLogin(
	p_username CHAR(25),
	p_password CHAR(25))
	RETURNS BOOL
BEGIN
RETURN EXISTS (SELECT * FROM users WHERE username = p_username AND password = p_password);
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS ClearState;
CREATE PROCEDURE ClearState(p_username CHAR(25))
BEGIN
UPDATE users 
SET state = NULL 
WHERE username = p_username;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS SetState;
CREATE PROCEDURE SetState(
	p_username CHAR(25),
	p_type INT,
	p_name CHAR(25))
BEGIN
UPDATE users
SET state = IF (p_type = 1, p_name, IF(p_type = 2, CONCAT('ROOM ', p_name), NULL))
WHERE username = p_username;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS saveMessage;
CREATE PROCEDURE saveMessage(
	p_sender CHAR(25),
	p_receiver CHAR(25),
	p_mess TEXT,
	p_time INTEGER)
BEGIN
INSERT INTO messages(sender, receiver, message, time) VALUES (p_sender, p_receiver, p_mess, FROM_UNIXTIME(p_time));
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS getMessageOfUser;
CREATE PROCEDURE getMessageOfUser(
	p_receiver CHAR(25))
BEGIN
SELECT sender, message, UNIX_TIMESTAMP(time) AS time FROM messages 
	WHERE receiver = p_receiver;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS countMessageOfUser;
CREATE PROCEDURE countMessageOfUser(
	p_receiver CHAR(25))
BEGIN
SELECT COUNT(*) FROM messages WHERE receiver = p_receiver;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS deleteMessageOfUser;
CREATE PROCEDURE deleteMessageOfUser(
	p_receiver CHAR(25))
BEGIN
DELETE FROM messages WHERE receiver = p_receiver;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS modifyUser;
CREATE PROCEDURE modifyUser(
	p_username CHAR(25),
	p_password CHAR(25),
	p_role INT)
BEGIN
UPDATE users 
SET 
	password = p_password,
	role = p_role
WHERE
	username = p_username;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS deleteUser;
CREATE PROCEDURE deleteUser(
	p_username CHAR(25))
BEGIN
DELETE FROM users
WHERE
	username = p_username;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS modifyRoom;
CREATE PROCEDURE modifyRoom(
	p_roomID CHAR(25),
	p_capacity INT)
BEGIN
UPDATE rooms 
SET 
	capacity = p_capacity
WHERE
	roomID = p_roomID;
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS deleteRoom;
CREATE PROCEDURE deleteRoom(
	p_roomID CHAR(25))
BEGIN
DELETE FROM rooms
WHERE
	roomID = p_roomID;
END //
DELIMITER ;

DELIMITER //
DROP FUNCTION IF EXISTS haveOfflineMessage;
CREATE FUNCTION haveOfflineMessage(
	p_username CHAR(25))
	RETURNS BOOL
BEGIN
RETURN EXISTS (SELECT * FROM messages WHERE sender = p_username OR receiver = p_username);
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS findUserWithName;
CREATE PROCEDURE findUserWithName(
	p_username CHAR(25))
BEGIN
SELECT * FROM users WHERE username LIKE CONCAT('%', p_username, '%');
END //
DELIMITER ;

DELIMITER //
DROP PROCEDURE IF EXISTS countUserWithName;
CREATE PROCEDURE countUserWithName(
	p_username CHAR(25))
BEGIN
SELECT COUNT(*) FROM users WHERE username LIKE CONCAT('%', p_username, '%');
END //
DELIMITER ;