<?php 
	/*Templating -- */
	$page_title ="Delete Rating";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';
	$page_title = "Delete Rating";

$entry = $_POST["delete_rating"];
// echo $entry;

//$foreign_key_off = "SET foreign_key_checks = 0;"; Non-functional, but left in to demonstrate thought process. Generally, don't do this. 														Release keys first!!!!

$check_rt = $connection->query("SELECT * FROM game WHERE rating_id = '" . $entry . "';");
$delete_call = "SET FOREIGN_KEY_CHECKS = 0;";
$delete_call .= "DELETE FROM rating WHERE rating_id = '" . $entry . "';";
$delete_call .= "UPDATE game g SET g.rating_id = '0' WHERE g.rating_id = " . $entry . ";";
$delete_call .= "SET FOREIGN_KEY_CHECKS = 1;";


// $foreign_key_on = "SET foreign_key_checks = 1;";

if(mysqli_num_rows($check_rt) < 1)
{
	echo "No records to delete. Deleting rating...";
	if($connection->multi_query($delete_call) === TRUE)
	{
		echo "Record deleted.";
	}
	else
	{
	   echo "Error deleting record: " . $connection->error . "<br />";
	}
}

if(mysqli_num_rows($check_rt) > 0)
{
	if($connection->multi_query($delete_call) === TRUE) {
	    echo "Record deleted successfully <br />";
	}
	else
	{
	   echo "Error deleting record: " . $connection->error . "<br />";
	}
}
