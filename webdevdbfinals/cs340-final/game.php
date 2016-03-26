<?php 
	/*Templating -- */
	$page_title = 'Index and Registry Page';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

	$game_sql = "SELECT g.game_id, g.title AS gamename, g.publisher, g.release_date, r.title FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) ORDER BY game_id ASC";
	$rating_sql = "SELECT * FROM rating";
	$add_review_sql = "SELECT g.game_id, g.title FROM game g";
	$review_sql = "SELECT * FROM review r INNER JOIN game g ON g.game_id = r.game_id";
	$rating_sql = "SELECT * FROM rating";
	$platform_sql = "SELECT * FROM platform";
	$genre_sql = "SELECT * FROM genre";
	$add_review = $connection->query($review_sql);
	$add_review_game = $connection->query($add_review_sql);
	$game_result = $connection->query($game_sql);
	$platform_roll = $connection->query($platform_sql);
	$genre_roll = $connection->query($genre_sql);
	$review_result = $connection->query($review_sql); 
	$rating_result = $connection->query($rating_sql); ?>


	<p>Please select a link to look up a specific category.</p>
	</ul>


	<?php if(mysqli_num_rows($game_result) > 0)
	{
		while($row = $game_result->fetch_assoc())
		{
			echo "id:" . $row["game_id"] . " - Name: " . $row["gamename"] . " - Publisher: " . $row["publisher"] . "- Rating: " . $row["title"] . "<br />";
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
		<?php 
		if(mysqli_num_rows($rating_result) > 0)
		{
			while($row = $rating_result->fetch_assoc())
			{
				echo "<option value=" .$row["rating_id"] .">" . $row["title"] . "</option>"; 
			}
		}
		else 
		{
			echo "Empty Query. Please add ratings.";
		}

				?>
					</select><br /><br />
		<label>Game Genres:</label><br />
		<?php if(mysqli_num_rows($genre_roll) > 0)
		{
			$break_point = 0;
			while($row = $genre_roll->fetch_assoc())
			{   
				echo "<input type='checkbox' name='genres[]' id= ".$row["genre_id"] . " value=" .$row["genre_id"]." /> ". $row["name"];
				$break_point++;
				if(($break_point % 4) === 0)
				{
					echo "<br />";
				}
			}
		}
		else 
		{
			echo "Empty query. Add some platforms.";
		}

		echo "<br /><br/>";
		
		

		if(mysqli_num_rows($platform_roll) > 0)
		{
			$break_point = 0;
			while($row = $platform_roll->fetch_assoc())
			{   
				echo "<input type='checkbox' name='platforms[]' id='" . $row["platform_id"] ."' value=" .$row["platform_id"]." /> ". $row["name"];
				$break_point++;
				if(($break_point % 4) === 0)
				{
					echo "<br />";
				}
			}
		}
		else 
		{
			echo "Empty query. Add some genres.";
		}

		echo "<br /><br/>";

		?>

			<input type="submit" name="addgame" id="addgame" value="SUBMIT" />
		

		

		</fieldset>
	</form>

<br /><br />

	

	
	<h1>Reviews:</h1>
	<?php 
		echo "<form action='addreview.php' method='post'>";
		echo "<fieldset>";
		echo "<legend>Add Review</legend>";
		echo "<label>Game Name: </label>";
		echo "<select name='select_game' id='select_game'>";
		
		if(mysqli_num_rows($add_review) > 0)
		{
			while($row = mysqli_fetch_assoc($add_review_game))
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
		echo "<textarea rows='8' cols='50' name='text_review' id='text_review' required ></textarea>";
		//echo "<input type='hidden' name='game_id' id='game_id'>";
		echo "<input type='submit' name='addreview' id='addreview' value='SUBMIT' /> </fieldset></form>"; ?>
<br /><br /> 

<?php include 'footer.php'; ?>

</body>
</html>
