![Imgur](https://i.imgur.com/XHw8dh5.png)

# Jeu de moulin par Amine Yahemdi

Ce jeu a été développé dans le cadre d'un projet scolaire, il comporte trois modes :
1. Un mode Joueur contre Joueur
2. Un mode Joueur contre IA avec deux niveaux : facile ou moyen.
3. Un mode Joueur contre Joueur en LAN.

Le jeu est téléchargeable dans la partie release.

Le jeu est aussi compilable/jouable depuis Ubuntu.

# Fiche technique 

Nom du jeu : [Jeu de moulin](https://fr.wikipedia.org/wiki/Jeu_du_moulin)

Type : Stratégie/réflexion

Plateforme : Windows / Ubuntu

Version actuel : 0.2.1

Langage : C

Librairies utilisées : [SDL2](https://www.libsdl.org/download-2.0.php), [inih](https://github.com/benhoyt/inih)

![Imgur](https://i.imgur.com/od07WWi.png)

# Configuration du mode LAN

Le mode LAN est configurable dans le fichier config.ini qui se trouve dans le repertoire de l'executable moulin.exe , plus précisement dans la partie Network du fichier :

    [Network]
    ;Les informations reseau de l'adversaire
    Active=1 		; Si le mode en ligne est active
    IP=192.168.43.85
    Port=49152		; par défaut 49152
    ID=1

Pour jouer contre un joueur connecté depuis un autre PC, il faut :

1. Etre connecté sur un même réseau LAN
2. Posseder son IP local, cette derniére est acessible depuis la commande ipconfig
3. Le champs "ID=" doit comporter la valeur 0 ou 1 et doit être opposé à celui enregistré dans le fichier config.ini de l'adversaire. Ainsi, si l'adversaire choisit d'être le premier joueur (ID=0), il faut configurer (ID=1) dans le fichier config.ini.
## Utiliser Hamachi
Si vous n'êtes pas connecté à un réseau local, vous pouvez en créer un virtuellement en utilisant le logiciel [Hamachi](https://www.logmein.com/).

# Compiler sous Code::Blocks

Pour compiler le projet sous Code:Blocks il ne faut pas oublier de modifier les parametres du projet dans Project > Build Options > moulin > Search Directories et modifier les chemins indiqués dans Compiler et Linker pour indiquer l'emplacement de la SDL2 et ses extensions SDL_ttf, SDL_image et SDL_mixer. Il est préférable que les extensions soient installées dans le même dossier que la SDL2.

Vous pouvez aussi télécharger le fichier "" qui contient le code source et la SDL2+ses exentions pré-installés.

# Makefile

Le projet sera bientôt compilable avec Makefile.

# Ce qui est prévu dans la v2.0
- Une meilleure IA, notamment basé sur l'algorithme minimax
- Un mode en LAN dynamique (plus besoin de spécifier l'ID) et directement configurable dans le jeu.
- Les paramètres configurable actuellement dans le fichier config.ini le seront directement dans le jeu.
- Ajout d'un mode de jeu par internet. 
