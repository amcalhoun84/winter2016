<?php 
	/*Templating -- */
	$page_title = "results";
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

?>

<?php
	#get information for the search
	parse_str($_SERVER['QUERY_STRING']);
	$select = "SELECT game.title AS gTitle, game.publisher, game.release_date, rating.title AS rTitle, review.star_rating ";
	$join = "FROM game INNER JOIN review ON game.review_id=review.review_id
			INNER JOIN rating ON game.rating_id=rating.rating_id
			INNER JOIN game_genre ON game.game_id=game_genre.game_id
			INNER JOIN genre ON game_genre.genre_id=genre.genre_id
			INNER JOIN game_platform ON game.game_id=game_platform.game_id
			INNER JOIN platform ON game_platform.platform_id=platform.platform_id";
	$where = " WHERE ";
	
	#build search query
	#get game_title
	if($game_title !== ''){
		$where = $where . "game.title=\"" . $game_title . "\" ";
	}

	if($lowRev > $highRev)
	{
		$swap = $highRev;
		$highRev = $lowRev;
		$lowRev = $swap;
	}

	#get lowRev
	if($lowRev !== "NULL" and $where == " WHERE " ){
		$where = $where . "review.star_rating>=\"" . $lowRev . "\" ";
	}
	elseif($lowRev !== "NULL"){
		$where = $where . "AND review.star_rating>=\"" . $lowRev . "\" ";
	}
	elseif($lowRev === ""){
		$where = $where . "AND review.star_rating>=\"" . $lowRev . "\" ";
	}
	elseif($lowRev === "" and $where == " WHERE " ){
		$where = $where . "review.star_rating>=\"" . $lowRev . "\" ";
	}

	
	#get highRev
	if($highRev !== "NULL" and $where == " WHERE "){
		$where = $where . "review.star_rating<=\"" . $highRev . "\" ";
	}
	elseif($highRev !== "NULL"){
		$where = $where . "AND review.star_rating<=\"" . $highRev . "\" ";
	}
	elseif($highRev === ""){
		$where = $where . "AND review.star_rating>=\"" . $highRev . "\" ";
	}
	elseif($highRev === "" and $where == " WHERE " ){
		$where = $where . "review.star_rating>=\"" . $highRev . "\" ";
	}
	
	echo $genre;

	#get genre(s)
	if($genre !== "NULL" and $where == " WHERE "){
		$where = $where . "genre.name=\"" . $genre . "\" ";
	}
	elseif($genre !== "NULL"){
		$where = $where . "AND genre.name =\"" . $genre . "\" ";
	}		

	#get rating(s)
	if($rating !== "NULL" and $where == " WHERE "){
		$where = $where . "rating.title=\"" . $rating . "\" ";
	}
	elseif($rating !== "NULL"){
		$where = $where . "AND rating.title=\"" . $rating . "\" ";
	}	

	#get platform
	if($platform !== "NULL" and $where == " WHERE "){
		$where = $where . "platform.name=\"" . $platform . "\" ";
	}
	elseif($platform !== "NULL"){
		$where = $where . "AND platform.name=\"" . $platform . "\" ";
	}

	#create full query string
	if($where == " WHERE "){
		$search = $select . $join . " GROUP BY game.title ASC;";
	}
	else{
		$search = $select . $join . $where . " GROUP BY game.title ASC;";
	}
	
	$results = $connection->query($search);

	if(mysqli_num_rows($results) > 0)
	{
		echo '<table>
			<thead>
				<tr>
				<th>Game Title:</th><th>Publisher</th><th>Release Date</th><th>Rating</th><th>Review Score</th></tr>
				</thead>';

		while($row = $results->fetch_assoc())
		{
			echo "<tr><td>" . $row["gTitle"] . "</td><td>" .$row["publisher"] . "</td><td>" . $row["release_date"] ."</td><td>" . $row["rTitle"] . "</td><td>" . $row["star_rating"] . "</td></tr>";
		}

		echo '</table>';

	}
	else
		{
			echo "Sorry, no results found. Please try again.";
		}	
?>