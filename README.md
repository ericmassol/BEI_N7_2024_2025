# BEI N7 - Déventement du parachute d'un aérostat du CNES

_Projet courant du 14/10/2024 au 31/01/25_

## Présentation des participants impliqués sur le projet

_Référents_ : 
- Frédéri Mirc (côté CNES)
- Mme Ngueuveu (côté ENSEEIHT)

_Etudiants_ : 
- Alexandre PAUL (référent communication N7/CNES)
- Philippe CASTERES (référent technique)
- Eric MASSOL (Partie logicielle)
- Loïc BARTHE

## Présentation du projet

Dans le cadre des missions aérostatiques du CNES, notre projet consiste à développer un **système embarqué critique** pour gérer le déventement du parachute auxiliaire attaché à la charge utile après l'atterrissage. Ce système vise à éviter les dommages potentiels sur les équipements sensibles et coûteux en cas de traînée au sol.

L'objectif principal est de concevoir un **système robuste à la panne avancée**, c'est-à-dire capable de prévenir tout incident grave en cas de défaillance du système. Par contre, le système **ne doit pas être robuste à la panne retardée**, car un retard dans le déventement ne devra pas engendrer de conséquence catastrophique.

## Software et Hardware utilisé

Software :
- [PlateformIO (IDE)](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start)
- Interfaces simulées (IP, RS232, ...)

Hardware :
- [LOLIN32 lite](https://done.land/components/microcontroller/families/esp/esp32/classicesp32/lolin32lite) : Microcontrôleur basé sur l'ESP32, utilisé pour la gestion du système embarqué, avec Bluetooth et Wifi intégré.
- Capteur de pression : Pour mesurer la pression atmosphérique et déclencher le déventement.
- Capteur GPS : Pour suivre la position de la charge utile.
- LED Rouge : Utilisée pour simuler la cisaille pyrotechnique.
- Autres composants à définir selon l'évolution des besoins du projet.

## Bibliographe et sources

_Ajouter ici les sites de références (code, doc, ...) utiles, utilisés et/ou utilisables pour le projet._
