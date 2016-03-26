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
	$delete = $connection->query($choice);
	$num_rows = mysqli_num_rows($result);
/*	while($row = $result->fetch_array())			// For when we do individual picks.
	{
		$genre_id = $row['id'];
		$genre_name = $row['name'];
		$game_characteristics = $row['characteristics'];
	}
*/

//	echo $result;

?>

<h3>Add a genre:</h3>
	

	<form action="addgenre.php" method="post">
		<fieldset>
		<legend>Genre Information</legend>
		<label for="genre_name">Genre Name: </label>
		<input type="text" name="genre_name" id="genre_name" required /><br />
		<label for="genre_characteristics">Genre Characteristics: </label>
		<textarea rows="4" cols="50" name="genre_characteristics" id="genre_characteristics" required /></textarea><br />
		<input type="submit" name="addgenre" id="addgenre" value = "Add Genre" />
		</fieldset></form>
	</form>


<h3>Delete a genre:</h3>
	
		<form action="deletegenre.php" method="post">

		<fieldset>
		
			<label>Select Genre</label>
			<select name="delete_genre" id="delete_genre">
		<?php 

			if(mysqli_num_rows($delete) > 0)
			{
				while($row = $delete->fetch_assoc())
				{
					echo '<option value='. $row["genre_id"] .'>' . $row["name"] . '</option>';
				}
			}

		?>

			<br/><br/>
			<input type="submit" name="del_genre" id="del_genre" value = "Delete Genre" />

		</fieldset>


		</form>
<?php 

	if($num_rows > 0)
	{
		while($row = $result->fetch_assoc())
		{
			echo '  <h3> Genre Name: ' . $row['name'] . ' </h3>
			<p>  Characteristics: ' . $row['characteristics'] . ' </h3><br />
			<p> Games In Genre: </p>';
			
			$games_in_genre = "SELECT g.title as gTitle, g.publisher, g.release_date, r.title as rTitle FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) INNER JOIN game_genre gg ON (gg.game_id = g.game_id) INNER JOIN genre ge ON (ge.genre_id = gg.genre_id) WHERE ge.genre_id = '" . $row["genre_id"] . "';";
			$games = $connection->query($games_in_genre);
			$num_rows2 = mysqli_num_rows($games);

			if($num_rows2 > 0)
			{
				echo '<table>
					  <thead>
					  <tr>
						<th>Title</th> <th>Publisher</th> <th>Release Date</th> <th>Rating </th></tr>';

				while($sub_row = $games->fetch_assoc())
				{
					echo '<tr><td> ' . $sub_row["gTitle"] . '</td><td>' . $sub_row["publisher"] . '</td><td>' . $sub_row["release_date"] . '</td><td>' . $sub_row["rTitle"] . '</td></tr>';
				}

				echo '</thead>
					  </table>';
			}

			else
			{
				echo "No games in this genre.";
			}

		}	
	}
	else {

		echo "Sorry, this is an empty query. <br/><br/>";
	}


 include('footer.php');  ?>



	<?php $connection->close(); ?>
