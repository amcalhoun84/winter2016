<?php 
	/*Templating -- */
	$page_title = "Delete Platform";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

$entry = $_POST["delete_platform"];

// echo $entry;

$check_gp = $connection->query("SELECT * FROM game_platform WHERE platform_id = '" . $entry . "';");
$delete_call = "DELETE FROM platform WHERE platform_id = '" . $entry . "';";
// $foreign_key_on = "SET foreign_key_checks = 1;";

if(mysqli_num_rows($check_gp) > 0)
{
	$delete_gp = "DELETE FROM game_platform WHERE platform_id = '" . $entry . "';";
	if($connection->query($delete_gp) === TRUE) {
		echo "Game Platform  Released  <br />";
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
