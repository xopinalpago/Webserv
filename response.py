import os

query = os.environ.get('QUERY_STRING')

content = "<!DOCTYPE html> \
<html lang=\"en\"> \
    <head> \
        <link href=\"./style/style.css\" rel=\"stylesheet\"> \
        <link href=\"./style/form.css\" rel=\"stylesheet\"> \
        <title>Response</title> \
    </head> \
    <body> \
        <h1 class=\"title\">Response</h1> \
        <h2>Hello, World!</h2> \
        <p>" + query + "</p> \
        <a href=\"site_index.html\"><button>INDEX</button></a> \
    </body> \
</html>"

print(content)