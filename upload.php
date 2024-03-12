#!/usr/bin/php-cgi
<?php

$method = $_SERVER["REQUEST_METHOD"];
$userfile = $_POST['userfile'];
echo "userfile :: $userfile";

if (empty($_POST))
{
    // echo "lkaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	exit(13);
}


$uploaddir = './uploads/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);

echo '<pre>';
if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile)) {
    echo "Le fichier est valide, et a été téléchargé
           avec succès. Voici plus d'informations :\n";
} else {
    echo "Attaque potentielle par téléchargement de fichiers.
          Voici plus d'informations :\n";
}

echo 'Voici quelques informations de débogage :';
print_r($_FILES);

echo '</pre>';

?>