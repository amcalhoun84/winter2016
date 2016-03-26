<?php 
	/*Templating -- */
	$page_title = 'Index and Registry Page';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

	$game_sql = "SELECT g.game_id, g.title AS 'game_title', g.publisher, g.release_date, r.title AS 'rating_title' FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) GROUP BY g.game_id ASC";
	$genre_sql = "SELECT * FROM genre";
	$platform_sql = "SELECT * FROM platform";
	
	$add_review_sql = "SELECT g.game_id, g.title FROM game g";
	$review_sql = "SELECT * FROM review r INNER JOIN game g ON g.game_id = r.game_id";
	$rating_sql = "SELECT * FROM rating";


	$game_result = $connection->query($game_sql);
	$genre_result = $connection->query($genre_sql);
	$game_genre = $connection->query($genre_sql);
	$platform_result = $connection->query($platform_sql);
	$rating_result = $connection->query($rating_sql);
	$add_review = $connection->query($add_review_sql);
	$get_genre = $connection->query($genre_sql);
	$review_result = $connection->query($review_sql); ?>


	<p>Please select a link to look up a specific category.</p>
	</ul>


	<?php if(mysqli_num_rows($game_result) > 0)
	{
		while($row = $game_result->fetch_assoc())
		{
			echo "id:" . $row["game_id"] . " - Name: " . $row["game_title"] . " - Publisher: " . $row["publisher"] . " - Release Date:" . $row["release_date"] . "- Rating: " . $row["rating_title"] . "<br />";
		}
	}
	else
		{
			echo "Nope. Empty query.";
		}
	?>

	<br/><br/>

	<h3>Add a game:</h3>
	

	<form action="addgame.php" method="post">
		<fieldset>
		<legend>Game Information</legend>
		<label for="game_title">Game Title: </label>
		<input type="text" name="game_title" id="game_title" /><br />
		<label for="game_publisher">Publisher: </label>
		<input type="text" name="game_publisher" id="game_publsher" /><br />
		<label for="release_date">Release Date: </label>
		<input type="date" name="release_date" id="release_date" /><br />
		<label for="rating_id">Game Rating: </label>
					<select name="rating_id" id="rating_id">
						<option value="NULL">NULL</option>
						<option value="1">E</option>
						<option value="2">E10+</option>
						<option value="3">T</option>
						<option value="4">M</option>
						<option value="5">AO</option> <br /><br />
					</select><br /><br />
		<label>Game Genres:</label><br />
		<input type="checkbox" name="genres[]" id="genres[]" value="1">Action
		<input type="checkbox" name="genres[]" id="genres[]" value="2">First Person Shooter
		<input type="checkbox" name="genres[]" id="genres[]" value="3">Strategy
		<input type="checkbox" name="genres[]" id="genres[]" value="4">Casual<br />
		<input type="checkbox" name="genres[]" id="genres[]" value="5">Roleplaying Game
		<input type="checkbox" name="genres[]" id="genres[]" value="6">Sports
		<input type="checkbox" name="genres[]" id="genres[]" value="7">Simulation
		<input type="checkbox" name="genres[]" id="genres[]" value="8">Adventure<br />
		<input type="checkbox" name="genres[]" id="genres[]" value="9">Driving
		<input type="checkbox" name="genres[]" id="genres[]" value="10">War
		<input type="checkbox" name="genres[]" id="genres[]" value="11">Fighting
		<br />
		<br /><br/>

		<label>Platform Availability:</label><br />
		<input type="checkbox" name="platforms[]" id="platforms[]" value="1">PC 
		<input type="checkbox" name="platforms[]" id="platforms[]" value="2">Apple 
		<input type="checkbox" name="platforms[]" id="platforms[]" value="3">Playstation 4 
		<input type="checkbox" name="platforms[]" id="platforms[]" value="4">Xbox One<br />
		<input type="checkbox" name="platforms[]" id="platforms[]" value="5">Wii U
		<input type="checkbox" name="platforms[]" id="platforms[]" value="6">Playstation 3
		<input type="checkbox" name="platforms[]" id="platforms[]" value="7">Xbox 360
		<input type="checkbox" name="platforms[]" id="platforms[]" value="8">Wii<br />
		<input type="checkbox" name="platforms[]" id="platforms[]" value="9">Xbox
		<input type="checkbox" name="platforms[]" id="platforms[]" value="10">GameCube
		<input type="checkbox" name="platforms[]" id="platforms[]" value="11">Playstation 2
		<input type="checkbox" name="platforms[]" id="platforms[]" value="12">Dreamcast<br />
		<input type="checkbox" name="platforms[]" id="platforms[]" value="13">Nintentdo 64
		<input type="checkbox" name="platforms[]" id="platforms[]" value="14">Playstation
		<br /><br />

			<input type="submit" name="addgame" id="addgame" value="SUBMIT" />


		

		</fieldset>
	</form>

