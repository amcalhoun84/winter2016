<?php 
	/*Templating -- */
	$page_title = "Genre";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';
?>

<h1>Genres</h1>

<?php 


/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
	
	$choice = "SELECT * FROM genre";
	$result = $connection->query($choice);
	$num_rows = mysqli_num_rows($result);

/*	while($row = $result->fetch_array())			// For when we do individual picks.
	{
		$genre_id = $row['id'];
		$genre_name = $row['name'];
		$game_characteristics = $row['characteristics'];
	}
*/

	if($num_rows > 0)
	{

		while($row = $result->fetch_assoc())
		{
		echo '  <h3> Genre Name: ' . $row['name'] . ' </h3>
				<p>  Characteristics: ' . $row['characteristics'] . ' </h3><br /><br />';
				
		}
	}
	else {

		echo "Sorry, this is an empty query.";
	}

?>

		


	<?php $connection->close(); ?>
