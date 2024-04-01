function fillResourceList() {
    var selectElement = document.getElementById("resourceList");

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/listFiles/upload", true);
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            try {
                var files = JSON.parse(xhr.responseText);
                files.forEach(file => {
                    var option = document.createElement("option");
                    option.text = file;
                    selectElement.appendChild(option);
                });
            } catch {
                var option = document.createElement("option");
                option.text = "No files available";
                selectElement.appendChild(option);
                document.open();
                document.write(xhr.responseText);
                document.close();
            }
        }
    };
    xhr.send();
}

window.onload = function() {
    fillResourceList();
};
