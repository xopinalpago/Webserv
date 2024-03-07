<?php
    // echo 'ceci est un test';
    $url = 'http://localhost:8080/resource/123';

    // Initialiser une session cURL
    $ch = curl_init($url);
    
    // Configurer les options de la requête DELETE
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'DELETE');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    
    // Exécuter la requête
    $response = curl_exec($ch);
    echo $response;
    
    // Vérifier s'il y a des erreurs
    if(curl_errno($ch)){
        echo 'Erreur cURL : ' . curl_error($ch);
    }
    
    // Fermer la session cURL
    curl_close($ch);
    
    // Afficher la réponse du serveur
    echo $response;
?>