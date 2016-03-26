<?php 

$page_title = "Add Platform";
error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'header.php';

$name = $_POST["platform_name"];
$characteristics = $_POST["platform_manufacturer"];
$generation = $_POST["platform_generation"];

if(!($stmt = $connection->prepare("INSERT INTO platform(name, manufacturer, generation) VALUES (?, ?, ?);")))
{
	echo "Connection Error: " . $connection->errno . " " . $connection->connect_error;
}

if(!($stmt->bind_param("sss", $name, $manufacturer, $generation)))
{
	echo "Bind failed: " . $stmt->errno . " " . $stmt->error;
}

if(!$stmt->execute())
{
	echo "Execute failed: " . $stmt->errno . " " . $stmt->error;
}

else 
{
	echo "<br /><br />Platform table successfully updated!!!";
}

$stmt->close();

?>

