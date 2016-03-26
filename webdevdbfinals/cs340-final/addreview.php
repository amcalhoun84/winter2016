<?php 

$page_title = "Add Review";

error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'header.php';


$star_rating = $_POST["star_rating"];
$text_review = $_POST["text_review"];
$game_id = $_POST["select_game"];
$query = "INSERT INTO review(star_rating, text_review, game_id) VALUES (\"" . $star_rating . "\", \"" . $text_review . "\", \"" . $game_id . "\");";
$review_check = $connection->query("SELECT review_id FROM game g WHERE game_id = " . $game_id . ";");
$review_chk = mysqli_num_rows($review_check);
$double_check = mysqli_fetch_assoc($review_check);

//echo $game_id . "<br />";
//echo $review_chk;
echo $double_check["review_id"];

/*
$connection->query("SET foreign_key_checks = 0");
$connection->query("DELETE FROM review WHERE text_review=\"delete this\"");
$connection->query("SET foreign_key_checks = 1");
*/

if($double_check["review_id"] != '' and $double_check["review_id"] != '0')
{
	echo "Review for this game already exists.";
}

else {

	if(!($stmt = $connection->prepare("INSERT INTO review(star_rating, text_review, game_id) VALUES (?, ?, ?);")));
	{
		//echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
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
		$stmt->close();
		$reviewId = $connection->query("SELECT review_id FROM review WHERE game_id=\"" . $game_id . "\"");
		$usable_id = mysqli_fetch_assoc($reviewId);	
		if(!($stmt1 = $connection->prepare("UPDATE game SET review_id=\"" . $usable_id["review_id"] . "\" WHERE game_id=\"" . $game_id . "\"")))
		{
			 echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
		}

		$connection->query("SET foreign_key_checks = 0");

		if(!$stmt1->execute())
		{
			echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
			$connection->query("SET foreign_key_checks = 1");
		}
		else
		{
			$connection->query("SET foreign_key_checks = 1");
			echo "<br /><br />Added review";
		}
		$stmt1->close();
	}
}


?>

