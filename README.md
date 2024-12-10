## 1. Utilisation des arguments passés à la ligne de commande des programmes gettftp pour obtenir les informations de requêtes

![image](https://github.com/user-attachments/assets/e971f6de-e0d6-4b2d-b068-f25b3430223e)

La ligne de commande doit contenir exactement 4 arguments après le nom du programme.
Si les arguments sont incorrects, un message d’erreur est affiché et le programme s’arrête.

## 2. Appel à getaddrinfo pour obtenir l’adresse du serveur 

![image](https://github.com/user-attachments/assets/f4a803d6-e27b-483c-988e-b285a9c725d5)

On obtient l'adresse du serveur avec getaddrinfo. Cependant la console nous affiche 3 adresses, on doit donc filtrer afin que la fonction nous affiche qu'une adresse.

![image](https://github.com/user-attachments/assets/e0673813-1fe9-4f9f-848a-5de7429836ef)

On filtre pour avoir uniquement UDP : protocol = 17

![image](https://github.com/user-attachments/assets/f625e399-8087-494a-82e7-14ddd7774356)


