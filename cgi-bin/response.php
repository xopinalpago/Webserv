<?php
    $query = $_SERVER["QUERY_STRING"];
    echo '<!DOCTYPE html>
    <html lang="en">
        <head>
            <title>Response</title>
        </head>
        <body>
            <h1 class="title">Response</h1>
            <h2>Hello, World!</h2>
            <p>' . $query . '</p>
            <a href="/site_index.html"><button>Index</button></a>
        </body>
    </html>';
?>