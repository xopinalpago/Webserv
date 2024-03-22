<?php
// Fonction de gestion du signal
function signalHandler($signal) {
    switch($signal) {
        case SIGINT:
            echo "Signal SIGINT reçu.\n";
            // Opérations à effectuer en cas de réception de SIGINT
            break;
        case SIGTERM:
            echo "Signal SIGTERM reçu.\n";
            // Opérations à effectuer en cas de réception de SIGTERM
            break;
        case SIGQUIT:
            echo "Signal SIGQUIT reçu.\n";
            // Opérations à effectuer en cas de réception de SIGQUIT
            break;
        // Ajoutez d'autres cas pour d'autres signaux si nécessaire
    }
}

pcntl_signal(SIGINT, "signalHandler");
pcntl_signal(SIGTERM, "signalHandler");
pcntl_signal(SIGQUIT, "signalHandler");

while (true) {
    echo "Boucle infinie en cours...\n";
    sleep(1);
}
?>
