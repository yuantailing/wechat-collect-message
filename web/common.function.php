<?php
function check_input_param($keys, $arr = null)
{
	if (is_null($arr))
		$arr = $_POST;
	if (!is_array($keys))
		$local_keys = array($keys);
	else
		$local_keys = $keys;
	foreach ($local_keys as $key) {
		if (!array_key_exists($key, $arr))
			return array("error" => -1, "errorMessage" => "paramter missed: " . $key);
	}
	return true;
}

function param_exist_or_null($key)
{
	if (array_key_exists($key, $_POST))
		return $_POST[$key];
	else
		return null;
}

function mysql_execute_prepare($conn, $prepareSqlStmt, $types, $varArray, $hasResult = false)
{
	if (!$stmt = $conn->prepare($prepareSqlStmt))
		return array(-1, $conn->errno);
	$bindParam = array($types);
    if (strlen($types) > 0) {
        for ($i = 0; $i < count($varArray); $i++)
        {
            $bindParam[] = &$varArray[$i];
        }
        if(!call_user_func_array(array($stmt, "bind_param"), $bindParam))
            return array(-2, $stmt->errno);
    }
    if (!$stmt->execute())
        return array(-3, $stmt->errno);
    if ($hasResult)
        if (!$result = $stmt->get_result())
			return array(-4, $stmt->errno);
		else
			return array(0, $result);
	else
		return array(0, null);
}

function is_after_deadline($conn, $assignment_id) {
	list($ret_code, $result) = mysql_execute_prepare($conn, "select end_date from assignment where id = ?", "i", array($assignment_id), true);

	$row = $result->fetch_array();
	$deadline = new DateTime($row["end_date"]);
	$result->free();

	return new DateTime() >= $deadline;
}

?>
