<?php 

error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'connect_db.php';
/*
$star_rating = $_POST["star_rating"];
$text_review = $_POST["text_review"];
$game_id = $_POST["select_game"];

if(!($stmt = $connection->prepare("INSERT INTO review(star_rating, text_review, game_id) VALUES (?, ?, ?);")));
{
	// echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
}

if(!($stmt->bind_param("dsi", $star_rating, $text_review, $game_id)))
{
	echo "Bind failed: " . $stmt->errno . " " . $stmt->error;
}

if(!$stmt->execute())
{
	echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
}
else 
{
	#add review_id to the game table for the appropriate row
	$connection->query("UPDATE game SET review_id=(SELECT review_id FROM review WHERE game_id=" . $game_id . "\";);");
	
	echo "<br /><br />Added review";
}

$stmt->close();
*/

	if(!($connection->query("UPDATE game SET review_id=\"17\" WHERE game_id=\"17\""))){
		echo "error updating";
	}
?>

