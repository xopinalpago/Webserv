document.getElementById('deleteButton').addEventListener('click', function() {
    var selectElement = document.getElementById("resourceList");
    var selectedResource = selectElement.value;
    var source = selectElement.getAttribute("data-source");
    
    if (!selectedResource) {
        alert("Veuillez sélectionner une ressource avant de la supprimer.");
        return;
    }
    var confirmation = confirm("Êtes-vous sûr de vouloir supprimer la ressource ?");
    if (confirmation) {
        console.log("La suppression de la ressource est confirmée.");
        
        var xhr = new XMLHttpRequest();
        // Utilisez l'URL appropriée pour supprimer la ressource avec son nom
        xhr.open('DELETE', 'listFiles/upload/' + selectedResource, true);
        
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