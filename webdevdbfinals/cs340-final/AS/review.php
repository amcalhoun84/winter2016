<?php 
	/*Templating -- */
	$page_title = 'Review Rating';
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');
	include 'header.php';

	$game_sql = "SELECT * FROM game";
	$genre_sql = "SELECT * FROM genre";
	$platform_sql = "SELECT * FROM platform";
	$review_sql = "SELECT * FROM review";
	$rating_sql = "SELECT * FROM rating";

	$game_result = mysqli_query($connection, $game_sql);
	$genre_result = mysqli_query($connection, $genre_sql);
	$platform_result = mysqli_query($connection, $platform_sql);
	$rating_result = mysqli_query($connection, $rating_sql);
	$review_result = mysqli_query($connection, $review_sql); ?>


