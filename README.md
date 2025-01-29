# 🎈 BEI N7 - Déventement du Parachute d'un Aérostat du CNES

📆 _Projet en cours du **14/10/2024** au **31/01/2025**_

---

## 👥 Participants

### 🔹 Référents :
- **Frédéric Mirc** _(CNES)_
- **Mme Ngueuveu** _(ENSEEIHT)_
- **[Dominique Bonnet](mailto:domi.bonnet1301@gmail.com)** _(Management de Projet)_

### 🔹 Étudiants :
- **Loïc Barthe** _(Roi du monde et de l'univers)_
- **Alexandre Paul** _(Référent communication N7/CNES)_
- **Philippe Casteres** _(Référent technique)_
- **Eric Massol** _(Partie logicielle)_

---

## 📌 Présentation du Projet

Dans le cadre des missions **aérostatiques du CNES**, ce projet vise à concevoir un **système embarqué critique** pour **gérer le déventement du parachute auxiliaire** attaché à la charge utile après l'atterrissage.

🎯 **Objectifs :**  
✅ Éviter les dommages potentiels sur les équipements sensibles et coûteux.  
✅ Concevoir un **système robuste à la panne avancée** (capable de prévenir tout incident grave en cas de défaillance).  
✅ S'assurer que le **système ne soit pas robuste à la panne retardée**, pour éviter un retard dans le déventement sans conséquences catastrophiques.

---

## 🛠️ Structure

### **🔹 Software :**
- [PlatformIO (IDE)](https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start) _(Développement du firmware)_
- Interfaces simulées (_IP, RS232, ..._)
- 📑 [README du code](https://github.com/ericmassol/BEI_N7_2024_2025/blob/main/Partie%20Software/Notes.md)

### **🔹 Hardware :**
- **[LOLIN32 Lite](https://done.land/components/microcontroller/families/esp/esp32/classicesp32/lolin32lite)** : Microcontrôleur basé sur l'ESP32 (WiFi/Bluetooth intégré).
- **Capteur de pression** : Mesure la pression atmosphérique pour déclencher le déventement.
- **Capteur GPS** : Suit la position de la charge utile.
- **LED Rouge** : Simulation de la cisaille pyrotechnique.

---

## 📚 Bibliographie et Sources

📂 **Documents fournis par le CNES - Version 2023** :
- **MP 176065 XLR** : Batterie lithium-ion haute densité énergétique.
- **Initiateurs 5201B03 - Cisaille C5025** : Cisaille pyrotechnique pour séparation des parachutes.
- **NEO-M8** : Module GNSS pour localisation précise (GPS, Galileo, etc.).
- **T401 OGi Modem** : Modem satellite pour communication GNSS et transmission de données.
- **A3LA-R** : Modem satellite Iridium (données & voix).
- **BL-SPAP-00-88-CNES** : Spécifications d'assurance produit pour systèmes aérostatiques.
- **Exigences BEI-N7** : Règles de sécurité et tolérance aux défaillances.
- **RNC-CNES-R-ST-12-02** : Règlement de sauvegarde pour activités ballons du CNES.
