# BEI N7 - Déventement du parachute d'un aérostat du CNES

_Projet courant du 14/10/2024 au 31/01/25_

## Présentation des participants impliqués sur le projet

_Référents_ : 
- Frédéri Mirc (côté CNES)
- Mme Ngueuveu (côté ENSEEIHT)

_Etudiants_ : 
- Alexandre PAUL (référent communication N7/CNES)
- Philippe CASTERES (référent technique)
- Eric MASSOL
- Loïc BARTHE

## Présentation du projet

Dans le cadre des missions aérostatiques du CNES, notre projet consiste à développer un **système embarqué critique** pour gérer le déventement du parachute auxiliaire attaché à la charge utile après l'atterrissage. Ce système vise à éviter les dommages potentiels sur les équipements sensibles et coûteux en cas de traînée au sol.

L'objectif principal est de concevoir un **système robuste à la panne avancée**, c'est-à-dire capable de prévenir tout incident grave en cas de défaillance du système. Par contre, le système **ne doit pas être robuste à la panne retardée**, car un retard dans le déventement ne devra pas engendrer de conséquence catastrophique.

## Software et Hardware utilisé

Software :
- [PlateformIO (IDE)](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start)
- Interfaces simulées (IP, RS232, ...)

Hardware :
- [LOLIN32 lite](https://done.land/components/microcontroller/families/esp/esp32/classicesp32/lolin32lite) (basée ESP32)
- Capteur de pression (voir référence)
- Capteur de position GPS
- LED rouge (pour simuler cisaille pyrotechnique - celle interne à la carte ?)
- autre ? (à définir)

## Bibliographe et sources

_Ajouter ici les sites de références (code, doc, ...) utiles, utilisés et/ou utilisables pour le projet._
