<?php 
	/*Templating -- */
	$page_title = "Games";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

$entry = $_POST["delete_rev"];
// echo $entry; For debugging

/*
$connection->query("SET FOREIGN_KEY_CHECKS = 0;");
$connection->query("DELETE FROM game WHERE title=\"Aaron Test\"");
$connection->query("SET FOREIGN_KEY_CHECKS = ;");
*/

//$connection->query("SET FOREIGN_KEY_CHECKS = 0;");
$testQuery = "SELECT * FROM game WHERE title=\"Aaron Test 2\";";
$testRet = $connection->query($testQuery);
//$connection->query("SET FOREIGN_KEY_CHECKS = 1;");
if(mysqli_num_rows($testRet) > 0){
	while($row = mysqli_fetch_assoc($testRet)){
		echo "title: " . $row["title"] . " review ID: " . $row["review_id"];
	}
	
}

$delete_call = "SET FOREIGN_KEY_CHECKS = 0;";
$delete_call .= "DELETE FROM review where review_id = " . $entry . ";";
$delete_call .= "UPDATE game g SET g.review_id = '0' WHERE g.review_id = " . $entry . ";";
$delete_call .= "SET FOREIGN_KEY_CHECKS = 1;";

$check_rev = $connection->query("SELECT * FROM review WHERE review_id = '" . $entry . "';");
$check_game = $connection->query("SELECT review_id FROM game WHERE review_id = " . $entry . ";");


//$update_call = "SET FOREIGN_KEY_CHECKS = 0;";

//$update_call .= "SET FOREIGN_KEY_CHECKS = 1;";


if(mysqli_num_rows($check_rev) > 0)
{
	if($connection->multi_query($delete_call) === TRUE) {
		echo "Records deleted successfully <br />";
	} else {
    echo "Error deleting record: " . $connection->error . "<br />";
	}
}


?>