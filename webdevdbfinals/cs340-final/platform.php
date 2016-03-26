<?php 
	/*Templating -- */
	$page_title = 'Platforms';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

$results = $connection->query("SELECT * FROM platform");
$delete = $connection->query("SELECT * FROM platform");
$num_rows = mysqli_num_rows($results);
$i = 0;

?>

<h3>Add a platform:</h3>
	

	<form action="addplatform.php" method="post">
		<fieldset>
		<legend>Genre Information</legend>
		<label for="platform_name">Platform Name: </label>
		<input type="text" name="platform_name" id="platform_name" required /><br />
		<label for="platform_manufacturer">Platform Manufacturer: </label>
		<input type="text" name="platform_manufacturer" id="platform_manufacturer" required /><br />
		<label for="platform_generation">Platform Gen: </label>
		<input type="text" name="platform_generation" id="platform_generation" required /><br />
		<input type="submit" name="addplatform" id="addplatform" value = "Add Genre" />
		</fieldset></form>
	</form>


<h3>Delete a platform:</h3>
	
		<form action="deleteplatform.php" method="post">

		<fieldset>
		
			<label>Select Platform</label>
			<select name="delete_platform" id="delete_platform">
		<?php 

			if(mysqli_num_rows($delete) > 0)
			{
				while($row = $delete->fetch_assoc())
				{
					echo '<option value='. $row["platform_id"] .'>' . $row["name"] . '</option>';
				}
			}

		?>

			<br/><br/>
			<input type="submit" name="del_platform" id="del_platform" value = "Delete Genre" />

		</fieldset>


		</form>

<?php

	if($num_rows > 0)
	{
		while($row = $results->fetch_assoc())
		{

			$platform_id = $row["platform_id"];
			echo '  <h3> Platform : ' . $row['name'] . ' </h3>
			<p>  Manufacturer: ' . $row['manufacturer'] . '<br /> Generation: ' . $row['generation'] . ' </h3><br />
			<p> Games On Platform: </p>';
			
			$games_on_platform = "SELECT g.title AS gTitle, g.publisher, g.release_date, r.title AS rTitle FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) INNER JOIN game_platform gp ON (g.game_id = gp.game_id) INNER JOIN platform p ON (p.platform_id = gp.platform_id) WHERE p.platform_id = " . $row["platform_id"] . ";";
			$games = $connection->query($games_on_platform);
			$num_rows2 = mysqli_num_rows($games);

			if($num_rows2 > 0)
			{
				echo '<table>
					  <thead>
					  <tr>
						<th>Game</th> <th>Publisher</th><th> Release Date</th> <tr>Rating</th></tr>';

				while($sub_row = $games->fetch_assoc())
				{
					echo '<tr><td> ' . $sub_row["gTitle"] . '</td><td>' . $sub_row["publisher"] . '</td><td>' . $sub_row['release_date'] . '</td><td> ' . $sub_row['rTitle'] . '</td></tr>';
				}

				echo '</thead>
					  </table>';
			}

			else {
				echo "No games For That System Are In This Database";
			}

		}	
	}
	else {

		echo "Sorry, this is an empty query.";
	}

?>

