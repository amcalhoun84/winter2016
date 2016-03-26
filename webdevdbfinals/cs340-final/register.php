<?php 
	$page_title = "Register";
	include 'header.php';
	

	if(isset($_POST['action'] && $_POST['action'] == 'register')
	{
		$first_name = mysqli_escape_string($connection, $_POST['register_firstName']);
		$last_Name = mysqli_real_esacpe_string($connection, $_POST['register_lastName'];

		$username = mysqli_real_esacpe_string($connection, $_POST['register_username']);
		$encodeHash = md5(mysqli_real_esacpe_string($connection, $_POST['register_password']);
		
	}

?>
	Honestly, I doubt we are going to use this, but I figured we'd have it.
	<form action="" autocomplete="off" method="POST">
			<h1>User Account Log-In</h1>
				<label for="userName">User Name</label>
				<input type="text" name="userName" id="userName" />
				<br /><br />

				<label for="password">Password</label>
				<input type="text" name="password" id="password" />
				<br /><br />

				<label for="password">Re-Enter Password:</label>
				<input type="text" name="password" id="password" />
			</form>

