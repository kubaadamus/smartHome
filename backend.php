<?php
header("Access-Control-Allow-Origin:*");
$user = 'jakubadamus';
$DBpassword = 'Kubaadamus1991';
$db = 'jakubadamus';
$host = 'mysql.cba.pl';
$port = 3360;
$database = mysqli_connect($host, $user, $DBpassword, $db) or die('Niedaradyyy' . mysqli_connect_error());
if ($database) {
    mysqli_set_charset($database, 'utf8');
} else {
}

$action = $_POST['action'];
$object = $_POST['object'];

if ($object) {
    $object = json_decode(urldecode($object));
}
if (!$action) {
    $action = $object->action;
}




$resultObject = new stdClass();


// Pobierz dane z czujników
if ($action == 'getSensorData') {
    $sql = "SELECT * FROM smartHomeSensors";
    $sensorsData = mysqli_fetch_all(mysqli_query($database, $sql), MYSQLI_ASSOC);


    $resultObject->msg = "pobrano dane sensorow";
    $resultObject->sensorData = $sensorsData;
    echo (json_encode($resultObject));
    exit();
}

if ($action == 'updateInput') {

    $inputs = $object->inputs;

    foreach ($inputs as $input) {

        $sql = "UPDATE smartHomeInput SET
        inputValue = '$input->inputValue'
        WHERE id = '$input->id'";
        mysqli_query($database, $sql);
    }

    $resultObject->msg = "Updejtowano inputy";
    $resultObject->sql = $sql;
    echo (json_encode($resultObject));
    exit();
}

if ($action == 'getInput') {


    $sql = "SELECT * FROM smartHomeInput";
    $inputs = mysqli_fetch_all(mysqli_query($database, $sql), MYSQLI_ASSOC);
    $resultObject->msg = "Updejtowano inputy";
    $resultObject->inputs = $inputs;
    echo (json_encode($resultObject));
    exit();
}
if ($action == 'updateSensorData') {

    $sensors = $object->sensors;
    $successArray = [];
    foreach ($sensors as $sensor) {
        $sql = "UPDATE smartHomeSensors SET
    sensorValue = '$sensor->value'
    WHERE id = '$sensor->id'";
        if (mysqli_query($database, $sql)) {
            $updateLog = new stdClass();
            $updateLog->msg = "zmieniono: $sensor->id na: $sensor->value";
            array_push($successArray, json_encode($updateLog));
        } else {
            array_push($successArray, 'fail');
        }
    }

    // Odeślij inputy
    $sql = "SELECT * FROM smartHomeInput";
    $input = mysqli_fetch_all(mysqli_query($database, $sql), MYSQLI_ASSOC);

    $resultObject->success = true;
    $resultObject->successArray = $successArray;
    $resultObject->input = json_encode($input);
    $resultObject->msg = "updejtowano sensory";
    echo (json_encode($resultObject));
    exit();
}
