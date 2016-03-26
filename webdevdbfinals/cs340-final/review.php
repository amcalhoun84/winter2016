<?php 
	/*Templating -- */
	$page_title = 'Review Rating';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

	$review_sql = "SELECT * FROM review INNER JOIN game ON (game.review_id = review.review_id)";
	$review_result = $connection->query($review_sql);


	echo "<h2>Comprehensive Review List</h2>";

	if(mysqli_num_rows($review_result) > 0)
	{
		echo "<table>
				<thead>
				<tr>
				<th>Title</th>
				<th>Star Rating</th>
				<th>Text Review</th>";

		while($row = $review_result->fetch_assoc())
		{
			echo "<tr><td>" . $row["title"] . "</td><td>" . $row["star_rating"] . "</td><td>" . $row["text_review"] . "</td></tr>";
		}

		echo "</table>";
	}

	else
	{
		echo "We are sorry, but there doesn't appear to be any reviews!";
	}


	include('footer.php'); ?>


