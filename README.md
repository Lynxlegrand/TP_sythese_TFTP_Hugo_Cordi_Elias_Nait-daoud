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

Maintenant qu'on a l'adresse du serveur, on peut créer un socket. Cependant, en regardant wireshark, on s'aperçoit que rien ne change. On essaie de lui envoyer une requète de ping mais il ne répond pas. On suppose donc qu'il est hors ligne. On a tout d'abord essayé de créer un serveur avec atftp mais on a pas réussi donc on a choisi d'utiliser le serveur disponible sur moodle (local). Quand on ping ce serveur, il répond. On est donc sur que le serveur est en linge. 

## 4. Pour gettftp: a) Construction d’une requête en lecture (RRQ) correctement formée, et envoi au serveur.

![image](https://github.com/user-attachments/assets/1f98908d-7554-4e6a-9f48-d9597fe52ffa)

On le ping depuis une autre console pour voir si il répond. Maintenant qu'on sait qu'il répond, on peut créer la tramme et la stocker dans un buffer avec les bons paramètres et l'envoyer. 

![image](https://github.com/user-attachments/assets/6ccc6d14-806e-46f0-a11c-3a88254b3573)

D'après Wireshark, la tramme est bien envoyée et le serveur répond. 

## b) ) Réception d’un fichier constitué d’un seul paquet de données (DAT) et son acquittement (ACK) 
Maintenant qu'on a créé le lien entre le serveur et le client, on peut envoyer des paquets. Pour commencer, j'ai créer dans le serveur un ficher test.txt de 20 octets < 512 octets. Il y a aussi dans le serveur une image qui s'appelle ensea.png qui fait plus de 512 octets, donc pour pouvoir l'envoyer en entier, il faudra envoyer plusieurs paquets de 521 octets. (c'est l'objet de la question 4.c)

![image](https://github.com/user-attachments/assets/e258444f-f0e6-4986-b066-6c52cdb35270)

On voit que le fichier test.txt est bien transmis en entier mais que pour le fichier ensea.png, seul le premier paquet est transmis. 

## c) Réception d’un fichier constitué de plusieurs paquets de données (DAT) et leurs acquittements respectifs (ACK) 

On ajoute une boucle while dans le code : tant que tous les paquets n'ont pas étés envoyés, on reste dans la boucle et on envoie des paquets. 

![image](https://github.com/user-attachments/assets/bfc0ac8f-82fb-4b09-9c85-a7dc0bc70098)

On voit que l'intégralité du fichier ensea.png a été envoyé. 
