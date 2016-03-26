<?php 
	/*Templating -- */
	$page_title = 'Ratings';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';
?>


<h2>Rating
<?php 


/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
	
	$choice = "SELECT * FROM rating";
	$result = $connection->query($choice);
	$num_rows = mysqli_num_rows($result);

/*	while($row = $result->fetch_array())			// For when we do individual picks.
	{
		$rating_id = $row['id'];
		$rating_title = $row['title'];
		$rating_description = $row['description'];
		$rating_gameID = $row['game_id'];
	}
*/

	if($num_rows > 0)
	{

		while($row = $result->fetch_assoc())
		{
		echo '  <h3> Title: ' . $row['title'] . ' </h3>
				<p> Description: ' . $row['description'] . ' </p><br/><br/>';
		}
	}
	else {

		echo "Sorry, this is an empty query.";
	}

?>

		


	<?php $connection->close(); ?>
