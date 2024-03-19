document.getElementById('deleteButton').addEventListener('click', function() {
    var confirmation = confirm("Êtes-vous sûr de vouloir supprimer la ressource ?");
    if (confirmation) {
        console.log("La suppression de la ressource est confirmée.");
        var xhr = new XMLHttpRequest();
        xhr.open('DELETE', '/ressource/123', true);
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
        console.log("La suppression de la ressource est annulee.");
    }
});
