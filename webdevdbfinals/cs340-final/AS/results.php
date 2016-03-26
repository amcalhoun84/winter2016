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
	$select = "SELECT game.title AS gTitle, rating.title AS rTitle, review.star_rating ";
	$join = "FROM game INNER JOIN review ON game.review_id=review.review_id
			INNER JOIN rating ON game.rating_id=rating.rating_id
			INNER JOIN game_genre ON game.game_id=game_genre.game_id
			INNER JOIN genre ON game_genre.genre_id=genre.genre_id
			INNER JOIN game_platform ON game.game_id=game_platform.game_id
			INNER JOIN platform ON game_platform.platform_id=platform.platform_id";
	$where = " WHERE ";
	
	$mutli_select = "SELECT g1.title AS gTitle, rating.title AS rTitle, review.star_rating";
	#for just genre, assuming the platforms are NULL;
	$multi_join1 = "FROM((game g INNER JOIN game_genre gg ON (gg.game_id = g.game_id)) INNER JOIN genre ge ON (ge.genre_id = gg.genre_id)) WHERE (ge.name = '" .$genre1. "')) AS g1
					INNER JOIN ((game g INNER JOIN game_genre gg ON (gg.game_id = g.game_id)) INNER JOIN genre ge ON (ge.genre_id = gg.genre_id)) WHERE (ge.name = '" .$genre2. "')) AS g2
					WHERE g1.title = g2.title;";



	#DEBUGGING AREA:
	echo $genre1 . "<br />";
	echo $genre2 . "<br />";
	echo $platform1 . "<br />";
	echo $platform2 . "<br />";

	#build search query
	#get game_title
	if($game_title !== ''){
		$where = $where . "game.title=\"" . $game_title . "\" ";
	}

	#get lowRev
	if($lowRev !== "NULL" and $where == " WHERE " ){
		$where = $where . "review.star_rating>=\"" . $lowRev . "\" ";
	}
	elseif($lowRev !== "NULL"){
		$where = $where . "AND review.star_rating>=\"" . $lowRev . "\" ";
	}
	
	#get highRev
	if($highRev !== "NULL" and $where == " WHERE "){
		$where = $where . "review.star_rating<=\"" . $highRev . "\" ";
	}
	elseif($highRev !== "NULL"){
		$where = $where . "AND review.star_rating<=\"" . $highRev . "\" ";
	}	
	
	#get genre(s)
	if($genre1 !== "NULL" and $genre2 == "NULL" and $where == " WHERE "){
		$where = $where . "genre.name=\"" . $genre1 . "\" ";
	}
	elseif($genre1 !== "NULL" and $genre2 == "NULL"){
		$where = $where . "AND genre.name=\"" . $genre1 . "\" ";
	}		

	#get rating(s)
	if($rating !== "NULL" and $where == " WHERE "){
		$where = $where . "rating.title=\"" . $rating . "\" ";
	}
	elseif($rating !== "NULL"){
		$where = $where . "AND rating.title=\"" . $rating . "\" ";
	}	

	#get platform
	if($platform1 !== "NULL" and $where == " WHERE "){
		$where = $where . "platform.name=\"" . $platform . "\" ";
	}
	elseif($platform1 !== "NULL"){
		$where = $where . "AND platform.name=\"" . $platform1 . "\" ";
	}
	
	#create full query string
	
	if($genre1 !== "NULL" AND $genre2 !== "NULL" AND $platform1 == "NULL" AND $platform2 == "NULL" AND $where == " WHERE "){
		$search = $select . $multi_join1 . " GROUP BY game.title ASC;";
	}


	else if($where == " WHERE "){
		$search = $select . $join . " GROUP BY game.title ASC;";
	}
	else{
		$search = $select . $join . $where . " GROUP BY game.title ASC;";
	}
	echo $search . "<br />";
	$results = $connection->query($search);
#	$results = $connection->query("SELECT game.title FROM game INNER JOIN rating ON game.rating_id=rating.rating_id;");	
	
	if(mysqli_num_rows($results) > 0)
	{
		while($row = $results->fetch_assoc())
		{
			# echo "title:" . $row["title"] /* . " - reviewID: " . $row["review_id"]*/ . "</br>";
			echo "title:" . $row["gTitle"] . " - rating: " . $row["rTitle"] . " - star_rating: " . $row["star_rating"] . "<br />";
		}
	}
	else
		{
			echo "Sorry, no results found. Please try again.";
		}	
?>