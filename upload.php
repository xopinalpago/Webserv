

<?php
    $uploaddir = __DIR__ . '/uploads/';
    $uploadfile = $uploaddir . basename($_FILES['fileToUpload']['name']);
    $tmp_name = $_FILES['fileToUpload']['tmp_name'];
    if (!move_uploaded_file($tmp_name, $uploadfile)) {
    	exit(13);
    }
?>