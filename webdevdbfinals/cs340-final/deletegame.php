<?php 
	/*Templating -- */
	$page_title = "Delete Games";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

$entry = $_POST["delete_game"];
// echo $entry;

//$foreign_key_off = "SET foreign_key_checks = 0;"; Non-functional, but left in to demonstrate thought process. Generally, don't do this. 														Release keys first!!!!

$check_gg = $connection->query("SELECT * FROM game_genre WHERE game_id = '" . $entry . "';");
$check_gp = $connection->query("SELECT * FROM game_platform WHERE game_id = '" . $entry . "';");
$check_rev = $connection->query("SELECT * FROM review WHERE game_id = '" . $entry . "';");
$delete_call = "DELETE FROM game WHERE game.game_id = '" . $entry . "';";
// $foreign_key_on = "SET foreign_key_checks = 1;";

if(mysqli_num_rows($check_gg) === 0)
{
	echo "No records to delete from game_genres";
}

if(mysqli_num_rows($check_gg) > 0)
{
	$delete_gg = "DELETE FROM game_genre WHERE game_id = '" . $entry . "';";
	if($connection->query($delete_gg) === TRUE) {
		echo "Game Genres Released  <br />";
	}
	else
	{
		echo "Game Genres Not Released<br />";
	}
}

if(mysqli_num_rows($check_gp) === 0)
{
	echo "No records to delete from game_platform<br/>";
}

if(mysqli_num_rows($check_gp) > 0)
{
	$delete_gp = "DELETE FROM game_platform WHERE game_id = '" . $entry . "';";
	if($connection->query($delete_gp) === TRUE) {
		echo "Game Platforms Released <br />";
	}
	else
	{
		echo "Game Genres Not Platforms<br />";
	}
}


if ($connection->query($delete_call) === TRUE) {
    echo "Record deleted successfully <br />";
} else {
    echo "Error deleting record: " . $connection->error . "<br />";
}

if(mysqli_num_rows($check_rev) === 0)
{
	echo "No records to delete from review<br/>";
}

if(mysqli_num_rows($check_rev) > 0)
{
	$delete_rev = "DELETE FROM review WHERE game_id = '" . $entry . "';";
	if($connection->query($delete_rev) === TRUE) {
		echo "Record deleted successfully <br />";
	} else {
    echo "Error deleting record: " . $connection->error . "<br />";
	}
}