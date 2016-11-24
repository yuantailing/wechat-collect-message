<?php
require_once("common.function.php");

function message_insert_text_message($conn, $to_user_name, $from_user_name, $create_time, $msg_type, $content, $pic_url, $media_id, $thumb_media_id,
	$format, $recognition, $location_x, $location_y, $scale, $label, $title, $description, $url, $msg_id) {
	$sql = "INSERT INTO message (to_user_name, from_user_name, create_time, msg_type, content, pic_url, media_id, thumb_media_id, ".
		"format, recognition, location_x, location_y, scale, label, title, description, url, msg_id) VALUES (?, ?, ?, ?";
	$bind_type = "ssis";
	$bind_param = array($to_user_name, $from_user_name, $create_time, $msg_type);
	if ($content) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $content;
	} else
		$sql .= ", NULL";
	
	if ($pic_url) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $pic_url;
	} else
		$sql .= ", NULL";
	
	if ($media_id) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $media_id;
	} else
		$sql .= ", NULL";
	
	if ($thumb_media_id) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $thumb_media_id;
	} else
		$sql .= ", NULL";
	
	if ($format) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $format;
	} else
		$sql .= ", NULL";
	
	if ($recognition) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $recognition;
	} else
		$sql .= ", NULL";
	
	if (is_numeric($location_x)) {
		$sql .= ", ?";
		$bind_type .= "d";
		$bind_param[] = $location_x;
	} else
		$sql .= ", NULL";
	
	if (is_numeric($location_y)) {
		$sql .= ", ?";
		$bind_type .= "d";
		$bind_param[] = $location_y;
	} else
		$sql .= ", NULL";
	
	if (is_numeric($scale)) {
		$sql .= ", ?";
		$bind_type .= "d";
		$bind_param[] = $scale;
	} else
		$sql .= ", NULL";
	
	if ($label) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $label;
	} else
		$sql .= ", NULL";
	
	if ($title) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $title;
	} else
		$sql .= ", NULL";
	
	if ($description) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $description;
	} else
		$sql .= ", NULL";
	
	if ($url) {
		$sql .= ", ?";
		$bind_type .= "s";
		$bind_param[] = $url;
	} else
		$sql .= ", NULL";
	
	$sql .= ", ?)";
	$bind_type .= "i";
	$bind_param[] = $msg_id;

	list($ret_code, ) = mysql_execute_prepare($conn,
		$sql, $bind_type, $bind_param, false);
    if ($ret_code < 0)
        return array("error" => -1025, "errorMessage" => "Impossible to execute query.");
	$message_id = $conn->insert_id;
	$conn->commit();
	return array("error" => 0, "message_id" => $message_id);
}

function message_insert_danmu($conn, $message_id, $text) {
	list($ret_code, ) = mysql_execute_prepare($conn,
		"INSERT INTO danmu (message_id, text, create_time) VALUES (?, ?, NOW())",
		"is", array($message_id, $text), false);
    if ($ret_code < 0)
        return array("error" => -1025, "errorMessage" => "Impossible to execute query.");
	$conn->commit();
	return array("error" => 0);
}

function message_get_danmu_last($conn, $n) {
	$max_count = 256;
	list($ret_code, $result) = mysql_execute_prepare($conn,
		"SELECT danmu.message_id, danmu.text, danmu.create_time FROM danmu WHERE 1=1 " .
		"ORDER BY danmu.message_id DESC LIMIT ?",
		"i", array(min(max(intval($n), 0), $max_count)), true);
    if ($ret_code < 0)
        return array("error" => -1025, "errorMessage" => "Impossible to execute query.");
	$danmu_set = array();
	while ($row = $result->fetch_array()) {
		$danmu_set[] = array("message_id" => $row["message_id"],
							"text" => $row["text"],
							"create_time" => $row["create_time"]);
	}
	if (count($danmu_set) >= $max_count)
		return array("error" => 0, "hint" => "count(danmu_set) reach the limit ($max_count), maybe not all danmus are in list.", "danmu_set" => $danmu_set);
	return array("error" => 0, "danmu_set" => $danmu_set);
}

function message_get_danmu_recent($conn, $seconds) {
	$max_count = 256;
	list($ret_code, $result) = mysql_execute_prepare($conn,
		"SELECT danmu.message_id, danmu.text, danmu.create_time FROM danmu WHERE NOW() - danmu.create_time >= 0 AND TIME_TO_SEC(TIMEDIFF(NOW(), danmu.create_time)) < ? " .
		"ORDER BY danmu.message_id DESC LIMIT 256",
		"i", array($seconds), true);
    if ($ret_code < 0)
        return array("error" => -1025, "errorMessage" => "Impossible to execute query.");
	$danmu_set = array();
	while ($row = $result->fetch_array()) {
		$danmu_set[] = array("message_id" => $row["message_id"],
							"text" => $row["text"],
							"create_time" => $row["create_time"]);
	}
	if (count($danmu_set) >= $max_count)
		return array("error" => 0, "hint" => "count(danmu_set) reach the limit ($max_count), maybe not all danmus are in list.", "danmu_set" => $danmu_set);
	return array("error" => 0, "danmu_set" => $danmu_set);
}

function message_get_danmu_message_id__gt($conn, $message_id) {
	$max_count = 256;
	list($ret_code, $result) = mysql_execute_prepare($conn,
		"SELECT danmu.message_id, danmu.text, danmu.create_time FROM danmu WHERE danmu.message_id > ? " .
		"ORDER BY danmu.message_id DESC LIMIT 256",
		"i", array($message_id), true);
    if ($ret_code < 0)
        return array("error" => -1025, "errorMessage" => "Impossible to execute query.");
	$danmu_set = array();
	while ($row = $result->fetch_array()) {
		$danmu_set[] = array("message_id" => $row["message_id"],
							"text" => $row["text"],
							"create_time" => $row["create_time"]);
	}
	if (count($danmu_set) >= $max_count)
		return array("error" => 0, "hint" => "count(danmu_set) reach the limit ($max_count), maybe not all danmus are in list.", "danmu_set" => $danmu_set);
	return array("error" => 0, "danmu_set" => $danmu_set);
}
?>
