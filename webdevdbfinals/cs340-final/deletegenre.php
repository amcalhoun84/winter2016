<?php 
	/*Templating -- */
	$page_title = "Delete Genre";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

$entry = $_POST["delete_genre"];
// echo $entry; DEBUGGING

//$foreign_key_off = "SET foreign_key_checks = 0;"; Non-functional, but left in to demonstrate thought process. Generally, don't do this. 														Release keys first!!!!

$check_gg = $connection->query("SELECT * FROM game_genre WHERE genre_id = '" . $entry . "';");
$delete_call = "DELETE FROM genre WHERE genre_id = '" . $entry . "';";
// $foreign_key_on = "SET foreign_key_checks = 1;";

if(mysqli_num_rows($check_gg) > 0)
{
	$delete_gg = "DELETE FROM game_genre WHERE genre_id = '" . $entry . "';";
	if($connection->query($delete_gg) === TRUE) {
		echo "Game Genres Released  <br />";
	}
	else
	{
		echo "Game Genres Not Released/No Records to Delete  <br />";
	}
}

if ($connection->query($delete_call) === TRUE) {
    echo "Record deleted successfully <br />";
} else {
    echo "Error deleting record: " . $connection->error . "<br />";
}
