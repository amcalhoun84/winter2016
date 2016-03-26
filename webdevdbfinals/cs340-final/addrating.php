<?php 

$page_title = "Add Rating";
error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'header.php';



$title = $_POST["rating_title"];
$description = $_POST["rating_description"];
$query = "INSERT INTO rating(title, description) VALUES (\"" . $title . "\", \"" . $description . "\");";
/*
$connection->query("SET foreign_key_checks = 0");
$connection->query("DELETE FROM review WHERE text_review=\"delete this\"");
$connection->query("SET foreign_key_checks = 1");
*/

if(!($stmt = $connection->prepare("INSERT INTO rating(title, description) VALUES (?, ?);")));
{
//	echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
}

if(!($stmt->bind_param("ss", $title, $description)))
{
	echo "Bind failed: " . $stmt->errno . " " . $stmt->error;
}

if(!$stmt->execute())
{
	echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
}
else 
{
	echo "Executed Query Successfully.";
	$stmt->close();
}


?>

