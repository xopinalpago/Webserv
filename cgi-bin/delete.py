import requests

def send_delete_request(url):
    try:
        response = requests.delete(url)
        print("ENVOI de la requete")
    #     response.raise_for_status()  # Vérifie si la réponse est un succès
        print("La requête DELETE a été envoyée avec succès.")
    except requests.exceptions.RequestException as e:
        print("Erreur lors de l'envoi de la requête DELETE:", e)

# URL de la ressource que vous souhaitez supprimer
url = 'http://localhost:8080/resource/123'

# Envoi de la requête DELETE
send_delete_request(url)