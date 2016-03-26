-- Table Entries --

INSERT INTO genre(name, characteristics)
VALUE ('Action', 'A generic catch-all for any game that requires a high degree of coordination and reflexes. Breaks down into several sub-genres, such as platformers, first-person shooters, stealth, etc.');

INSERT INTO genre(name, characteristics)
VALUE ('First Person Shooter', 'First-person shooter video games, commonly known as FPSs, emphasize shooting and combat from the perspective of the character controlled by the player. This perspective is meant to give the player the feeling of "being there", and allows the player to focus on aiming');

INSERT INTO genre(name, characteristics)
VALUE ('Strategy', 'A generic catch-all for games that require both short and long term tactical and strategic thinking, management of resources, and planning. Sub-genres include civilization building, military simulation, real-time-strategy, and resource/corporate management titles.');

INSERT INTO genre(name, characteristics)
VALUE ('Casual', 'A catch-all for any game that can be played by non-enthusiasts and for short bits of time while providing a satisfying experience. Can include light puzzle games, mobile phone games, and any type of quick game that can be played in 15-30 minute bursts.');

INSERT INTO genre(name, characteristics)
VALUE ('Roleplaying Game', 'Games where the player assumes a high level of customization and control over the character in an often expansive and sprawling environment. This and action games are probably some of the most diverse over-arching genres. Can include fantasy, modern-era, sci-fi, steampunk, and other fantastic settings.');

INSERT INTO genre(name, characteristics)
VALUE ('Sports', 'A simulation of games played for sport, where one or more teams play competiviely against the other.');

INSERT INTO genre(name, characteristics)
VALUE ('Simulation', 'A simulation video game describes a diverse super-category of video games, generally designed to closely simulate aspects of a real or fictional reality.');

INSERT INTO genre(name, characteristics)
VALUE('Adventure', 'Adventure games were some of the earliest games created, beginning with the text adventure Colossal Cave Adventure in the 1970s. That game was originally titled simply "Adventure," and is the namesake of the genre. Over time, graphics have been introduced to the genre and the interface has evolved.');

INSERT INTO genre(name, characteristics)
VALUE('Other', 'Whatever we want it to be!');

-- PLATFORMS, anything prior to 7th Gen or Playstation 3/Xbox360/wii is used mostly for presentation purposes, and not for actual use in a
-- public environment. However, in a demonstration or personal use, especially if a user is cataloguing their collection of video games, they can
-- add older systems to accomodate older games. 

-- Some older games are legends, and do deserve to be catalogued somewhere. 

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Personal Computer', 'Various / Home-built', 'Variable On Internal Components');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Apple Mac-Family PCs', 'Apple', 'Variable On Internal Components');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Playstation 4', 'Sony', '8th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('XBox One', 'Microsoft', '8th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Wii U', 'Nintendo', '8th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Playstation 3', 'Sony', '7th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Xbox 360', 'Microsoft', '7th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Wii', 'Nintendo', '7th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Xbox', 'Microsoft', '6th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('GameCube', 'Nintendo', '6th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Playstation 2', 'Sony', '6th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Dreamcast', 'Sega', '6th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Nintendo 64', 'Nintendo', '5th');

INSERT INTO platform(name, manufacturer, generation)
VALUE ('Playstation', 'Sony', '5th');

-- RATINGS, this is merely based on the ESRB rating system. Other rating systems do exist, and in a perfect world, some games would have more than one rating depending on the country or area of regulation.

INSERT INTO rating(title, description)
VALUE ('E', 'Everyone, Content is generally suitable for all ages. May contain minimal cartoon, fantasy or mild violence and/or infrequent use of mild language. Equivalent to film rating G or milder PG.');

INSERT INTO rating(title, description)
VALUE ('E10+', 'Everyone 10+, Content is generally suitable for ages 10 and up. May contain more cartoon, fantasy or mild violence, mild language and/or minimal suggestive themes. Equivalent to film rating PG to mild PG-13.');

INSERT INTO rating(title, description)
VALUE ('T', 'Teen, Content is generally suitable for ages 13 and up. May contain violence, suggestive themes, crude humor, minimal blood, simulated gambling and/or infrequent use of strong language. Equivalent to Film Rating PG-13, can border into R territory.');

INSERT INTO rating(title, description)
VALUE ('M', 'Mature, Meant for adult audiences. Content is generally suitable for ages 17 and up. May contain intense violence, blood and gore, sexual content and/or strong language.. Equivalent to Film Rating R.');

INSERT INTO rating(title, description)
VALUE ('AO', 'Adults Only, Content suitable only for adults ages 18 and up. May include prolonged scenes of intense violence, graphic sexual content and/or gambling with real currency. Equivalent to Film Rating NC-17');

INSERT INTO rating(title, description)
VALUE ('EC', 'Early Childhood, this title is often awarded to educational and edutainment games designed for children under the age of 7. ');

-- 

INSERT INTO game(title, publisher, release_date, rating_id)
VALUES ('Skyrim', 'Bethesda Softworks', 2011-11-11, 3);

INSERT INTO game(title, publisher, release_date, rating_id)
VALUES ('World of Warcraft', 'Blizzard Entertainment', 2004-11-05, 3);

INSERT INTO game(title, publisher, release_date, rating_id)
VALUES ('Assassin\'s Creed', 'UbiSoft', 2007-11-14, 4);
