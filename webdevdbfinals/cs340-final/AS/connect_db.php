<?php
    ob_start();
   // session_start();
	error_reporting(E_ALL);
	ini_set('display_errors', 'On');

// Move this to a passkey file.
	$dbhost = 'fpACAS.db.4970435.hostedresource.com';
	$dbname = 'fpACAS';
	$dbuser = 'fpACAS';
	$dbpass = 'RoughPr0ject!';	


	$connection = new mysqli($dbhost, $dbuser, $dbpass, $dbname);

	if($connection->connect_errno){
		die("Failed to connect to mySQL: (" . $connection->connect_errno . ")" . $connection->connect_error);
	}
	else
	{
		echo "Everything looks fine. Server <span style='color:green'><b>connected</b></span>.";
	}


// Date validation check -- credit due to https://gist.github.com/voku/dd277e9c660f38b8c3a3 for dateCheck validation

function checkDateFormats($date)
{
    if(preg_match("/^([0-9]{4})-([0-9]{2})-([0-9]{2})$/", $date, $parts))
    {
        if(checkdate($parts[2],$parts[3],$parts[1]))
            return true;
        else
            return false;
    }
    else 
        return false;
}


?>