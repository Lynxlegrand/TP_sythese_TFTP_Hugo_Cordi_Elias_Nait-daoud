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

On voit bien que le PC et le serveur DNS communiquent.

## 3. Réservation d’un socket de connexion vers le serveur 

![image](https://github.com/user-attachments/assets/4259aacc-e458-4346-a628-c5d0fa41fcd7)

Maintenant qu'on a l'adresse du serveur, on peut créer un socket. Cependant, en regardant wireshark, on s'aperçoit que rien ne change. On essaie de lui envoyer une requète de ping mais il ne répond pas. On suppose donc qu'il est hors ligne. On choisit donc d'utiliser le serveur disponible sur moodle (local). 

![image](https://github.com/user-attachments/assets/1f98908d-7554-4e6a-9f48-d9597fe52ffa)

On le ping depuis une autre console pour voir si il répond. Malheureusement, puisqu'il est interne, on ne peut pas voir les communications client-serveur avec wireshark car ce logiciel permet de visualiser les communications sortantes et entrantes. 

![image](https://github.com/user-attachments/assets/e7b62118-35a4-45b9-973b-8e59b3426203)

Lorsque l'on ping sur un autre serveur, il ne répond pas on décide donc d'utiliser atftpd qui est un serveur local et on voit bien qu'il est actif. Maintenant il faut voir s'il répond à une requête.
