<?php 

$page_title = "Add Genre";
error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'header.php';

$name = $_POST["genre_name"];
$characteristics = $_POST["genre_characteristics"];

if(!($stmt = $connection->prepare("INSERT INTO genre(name, characteristics) VALUES (?, ?);")))
{
	echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
}

if(!($stmt->bind_param("ss", $name, $characteristics)))
{
	echo "Bind failed: " . $stmt->errno . " " . $stmt->error;
}

if(!$stmt->execute())
{
	echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
}

else 
{
	echo "<br /><br />Genre table successfully updated!!!";
}

$stmt->close();

?>

