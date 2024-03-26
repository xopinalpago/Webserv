document.getElementById('deleteButton').addEventListener('click', function() {
    var selectElement = document.getElementById("resourceList");
    var selectedResource = selectElement.value;
    var source = selectElement.getAttribute("data-source");
    
    if (!selectedResource) {
        alert("Veuillez sélectionner une ressource avant de la supprimer.");
        return;
    } else if (selectedResource === "No files available") {
        alert("No files are available.");
        return;
    }
    var confirmation = confirm("Êtes-vous sûr de vouloir supprimer la ressource ?");
    if (confirmation) {
        console.log("La suppression de la ressource est confirmée.");
        
        var xhr = new XMLHttpRequest();
        xhr.open('DELETE', 'listFiles/upload/' + selectedResource, true);
        
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                document.open();
                document.write(xhr.responseText);
                document.close();
                if (xhr.status != 200) {
                    alert('Une erreur s\'est produite lors de la suppression de la ressource.');
                }
            }
        };
        xhr.send();
    } else {
        console.log("La suppression de la ressource est annulée.");
    }
});