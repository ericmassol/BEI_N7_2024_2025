# **Code pour Microcontrôleur : GPS et Gestion de l'Armement**

## **Présentation**
Ce projet implémente un système basé sur un microcontrôleur qui :
- **Récupère** les données d'un module GPS via la bibliothèque TinyGPS++.
- **Calcule** les vitesses verticale et horizontale.
- **Gère** plusieurs LEDs pour indiquer les états et conditions de fonctionnement.
- **Active** une LED d'armement lorsque toutes les conditions précises sont respectées.

---

## **Fonctionnalités**

### 1️⃣ Lecture des données GPS  
✅ Altitude, latitude, longitude, et nombre de satellites.  
✅ Validation des données GPS (données valides uniquement si à partir de 3 satellites).  

### 2️⃣ Calculs dynamiques  
✅ **Vitesse verticale** : évaluée à partir des changements d'altitude.  
✅ **Vitesse horizontale** : calculée à partir des coordonnées GPS (formule de Haversine).  

### 3️⃣ Gestion des LEDs  
✅ **LED_AUTORISATION** : Contrôlée par un bouton (bascule entre éteinte et allumée).  
✅ **LED_ETAGE1** : Similaire à LED_AUTORISATION, contrôlée par un autre bouton.  
✅ **LED_GPS** : Allumée lorsque les données GPS sont valides.  
✅ **LED_ARMER** : Allumée lorsque toutes les conditions d'armement sont remplies.  

### 4️⃣ Conditions d'armement  
✅ **Autorisation de l'opérateur** active.  
✅ **Autorisation de séparation** active.  
✅ **Données GPS valides**.  
✅ **Vitesse verticale inférieure** à un seuil prédéfini.  
✅ **Temporisation (temps seuil) respectée avant activation.**  

---

## **Configuration Matérielle**
| Composant              | Broche Microcontrôleur |
|------------------------|----------------------|
| **Microcontrôleur**    | Compatible Arduino |
| **Module GPS**        | RX : A2, TX : A3 |
| **Bouton Autorisation** | 5 |
| **Bouton Séparation**  | 4 |
| **LED Autorisation**  | 8 |
| **LED Séparation**   | 7 |
| **LED Armement**     | 6 |
| **LED GPS**         | 12 |

---

## **Détails du Code**

### 🔑 **Variables Clés**
- `verticalSpeed` : Vitesse verticale calculée (m/s).  
- `horizontalSpeed` : Vitesse horizontale calculée (m/s).  
- `Etat_GPS` : Indique si les données GPS sont valides.  
- `etat_LED_AUTORISATION`, `etat_LED_ETAGE1` : États des LEDs correspondantes.  

### 🔧 **Fonctions principales**
1. **Lecture des données GPS**  
   - Validation à partir de 3 satellites.
   - Extraction de l'altitude, latitude et longitude.
   
2. **Calculs de vitesse**  
   - Vitesse verticale basée sur les changements d'altitude.
   - Vitesse horizontale calculée avec la formule de Haversine.
   
3. **Mise à jour des LEDs**  
   - En fonction des conditions d'autorisation et de GPS.

4. **Conditions d'activation de la LED d'armement**  
   - Vérification des conditions avant activation.
   - Utilisation d'un chronomètre pour la temporisation.

---

## **Instructions d'Utilisation**
1. **Connectez** les composants aux broches indiquées.  
2. **Chargez** le code sur le microcontrôleur.  
3. **Lancez** le système et observez les comportements des LEDs :  
   - Assurez-vous que les données GPS sont valides.  
   - Testez les boutons d'autorisation et vérifiez les réactions des LEDs.  
   - Surveillez le port série pour des informations de débogage.  

---

## **Remarques**
✅ **Erreurs potentielles** : Des imprécisions peuvent apparaître dans le calcul de la vitesse horizontale dues à la formule de Haversine.  
✅ **Désactivation recommandée** : Si la vitesse horizontale n'est pas critique, désactivez cette vérification.  
✅ **Temps seuil** : Peut être ajusté selon les besoins pour éviter les activations prématurées.  
✅ **Tests réels recommandés** : Utilisez des données GPS réelles pour valider les calculs et la logique du système