<br /><br />

	<h3>Delete a game:</h3>
	

	<form action="deletegame.php" method="post">
		<fieldset>
		<legend>Game Information</legend>
		<label for="game_title">Game Title: </label>
		<input type="text" name="game_title" id="game_title" /><br />
		<label for="game_publisher">Publisher: </label>
		<input type="text" name="game_publisher" id="game_publsher" /><br />
		<label for="release_date">Release Date: </label>
		<input type="date" name="release_date" id="release_date" /><br />
		<label for="rating_id">Game Rating: </label>
					<select name="rating_id" id="rating_id">
						<option value="NULL">NULL</option>
						<option value="1">E</option>
						<option value="2">E10+</option>
						<option value="3">T</option>
						<option value="4">M</option>
						<option value="5">AO</option> <br /><br />
					</select><br />
		<label>Primary Game Genre:</label>
		<select name="game_genre" id="game_genre">

		<?php 

	
							
				if(mysqli_num_rows($game_genre) > 0)
				{
						while($row = $game_genre->fetch_assoc())
						{
						
							echo '<option value="' . $row["genre_id"] . '">' . $row["name"] . '</option>';
						}
					}
			?>
			</select> <br />    
			<input type="submit" name="addgame" id="addgame" value="SUBMIT" />

		</fieldset>
	</form>

	

	<h1>Genre:</h1>


	<?php 
	echo '<form action="getgenre.php" method="post"><fieldset>
			<legend>Get Game By Genre</legend>';

			echo '<select name="get_genre" id="get_genre">';
			
				
			if(mysqli_num_rows($get_genre) > 0)
			{
					while($row = $get_genre->fetch_assoc())
					{
						
						echo '<option value="' . $row["genre_id"] . '">' . $row["name"] . '</option>';
					}
				}
				

			echo '</select>';
 			echo '<input type="submit" name="getGen" id="getGen" />';

 			echo '</fieldset>';


	echo '</form>';


	if(mysqli_num_rows($genre_result) > 0)
	{
		while($row = $genre_result->fetch_assoc())
		{
			echo "id:" . $row["genre_id"] . " - Name: " . $row["name"] . " - Characteristics: " . $row["characteristics"] . "<br /><br />";
		}
	}
	else
	{
			echo "Nope. Empty query.";
	}

	?>

	

	<h1>Platforms:</h1>
	
	<?php 

	if(mysqli_num_rows($platform_result) > 0)
	{
		while($row = mysqli_fetch_assoc($platform_result))
		{
			echo "id:" . $row["platform_id"] . " - Name: " . $row["name"] . " - Manufacturer: " . $row["manufacturer"] . " - Generation: " . $row["generation"] . "<br /><br />";
		}
	}
	else
	{
		echo "Nope. Empty query.";
	}

	?>

	<h1>Reviews:</h1>
	<?php 
		echo "<form action='addreview.php' method='post'>";
		echo "<fieldset>";
		echo "<legend>Add Review</legend>";
		echo "<label>Game Name: </label>";
		echo "<select name='select_game' id='select_game'>";
		
		if(mysqli_num_rows($add_review) > 0)
		{
			while($row = mysqli_fetch_assoc($add_review))
			{
			echo '<option value="' .$row["game_id"] .'">' . $row["title"] . '</option>';
			}
		}

		else
		{
			echo 'Empty Query';
		}

	

	echo "</select>";
	echo "<br/ ><br />";
	echo "<label>Star Rating: </label>";
	echo "<select name='star_rating' id='star_rating'>";
	echo "<option value='NULL'>NULL</option>";
	for($i = 0; $i <= 10.0; $i += 0.1)
	{
		echo '<option value="'.$i.'">' .$i . '</option>';
	}
	echo "</select> <br /><br />";

	echo "<label>Review Text: </label>";
	echo "<input type='textbox' name='text_review' id='text_review'>";
	//echo "<input type='hidden' name='game_id' id='game_id'>";
	echo "<input type='submit' name='addreview' id='addreview' value='SUBMIT' /> </fieldset></form>";


	if(mysqli_num_rows($review_result) > 0)
	{
		while($row = mysqli_fetch_assoc($review_result))
		{
			echo "id:" . $row["review_id"] . " - Title: " . $row["title"] . " - Star Rating: " . $row["star_rating"] . " - Text Review: " . $row["text_review"] . "<br /><br />";
		}
	}
	else
		{
			echo "Nope. Empty query.";
		}

	?>

<br /><br />

<?php include 'footer.php'; ?>

</body>
</html>
