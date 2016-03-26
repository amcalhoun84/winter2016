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
	$delete = $connection->query($choice);
	$num_rows = mysqli_num_rows($result);
	$i = 1;

/*	while($row = $result->fetch_array())			// For when we do individual picks.
	{
		$rating_id = $row['id'];
		$rating_title = $row['title'];
		$rating_description = $row['description'];
		$rating_gameID = $row['game_id'];
	}
*/

?>

<h3>Add a Rating:</h3>
	

	<form action="addrating.php" method="post">
		<fieldset>
		<legend>Rating Information</legend>
		<label for="rating_title">Rating Title: </label>
		<input type="text" name="rating_title" id="rating_title" required /><br /><br />
		<label for="rating_description">Genre Characteristics: </label>
		<textarea rows="2" cols="50" name="rating_description" id="rating_description" required /></textarea><br />
		<input type="submit" name="addrating" id="addrating" value = "Add Genre" />
		</fieldset></form>
	</form>

	<h3>Delete a genre:</h3>
	
		<form action="deleterating.php" method="post">

		<fieldset>
		
			<label>Select Rating</label>
			<select name="delete_rating" id="delete_rating">

			<?php 

			if(mysqli_num_rows($delete) > 0)
			{
				while($row = $delete->fetch_assoc())
				{
					echo '<option value='. $row["rating_id"] .'>' . $row["title"] . '</option>';
				}
			}

		?>

			<br/><br/>
			<input type="submit" name="del_rate" id="del_rate" value = "Delete Rating" />

		</fieldset>


		</form>




<?php
if($num_rows > 0)
	{
		while($row = $result->fetch_assoc())
		{
			echo '  <h3> Rating Title : ' . $row['title'] . ' </h3>
			<p>  Description: ' . $row['description'] . ' </h3><br />
			<p> Games With Rating: </p>';
			
			$games_with_rating = "SELECT g.title as gTitle, r.title as rTitle FROM game g INNER JOIN rating r ON (g.rating_id = r.rating_id) WHERE g.rating_id = '" . $i . "'";
			$games = $connection->query($games_with_rating);
			$num_rows2 = mysqli_num_rows($games);

			if($num_rows2 > 0)
			{
				echo '<table>
					  <thead>
					  <tr>
						<th>Game</th> <th>Rating</th></tr>';

				while($sub_row = $games->fetch_assoc())
				{
					echo '<tr><td> ' . $sub_row["gTitle"] . '</td><td>' . $sub_row["rTitle"] . '</td><tr>';
				}

				echo '</thead>
					  </table>';
			}

			else {
				echo "No games with that Rating Are In This Database";
			}

			$i++;

		}	
	}
	else {

		echo "Sorry, this is an empty query.";
	}



?>

		


	<?php $connection->close(); 
	include('footer.php'); ?>
