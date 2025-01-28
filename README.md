# BEI N7 - Déventement du parachute d'un aérostat du CNES

_Projet courant du 14/10/2024 au 31/01/25_

## Présentation des participants impliqués sur le projet

_Référents_ : 
- Frédéri Mirc (côté CNES)
- Mme Ngueuveu (côté ENSEEIHT)
- [Dominique Bonnet](domi.bonnet1301@gmail.com) (côté Management de Projet)

_Etudiants_ : 
- Loïc BARTHE (roi du monde et de l'univers)
- Alexandre PAUL (référent communication N7/CNES)
- Philippe CASTERES (référent technique)
- Eric MASSOL (Partie logicielle)


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
- LED Rouge : Utilisée pour simuler la cisaille pyrotechnique (intégrée sur la carte).

## Bibliographe et sources

Documents fournis par le CNES pour la version 2023 du projet : 
- MP 176065 xlr : Batterie lithium-ion haute densité énergétique.
- Initiateurs 5201B03 Cisaille C5025 : Cisaille pyrotechnique pour la séparation des parachutes.
- NEO-M8 : Module GNSS pour localisation précise via GPS, Galileo, et autres.
- T401 OGi Modem : Modem satellite pour communication GNSS et données.
- A3LA-R : Modem satellite Iridium pour transmission de données et voix.
- BL-SPAP-00-88-CNES : Spécifications d'assurance produit pour les systèmes aérostatiques.
- Exigences BEI-N7 : Règles de sécurité et tolérance aux défaillances pour le projet.
- RNC-CNES-R-ST-12-02 : Règlement de sauvegarde pour les activités ballons du CNES.
