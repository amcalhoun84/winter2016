<?php 
	/*Templating -- */
	$page_title = "Games";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

?>

<?php 
/*


$genre_sql = "SELECT * FROM genre";
$platform_sql = "SELECT * FROM platform";

$genre_results = $connection->query($genre_sql);

$platform_results = $connection->query($platform_sql);

$game_title = $_GET["game_title"];
$lowRev = $_GET["lowRev"];
$highRev = $_GET["highRev"];
$genre = $_GET["genre1"];
$rating = $_GET["rating"];
$platform = $_GET["platform1"];
*/

/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
	
//	$stmt = $connection->prepare("SELECT g.title, g.publisher, g.release_date, r.title FROM game g INNER JOIN rating r ON (r.rating_id = g.rating_id) ORDER BY g.title ASC");
	
	//$result = $connection->query($stmt);
	//$num_rows = mysqli_num_rows($result);
	$game_sql = "SELECT * from game";
	$review_sql = "SELECT * FROM review INNER JOIN game ON (game.review_id = review.review_id);";
	$genre_sql = "SELECT * FROM genre";
	$platform_sql = "SELECT * from platform";
	$genre_results = $connection->query($genre_sql);
	$genre_search = $connection->query($genre_sql);
	$get_genre = $connection->query($genre_sql);
	$platform_results = $connection->query($platform_sql);
	$get_platform = $connection->query($platform_sql);
	$platform_search = $connection->query($platform_sql);
	$delete_game = $connection->query($game_sql);
	$delete_review = $connection->query($review_sql);


?> 

			<form id="inputForm" action="results.php" method="get">
				<fieldset>
					<legend>Search Data</legend>
					<label>Game Title</label>
					<input type="text" name="game_title" id="game_title">
					</br></br>
					<label>Game Review</label>
					<select name="lowRev" id="lowRev">
					<option value="NULL">
<?php
					for($i = 0; $i <= 10.0; $i += 0.5)
					{
						echo '<option value="'.$i.'">' .$i . '</option>';
					}	
?>

					</select> - 
					<select name="highRev" id="highRev">
					<option value="NULL">
<?php
					for($i = 0; $i <= 10.0; $i += 0.5)
					{
						echo '<option value="'.$i.'">' .$i . '</option>';
					}	
?>

					</select>
					</br></br>
					<label>Game Genre</label>
					<select name="genre" id="genre">
						<option value="NULL"></option>
						
					<?php

					if(mysqli_num_rows($genre_search) > 0)
					{
						while($row = $genre_search->fetch_assoc())
						{
							echo '<option value= "'.$row["name"]. '"">' .$row["name"] .'</option>';

						}
					}
					

					?>
					</select>
					<br /><br>
					<label>Platform</label>
					<select name="platform" id="platform">
							<option value="NULL"></option>
					<?php
					if(mysqli_num_rows($platform_search) > 0)
					{
						while($row = $platform_search->fetch_assoc())
						{
							echo '<option value= "'.$row["name"]. '"">' .$row["name"] .'</option>';

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


	<?php 
	echo '<form action="getgenre.php" method="post"><fieldset>
			<legend>Game Genre Details</legend>';

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


	echo '<form action="platform.php" method="post"><fieldset>
			<legend>Platform Details</legend>';

			echo '<select name="get_platfom" id="get_platfom">';
							
			if(mysqli_num_rows($get_platform) > 0)
			{
					while($row = $get_platform->fetch_assoc())
					{
						
						echo '<option value="' . $row["platform_id"] . '">' . $row["name"] . '</option>';
					}
				}
				
			echo '</select>';
 			echo '<input type="submit" name="getPlt" id="getPlt" />';

 			echo '</fieldset>';


	echo '</form>';


	echo '<form action="deletegame.php" method="post"><fieldset>
			<legend>Remove Game</legend>';

			echo '<select name="delete_game" id="delete_game">';
							
			if(mysqli_num_rows($delete_game) > 0)
			{
					while($row = $delete_game->fetch_assoc())
					{
						
						echo '<option value="' . $row["game_id"] . '">' . $row["title"] . '</option>';
					}
				}
				
			echo '</select>';
 			echo '<input type="submit" name="delGame" id="delGame" />';

 			echo '</fieldset>';


	echo '</form>';

	echo '<form action="deletereview.php" method="post"><fieldset>
			<legend>Remove Review</legend>';

			echo '<select name="delete_rev" id="delete_rev">';
							
			if(mysqli_num_rows($delete_review) > 0)
			{
					while($row = $delete_review->fetch_assoc())
					{
						
						echo '<option value="' . $row["review_id"] . '">' . $row["title"] . '</option>';
					}
				}
				
			echo '</select>';
 			echo '<input type="submit" name="delRev" id="delRev" />';

 			echo '</fieldset>';


	echo '</form>';



/***************************
   GENERATE TABLE FOR GAMES
***************************/ 
/*	
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

*/
 include('footer.php'); ?>
