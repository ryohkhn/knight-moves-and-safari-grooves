# Projet de c++  - PICKERN-RODRIGUEZ

## Projet

Ce projet implémente les trois jeux de plateau suivants:

- Butin (http://jeuxstrategieter.free.fr/Butin_complet.php)
- Gounki (https://www.boardspace.net/gounki/english/Gounki-en-v1.0.pdf)
- Safari (https://escaleajeux.fr/jeu/prene.0.0)

L'affichage graphique du projet est géré par SFML.

### Compilation

Afin de compiler le programme, éxécuter la commande suivante:

```
make
```

Pour supprimer les fichiers binaires:

```
make clean
```

### Exécution

```
./knightmovesandsafarigrooves
```

### Utilisation

L'interface graphique vous proposera de sélectionner un jeu.  
Les noms des joueurs seront demandés, appuyer sur `add` pour les ajouter et sur `start` pour démarrer la partie.  
Pendant la partie, le nom du joueur dont le tour est en cours est colorié en rouge.  
Sélectionner une pièce sur la plateau affichera en surbrillance les cases dont les déplacements sont valables.

En appuyant sur `échap` il est possible de mettre en pause le jeu, revenir au menu principal et quitter le programme.  
À la fin d'une partie, revenir au menu principal vous permettra de relancer une partie sur un mode de jeu.

#### Butin

La phase d'initialisation consiste en la séléction d'une pièce jaune par personne à supprimer du plateau.  

Après avoir effectué le premier saut, si un enchaînement est possible la ou les cases destination seront elles aussi en surbrillance.  
La partie se terminera automatiquement lorsqu'il ne restera plus aucun coup possible selon les règles du jeu.  


#### Gounki

Chaque tour, le joueur courant a la possibilité d'effectuer un déplacement ou un déploiement.  
Un clique gauche permet de déplacer une pièce/pile de pièces, un clique droit d'effectuer une étape du déploiement.  
Le déplacement s'effectue en un seul coup et permet au joueur d'aller le plus loin possible en fonction de son empilement de pièces.  
Le déploiement s'effectue étape par étape. Si jamais le joueur se retrouve dans une impasse ou il n'a pas possibilité de terminer son déploiement, le jeu interrompra le coup prématurement.  
La partie se terminera automatiquement lorsque toutes les pièces ennemies seront prises, ou qu'une des pièces atteindra les cases gagnantes.

#### Safari

La phase d'initialisation permet à chaque joueur de placer 3 animaux et 3 barrières sur l'ensemble du plateau.  
Il est possible de déplacer les animaux ou barrières en recliquant dessus tant que le bouton `end turn` n'est pas appuyé.  
Un clique gauche permet de placer un animal, un clique droit de placer une barrière.

Chaque tour, le joueur courant doit déplacer un animal et placer une barrière.  
Similairement à la phase d'initialisation, il est possible de placer et retirer une barrière tant que le bouton `end turn` n'est pas appuyé.  
Le score affiché à côté des noms indique le nombre de pièces encore en vie pour chaque joueur.  
La partie se termine automatiquement lorsque toutes les pièces d'un joueur sont enfermées.

### Dépendances nécessaires

`g++`, `sfml`
