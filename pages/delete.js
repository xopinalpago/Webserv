document.getElementById('deleteButton').addEventListener('click', function() {
    var selectElement = document.getElementById("resourceList");
    var selectedResource = selectElement.value;
    var source = selectElement.getAttribute("data-source");
    
    if (!selectedResource) {
        alert("Please select a resource before deleting it.");
        return;
    } else if (selectedResource === "No files available") {
        alert("No files are available.");
        return;
    }
    var confirmation = confirm("Are you sure you want to delete the resource?");
    if (confirmation) {
        console.log("The deletion of the resource is confirmed.");
        
        var xhr = new XMLHttpRequest();
        xhr.open('DELETE', 'listFiles/upload/' + selectedResource, true);
        
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                document.open();
                document.write(xhr.responseText);
                document.close();
                if (xhr.status != 200) {
                    alert("An error occurred while deleting the resource.");
                }
            }
        };
        xhr.send();
    } else {
        console.log("The deletion of the resource is canceled.");
    }
});