<?php 
	/*Templating -- */
	$page_title = "Games";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

?>

<?php 



$genre_sql = "SELECT * FROM genre";
$platform_sql = "SELECT * FROM platform";

$genre1_results = $connection->query($genre_sql);
$genre2_results = $connection->query($genre_sql);

$platform1_results = $connection->query($platform_sql);
$platform2_results = $connection->query($platform_sql);


/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
	
	$stmt = $connection->prepare("SELECT g.title, g.publisher, g.release_date, r.title FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) ORDER BY g.title ASC");
	
	//$result = $connection->query($stmt);
	//$num_rows = mysqli_num_rows($result);

?> 

			<form id="inputForm" action="results.php" method="get">
				<fieldset>
					<legend>Search Data</legend>
					<label>Game Title</label>
					<input type="text" name="game_title" id="game_title">
					</br></br>
					<label>Game Review</label>
					<select name="lowRev" id="lowRev">
<?php
					for($i = 0; $i <= 10.0; $i += 0.5)
					{
						echo '<option value="'.$i.'">' .$i . '</option>';
					}	
?>

					</select> - 
					<select name="highRev" id="highRev">
<?php
					for($i = 0; $i <= 10.0; $i += 0.5)
					{
						echo '<option value="'.$i.'">' .$i . '</option>';
					}	
?>

					</select>
					</br></br>
					<label>Game Genre 1</label>
					<select name="genre1" id="genre1">
						<option value="NULL">NULL</option>
						
					<?php

					if(mysqli_num_rows($genre1_results) > 0)
					{
						while($row = $genre1_results->fetch_assoc())
						{
							echo '<option value= "'.$row["genre_id"]. '"">' .$row["name"] .'</option>';

						}
					}
					

					?>
					</select><br /><br />
					<label>Game Genre 2</label>
					<select name="genre2" id="genre2">
					<option value="NULL">NULL</option>

					<?php

					if(mysqli_num_rows($genre2_results) > 0)
					{
						while($row = $genre2_results->fetch_assoc())
						{
							echo '<option value= "'.$row["genre_id"]. '"">' .$row["name"] .'</option>';

						}
					}
					

					?>
					</select>
					<br /><br>
					<label>Platform 1</label>
					<select name="platform1" id="platform1">
							<option value="NULL">NULL</option>
					<?php
					if(mysqli_num_rows($platform1_results) > 0)
					{
						while($row = $platform1_results->fetch_assoc())
						{
							echo '<option value= "'.$row["platform_id"]. '"">' .$row["name"] .'</option>';

						}
					}
					

					?>
					</select>
					<br/><br/>
					<label>Platform 2</label>
					<select name="platform2" id="platform2">
						<option value="NULL">NULL</option>
					<?php
					if(mysqli_num_rows($platform2_results) > 0)
					{
						while($row = $platform2_results->fetch_assoc())
						{
							echo '<option value= "'.$row["platform_id"]. '"">' .$row["name"] .'</option>';

						}
					}
					

					?>
					</select>


					</br></br>
					<label>Game Rating</label>
					<select name="rating" id="rating">
						<option value="NULL"></option>
						<option value="E">E</option>
						<option value="E10+">E10+</option>
						<option value="T">T</option>
						<option value="M">M</option>
						<option value="AO">AO</option>
					</select>
					</br></br>
		
					<input type="submit" name="dataSub" id="dataSub">
				</fieldset>
			</form>


<div>

	<table>
		<tr>
			<td>Games and Ratings</td>
		</tr>
		<tr>
			<td>Game Title</td>
			<td>Game Publisher</td>
			<td>Release Date</td>
			<td>Rating</td>

<?php 

$game_title = $_GET["game_title"];
$lowRev = $_GET["lowRev"];
$highRev = $_GET["highRev"];
$genre1 = $_GET["genre1"];
$genre1 = $_GET["genre2"];
$rating = $_GET["rating"];
$platform = $_GET["platform1"];
$platform = $_GET["platform2"];

/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
	
	$stmt = $connection->prepare("SELECT g.title, g.publisher, g.release_date, r.title FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) ORDER BY g.title ASC");
	//$result = $connection->query($stmt);
	//$num_rows = mysqli_num_rows($result);

	if(!$stmt->execute()) {
			echo "Execute failed: " . $connection->connect_errno . " " . $connection->connect_error;
		}
	if(!$stmt->bind_result($game_title, $publisher, $release_date, $rating)) {
		echo "Bind failed: " . $connection->connect_errno . " " . $connection->connect_error;
		}
	while($stmt->fetch()) {
		echo "<tr>\n<td>\n" . $game_title . "\n</td>\n<td>\n" . $publisher . "\n</td>\n<td>\n" . $release_date . "\n</td><td>\n" . $rating . "\n</td>\n</tr>";
	}

	$stmt->close(); 


?>
