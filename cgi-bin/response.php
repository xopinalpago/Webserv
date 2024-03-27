<?php
    $query = $_SERVER["QUERY_STRING"];
    $login = $_GET["login"];
    echo "La valeur du champ login est : " . $login;
    echo '<!DOCTYPE html>
    <html lang="en">
        <head>
            <link href="/style/style.css" rel="stylesheet">
            <link href="/style/response.css" rel="stylesheet">
            <title>Response</title>
        </head>
        <body>
            <h1 class="pagesTitle">Response</h1>
            <p> test </p>
            <p>' . $query . '</p>
            <div class="index">
                <a class="indexButton" href="/site_index.html">Index</a>
            </div>
        </body>
    </html>';
?>