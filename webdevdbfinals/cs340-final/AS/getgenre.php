<?php 

error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'connect_db.php';


$genre_call = $_POST["get_genre"];

$row_get = $connection->query("SELECT g.title, ge.name FROM game g INNER JOIN game_genre gg ON (gg.game_id = g.game_id) INNER JOIN genre ge ON (gg.genre_id = ge.genre_id) WHERE ge.genre_id = $genre_call");

if(mysqli_num_rows($row_get) > 0)
{
	while($row = $row_get->fetch_assoc())
	{
		echo "<br />" . $row["title"] . " - " . $row["name"];
	}

}

?>