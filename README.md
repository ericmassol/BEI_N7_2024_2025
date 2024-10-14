# BEI N7 - Déventement du parachute d'un aérostat du CNES (2024/2025)

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

Dans le cadre des missions ballons du CNES, ils ont un besoin de développer un système embarqué permettant de déventer le parachute auxiliaire accroché à la charge utile une fois qu'il a attérrit pour éviter des dommages sur les équipements (coûteux). 

Notre mission est donc de développer ce système embarqué critique de façon à ce qu'il soit robuste à la panne avance (toute défaillance du système ne doit pas entrainer de catastrophe) et non robuste à la panne retard (un retard de déventement ne doit pas déclencher de catastrophe). 

## Software et Hardware utilisé

Software :
- PlateformIO (IDE)
- Interfaces simulées (IP, RS232, ...)

Hardware :
- LOLIN32 lite (type ESP32)
- Capteur de pression (voir référence)
- Capteur de position GPS
- LED rouge (pour simuler cisaille pyrotechnique)
- autre ? (à définir)
