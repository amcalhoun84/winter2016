<?php 


$page_title = 'get genre';
error_reporting(E_ALL);
ini_set('display_error', 'on');
include 'header.php';

$genre_call = $_POST["get_genre"];
$query = "SELECT * FROM genre WHERE genre_id=\"" . $genre_call . "\";";
$row_get = $connection->query($query);
if(!$row_get){
	echo "The query did not work";
}

if(mysqli_num_rows($row_get) > 0)
{
	echo "</br><table><tr><th>Genre Name</th><th>Genre Characteristics</th></tr>";
	while($row = $row_get->fetch_assoc())
	{
		echo "<tr><td>" . $row["name"] . "</td><td>" . $row["characteristics"] . "</td></tr>";
	}
	echo "</table>";
}

?>