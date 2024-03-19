function cleanForm() {
    var formulaire = document.getElementById('form');
    for (var i = 0; i < formulaire.elements.length; i++) {
        var element = formulaire.elements[i];
        if (element.type != 'submit')
            element.value = '';
    }
}