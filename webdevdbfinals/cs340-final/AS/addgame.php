<?php 

error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'connect_db.php';

$game_title = $_POST["game_title"];
$game_publisher = $_POST["game_publisher"];
$release_date = $_POST["release_date"];
$rating_id = $_POST["rating_id"];

if(!($stmt = $connection->prepare("INSERT INTO game(title, publisher, release_date, rating_id) VALUES (?, ?, ?, ?);")))
{
	echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
}

if(!($stmt->bind_param("sssi", $game_title, $game_publisher, $release_date, $rating_id)))
{
	echo "Bind failed: " . $stmt->errno . " " . $stmt->error;
}

if(!$stmt->execute())
{
	echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
}

else 
{
	echo "<br /><br />Added game";
}

$stmt->close();

$new_game_id = "SELECT game_id FROM game WHERE title = '". addslashes($game_title) . "';";

$row_get = $connection->query($new_game_id);

if(mysqli_num_rows($row_get) > 0)
{
	while($row = $row_get->fetch_assoc())
	{
		$game_id = $row["game_id"];

		if(isset($_POST['genres']))
		{
			foreach($_POST['genres'] as $value) {

				echo $value . "<br />";
				$insert = "INSERT INTO `game_genre`(`genre_id`, `game_id`) VALUES (" .$value .", " . $game_id . ");";
				echo $insert . "<br />";
				if ($connection->query($insert) === TRUE) {
    			echo "New record created successfully";}
    			else
    			{
    				echo "Woops! Something didn\'t work. :\(";
    			}

			}
		}

		if(isset($_POST['platforms']))
		{
			foreach($_POST['platforms'] as $value){

				echo $value . "<br />";

				$insert = "INSERT INTO `game_platform`(`platform_id`, `game_id`) VALUES (". $value .", " . $game_id . ");";
				echo $insert . "<br />";
				// $connection->query($insert);
				if ($connection->query($insert) === TRUE) {
    			echo "New record created successfully";}
    			else
    			{
    				echo "Woops! Something didn\'t work. :\(";
    			}
				//echo "Platform added. <br />";
			}
		}


	}
}



?>

