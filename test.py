import os

# header // a generer par le script cgi 
print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Content-Length: 306\n")

query = os.environ.get('QUERY_STRING')
# corps de la reponse
print("<!DOCTYPE html>")
print("<html lang=\"en\"><head>")
print("<link href=\"./style/style.css\" rel=\"stylesheet\"><link href=\"./style/form.css\" rel=\"stylesheet\">")
print("<title>Form</title></head>")
print("<body><h1 class=\"title\">Response</h1>")

print("<html><head><title>Ma page web</title></head>")
print("<body><h2>Hello, World!</h2>")
print("<p>", query, "</p>", sep='')
print("</body></html>")