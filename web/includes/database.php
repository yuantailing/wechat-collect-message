<?php
function newDBConnection($database_info, $db_name = "production")
{
    $mysqli = new mysqli($database_info["host"], $database_info["user"],
        $database_info["password"], $database_info["database prefix"] . "_" . $db_name);
    $mysqli->set_charset("utf8");
    $mysqli->autocommit(false);
    return $mysqli;
}
?>
