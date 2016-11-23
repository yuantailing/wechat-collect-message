<?php
require_once("settings.php");
require_once("common.function.php");
require_once("includes/database.php");
require_once("message.function.php");

session_start();
header('Content-type: text/json; charset=UTF-8');

if (($ret = check_input_param("action", $_GET)) !== true) {
	print json_encode($ret);
	return;
}
$action = $_GET["action"];

$conn = newDBConnection($database_info);
if ($conn == null) {
	print json_encode(array("error" => -1024, "errorMessage" => "Impossible to open database."));
	return;
}

switch ($action)
{
case "get_danmu_last":
	if (($ret = check_input_param("n", $_GET)) === true)
		$ret = message_get_danmu_last($conn, $_GET["n"]);
	break;
case "get_danmu_recent":
	if (($ret = check_input_param("seconds", $_GET)) === true)
		$ret = message_get_danmu_recent($conn, $_GET["seconds"]);
	break;
case "get_danmu_message_id__gt":
	if (($ret = check_input_param("message_id", $_GET)) === true)
		$ret = message_get_danmu_message_id__gt($conn, $_GET["message_id"]);
	break;
default:
	$ret = array("error" => -2, "errorMessage" => "Invalid action");
	break;
}

$conn->close();

print json_encode($ret);

?>
