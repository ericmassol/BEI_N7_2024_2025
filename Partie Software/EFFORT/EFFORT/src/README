# **Code pour Microcontrôleur : Capteur d'Effort et Armement du Module**

## **Présentation**
Ce projet implémente un système de contrôle pour un microcontrôleur qui gère un **capteur d'effort** et permet **l'armement d'un module**.  
Le système :
- **Surveille** un capteur d'effort.
- **Vérifie** les autorisations de l'opérateur et de séparation.
- **Déclenche** le signal d'armement si toutes les conditions sont réunies.
- **Affiche** les autorisations via des LEDs de débogage.
- **Gère** un délai avant le déclenchement du signal d’armement.

---

## **Fonctionnalités**
### 1️⃣ Surveillance du Capteur d'Effort  
✅ Lit la valeur d'effort depuis un **capteur analogique**.  
✅ Compare l'effort à un seuil prédéfini (**SEUIL_EFFORT**).  

### 2️⃣ Logique d'Armement du Module  
✅ Vérifie deux **conditions externes** :  
   - **Autorisation de l'opérateur** (**ack_operateur**) : garantit que la charge utile est en descente.  
   - **Autorisation de séparation** (**ack_separation**) : confirme la séparation avec le ballon.  
✅ Déclenche le signal d'armement lorsque :
   - L'effort est en dessous du seuil.  
   - Les deux autorisations sont validées.  
   - Le **temps d'attente** (**TEMPS_ATTENTE**) est dépassé après l'atterrissage.  

### 3️⃣ Affichage des Autorisations  
✅ Les **LEDs** indiquent l'état des autorisations :
   - **LED opérateur** (**PIN_LED_OPERATEUR**) : état de l’autorisation de l'opérateur.  
   - **LED séparation** (**PIN_LED_SEPARATION**) : état de l’autorisation de séparation.  

### 4️⃣ Exécution en Temps Réel  
✅ Exécute la boucle de surveillance et de contrôle **toutes les secondes**.

---

## **Exigences Matérielles**
| Composant              | Broche Microcontrôleur |
|------------------------|----------------------|
| **Microcontrôleur**    | Carte compatible Arduino |
| **Capteur d'Effort**   | A0 |
| **Entrée Autorisation Opérateur** | 4 |
| **Entrée Autorisation Séparation** | 5 |
| **Sortie Signal d'Armement** | 6 |
| **LED Autorisation Opérateur** | 8 |
| **LED Autorisation Séparation** | 7 |

---

## **Détails du Code**

### 🔑 **Variables Clés**
- **SEUIL_EFFORT** : Seuil d'effort pour déclencher l’armement (**par défaut : 10**).
- **TEMPS_ATTENTE** : Temps d'attente avant l’armement après l'atterrissage (**par défaut : 10**).
- **maf** : Indique si le signal d'armement a été déclenché.
- **ack_operateur** : Indique si l'autorisation de l'opérateur est accordée.
- **ack_separation** : Indique si l'autorisation de séparation est accordée.

### 🔧 **Fonctions**
1. **maf_deventement(float effort, bool ack_operateur, bool ack_separation, int &temps)**  
   - Évalue les conditions pour déclencher le signal d'armement.  
   - Vérifie le **seuil d’effort**, les **autorisations**, et le **délai d’attente**.  
   - Active la broche de sortie si toutes les conditions sont réunies.  

---

## **Configuration**
1. **Configuration des Broches :**  
   - Entrée capteur d'effort (**A0**).  
   - Entrées des autorisations (**4 & 5**).  
   - Sortie signal d'armement (**6**).  
   - LEDs de débogage (**7 & 8**).  

2. **Initialisation :**  
   - Le signal d'armement est initialisé à **LOW**.  
   - LEDs éteintes par défaut.  

---

## **Boucle Principale**
1. Lit les valeurs du **capteur d'effort**.  
2. Lit l’état des **autorisations** et affiche sur **les LEDs**.  
3. Vérifie les conditions d'**armement**.  
4. Déclenche ou non le **signal de mise à feu**.  
5. Attente d'**1 seconde** avant la prochaine itération.  

---

## **Instructions d'Utilisation**
1. **Connectez** les composants aux broches spécifiées.  
2. **Téléchargez** le code sur le microcontrôleur.  
3. **Alimentez** le système et observez le comportement :  
   - Vérifiez les **autorisations** via les LEDs.  
   - Vérifiez que le signal d'armement se déclenche **uniquement dans des conditions valides**.  

---

## **Gestion de la Mémoire**
🔹 **Non implémentée dans cette version**.  
🔹 Pour une future version :  
   - Ajout d’une fonction de sauvegarde en EEPROM.  
   - Gestion du **débordement mémoire** avec une **plage d’adresses limitée**.  

---

## **Remarques**
✅ Assurez-vous que **SEUIL_EFFORT** et **TEMPS_ATTENTE** sont configurés selon vos besoins.  
✅ Les LEDs aident à **déboguer les autorisations**.  
✅ La gestion mémoire n’est **pas encore testée**.  

---

📌 **Auteur(s) :** Equipe BEI étudiants
📆 **Dernière mise à jour :** 29/01/25
