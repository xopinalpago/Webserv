// Fonction pour remplir l'élément <select> avec les fichiers du répertoire
function fillResourceList() {
    var selectElement = document.getElementById("resourceList");

    // Faites une requête AJAX pour obtenir la liste des fichiers du répertoire
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/listFiles/upload", true); // Assurez-vous que votre serveur a une route appropriée pour cela
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            var files = JSON.parse(xhr.responseText);

            // Ajoutez chaque fichier comme une option dans l'élément <select>
            files.forEach(file => {
                var option = document.createElement("option");
                option.text = file;
                selectElement.appendChild(option);
            });
        }
    };
    xhr.send();
}

// Appelez la fonction pour remplir l'élément <select> lorsque la page est chargée
window.onload = function() {
    fillResourceList();
};
