// document.getElementById('deleteButton').addEventListener('click', function() {
    
//     var source = event.target.getAttribute("data-source");
//     var confirmation = confirm("Êtes-vous sûr de vouloir supprimer la ressource ?");
//     // if (!selectedResource) {
//     //     alert("Veuillez sélectionner une ressource avant de la supprimer.");
//     // }
//     if (confirmation) {
//         console.log("La suppression de la ressource est confirmée.");
//         var xhr = new XMLHttpRequest();
//         xhr.open('DELETE', '/ressource/123', true);
//         xhr.onreadystatechange = function() {
//             if (xhr.readyState === XMLHttpRequest.DONE) {
//                 if (xhr.status === 200) {
//                     document.open();
//                     document.write(xhr.responseText);
//                     document.close();
//                 } else {
//                     alert('Une erreur s\'est produite lors de la suppression de la ressource.');
//                 }
//             }
//         };
//         xhr.send();
//     } else {
//         console.log("La suppression de la ressource est annulee.");
//     }
// });

// Supposons que vous ayez un tableau de données contenant des ressources
// faire la liste des fichiers dans upload
var data = ["1", "2", "3"];

// Sélection de l'élément <select> par son ID
var selectElement = document.getElementById("resourceList");

// Parcours du tableau de données
data.forEach(function(resource) {
    // Création d'un nouvel élément <option>
    var option = document.createElement("option");
    
    // Définition de la valeur de l'option avec le nom de la ressource
    option.text = resource;
    
    // Ajout de l'option à l'élément <select>
    selectElement.appendChild(option);
});

document.getElementById('deleteButton').addEventListener('click', function() {
    var selectElement = document.getElementById("resourceList");
    var selectedResource = selectElement.value;
    var source = selectElement.getAttribute("data-source");
    
    // if (!selectedResource) {
    //     alert("Veuillez sélectionner une ressource avant de la supprimer.");
    // }
    var confirmation = confirm("Êtes-vous sûr de vouloir supprimer la ressource ?");
    if (confirmation) {
        console.log("La suppression de la ressource est confirmée.");
        
        var xhr = new XMLHttpRequest();
        // Utilisez l'URL appropriée pour supprimer la ressource avec son nom
        xhr.open('DELETE', '/upload/' + selectedResource, true);
        
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    document.open();
                    document.write(xhr.responseText);
                    document.close();
                } else {
                    alert('Une erreur s\'est produite lors de la suppression de la ressource.');
                }
            }
        };
        xhr.send();
    } else {
        console.log("La suppression de la ressource est annulée.");
    }
});