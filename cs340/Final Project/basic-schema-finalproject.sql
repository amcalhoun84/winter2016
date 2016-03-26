# FINAL PROJECT BASIC SCHEMA DRAFT I

SET foreign_key_checks = 0;
DROP TABLE IF EXISTS `game`, `genre`, `platform`, `review`, `rating`;
SET foreign_key_checks = 1;

-- table Genre
-- Attributes: ID, name, characteristics
-- id int (11) - PRIMARY, AUTO_INCREMENT
-- name varchar(255)
-- characteristics text
-- Foreign Key: game_id referenced id in game
-- table Game
-- Attributes: ID, int(11), (Primary Key, Auto_Increment)
--			   Title, varchar (255) (Not Null?)
--			   Publisher, varchar (255),
--			   Release Date DATE
-- 				NO FOREIGN KEYS ON THIS ONE - BREAKS THE THING
#--			   Foreign Key: platform_id references id in platform
#--			   Foreign Key: genre_id references id in genre
#--			   Foreign Key: rating_id references id in rating
#--			   Foreign Key: review_id references id in review
--			   Do you feel a unique key would be useful here?


CREATE TABLE `game`(
`game_id` int(11) PRIMARY KEY AUTO_INCREMENT,
`title` varchar(255), 
`publisher` varchar(255),
`release_date` date NOT NULL,
`platform_id` int(11) NOT NULL,
`genre_id` int(11) NOT NULL,
`rating_id` int(11) NOT NULL,
`review_id` int(11) NOT NULL

-- Move these to a few alter statements to get the table double linked up.
-- FOREIGN KEY(`platform_id`) REFERENCES `platform`(`platform_id`),
-- FOREIGN KEY(`genre_id`) REFERENCES `genre`(`genre_id`),
-- FOREIGN KEY(`rating_id`) REFERENCES `rating`(`rating_id`),
-- FOREIGN KEY(`review_id`) REFERENCES `review`(`review_id`)
) ENGINE = 'innoDB';



CREATE TABLE `genre`(
`genre_id` int(11) PRIMARY KEY AUTO_INCREMENT,
`name` varchar(255),
`characteristics` text,
`game_id` int(11) NOT NULL,
FOREIGN KEY(`game_id`) REFERENCES `game`(`game_id`)
) ENGINE = 'innoDB';

-- table platform
-- Attributes: ID, Name, Manufacturer, Generation
-- Foreign Key: game_id references id in game

CREATE TABLE `platform`(
`platform_id` int(11) PRIMARY KEY AUTO_INCREMENT,
`name` varchar(255),
`manufacturer` varchar(255),
`generation` varchar(255),
`game_id` int(11) NOT NULL,
FOREIGN KEY(`game_id`) REFERENCES `game`(`game_id`)
) ENGINE = 'innoDB';

-- table REVIEW
-- ATTRIBUTES: id, star_rating, text_review, game_id(FK)
CREATE TABLE `review`(
`review_id` int(11) PRIMARY KEY AUTO_INCREMENT,
`star_rating` smallint(3),
`text_review` text,
`game_id` int(11) NOT NULL,
FOREIGN KEY(`game_id`) REFERENCES `game`(`game_id`)
) ENGINE = 'innoDB';

-- table RATING
-- Attributes: ID, title, description, game_id
CREATE table `rating`(
`rating_id` int(11) PRIMARY KEY AUTO_INCREMENT,
`title` varchar(255),
`description` text,
`game_id` int(11) NOT NULL,
FOREIGN KEY(`game_id`) REFERENCES `game`(`game_id`)
) ENGINE = 'innoDB';




ALTER TABLE game
ADD FOREIGN KEY(platform_id) REFERENCES platform(platform_id),
ADD FOREIGN KEY(genre_id) REFERENCES genre(genre_id),
ADD FOREIGN KEY(rating_id) REFERENCES rating(rating_id),
ADD FOREIGN KEY(review_id) REFERENCES review(review_id);

