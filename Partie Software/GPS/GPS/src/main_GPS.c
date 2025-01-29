#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Configuration des broches RX et TX pour le GPS
SoftwareSerial gpsSerial(A2, A3); // RX, TX

// Initialisation de l'objet TinyGPS++
TinyGPSPlus gps;

// Variables pour le GPS et les vitesses
volatile double lastAltitude = 0.0; // Dernière altitude mesurée
volatile double verticalSpeed = 0.0; // Vitesse verticale calculée
volatile double horizontalSpeed = 0.0; // Vitesse horizontale calculée
volatile double lastLat = 0.0; // Dernière latitude mesurée
volatile double lastLng = 0.0; // Dernière longitude mesurée
const double verticalSpeedSeuil = 0.05; // Seuil de vitesse verticale (m/s)
const double horizontalSpeedSeuil = 4; // Seuil de vitesse horizontale (m/s)
const int timeSeuil = 10000; // Temps seuil en millisecondes
volatile unsigned long lastTime = 0; // Dernier temps mesuré

// Chronomètre et états des LEDs
#define PIN_LED_OPERATEUR 8
#define PIN_LED_SEPARATION 7
#define PIN_ARMEMENT 6
#define PIN_CAPTEUR_GPS 12
#define PIN_ACK_OPERATEUR 4
#define PIN_LED_SEPARATION 5

volatile unsigned long startTime = 0; // Pour suivre le temps écoulé
volatile bool counting = false; // Indique si le chronomètre est actif
volatile bool etat_PIN_LED_OPERATEUR = false; // État de la PIN_LED_OPERATEUR
volatile bool etat_PIN_LED_SEPARATION = false; // État de la PIN_LED_SEPARATION
volatile bool dernierEtat_PIN_ACK_OPERATEUR = false; // Dernier état connu du bouton PIN_ACK_OPERATEUR
volatile bool dernierEtat_PIN_LED_SEPARATION = false; // Dernier état connu du bouton PIN_LED_SEPARATION
volatile bool Etat_GPS = false; // État du GPS (données valides ou non)

void setup() {
  pinMode(PIN_LED_OPERATEUR, OUTPUT);
  pinMode(PIN_LED_SEPARATION, OUTPUT);
  pinMode(PIN_ARMEMENT, OUTPUT);
  pinMode(PIN_CAPTEUR_GPS, OUTPUT);
  pinMode(PIN_ACK_OPERATEUR, INPUT);
  pinMode(PIN_LED_SEPARATION, INPUT);

  // Initialisation des LEDs à LOW (éteintes)
  digitalWrite(PIN_LED_OPERATEUR, LOW);
  digitalWrite(PIN_LED_SEPARATION, LOW);
  digitalWrite(PIN_ARMEMENT, LOW);
  digitalWrite(PIN_CAPTEUR_GPS, LOW);

  // Initialisation du port série
  Serial.begin(9600);
  gpsSerial.begin(9600);

  Serial.println("Programme de test prêt.");
}

void loop() {
  // Lecture des états des boutons
  bool bouton_PIN_ACK_OPERATEUR = digitalRead(PIN_ACK_OPERATEUR);
  bool bouton_PIN_LED_SEPARATION = digitalRead(PIN_LED_SEPARATION);

  // Basculer l'état de PIN_LED_OPERATEUR lors d'un appui sur PIN_ACK_OPERATEUR
  if (bouton_PIN_ACK_OPERATEUR && !dernierEtat_PIN_ACK_OPERATEUR) {
    etat_PIN_LED_OPERATEUR = !etat_PIN_LED_OPERATEUR; // Inverser l'état
    digitalWrite(PIN_LED_OPERATEUR, etat_PIN_LED_OPERATEUR ? HIGH : LOW);
    Serial.println("PIN_LED_OPERATEUR changée.");
  }
  dernierEtat_PIN_ACK_OPERATEUR = bouton_PIN_ACK_OPERATEUR;

  // Basculer l'état de PIN_LED_SEPARATION lors d'un appui sur PIN_LED_SEPARATION
  if (bouton_PIN_LED_SEPARATION && !dernierEtat_PIN_LED_SEPARATION) {
    etat_PIN_LED_SEPARATION = !etat_PIN_LED_SEPARATION; // Inverser l'état
    digitalWrite(PIN_LED_SEPARATION, etat_PIN_LED_SEPARATION ? HIGH : LOW);
    Serial.println("PIN_LED_SEPARATION changée.");
  }
  dernierEtat_PIN_LED_SEPARATION = bouton_PIN_LED_SEPARATION;

  // Lecture des données GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.satellites.value() >= 3 && gps.location.isValid() && gps.altitude.isValid()) {
        // La première conditions permet de vérifier le nombre de satellites en communication, en réaliter le GPS en a besoin de 4 pour fonctionner correctement.
        // La deuxième condition vérifie si les données de localisation sont valides
        // La troisième condition vérifie si les données d'altitude sont valides

        double currentAltitude = gps.altitude.meters(); // Altitude actuelle
        double currentLat = gps.location.lat(); //  Latitude actuelle
        double currentLng = gps.location.lng(); //  Longitude actuelle
        unsigned long currentTime = millis(); //  Temps actuel

        if (lastTime != 0) {
          // Calcul de la vitesse verticale
          double deltaAltitude = currentAltitude - lastAltitude;
          double deltaTime = (currentTime - lastTime) / 1000.0; // En secondes

          if (deltaTime > 0) { // Éviter la division par zéro
            verticalSpeed = abs(deltaAltitude / deltaTime);
            horizontalSpeed = haversineDistance(lastLat, lastLng, currentLat, currentLng) / deltaTime;
          }
        }

        // Mettre à jour les dernières valeurs
        lastAltitude = currentAltitude;
        lastLat = currentLat;
        lastLng = currentLng;
        lastTime = currentTime;
        Etat_GPS = true;
        Serial.println(
          String("Altitude : ") + gps.altitude.meters() + " m,   " +
          String("Latitude : ") + gps.location.lat() + " °,   " +
          String("Longitude : ") + gps.location.lng() + " °,   " +
          "Vitesse verticale : " + verticalSpeed + " m/s,   " +
          "Vitesse horizontale : " + horizontalSpeed + " m/s,   "+
          "Satellites en communication : " + gps.satellites.value()
        );
        /*Serial.println("GPS valide, données mises à jour.");*/
        digitalWrite(PIN_CAPTEUR_GPS, HIGH);
      } else {
        // Cette conditions permet de vérifier et d'empecher l'activation de startTime et donc LED_AMER si les données ne sont pas valides
        Etat_GPS = false;
        Serial.println(String("Données GPS non valides, Satellites en communication : ") + gps.satellites.value());
      }
    }
  }

  // Mise à jour des LEDs
  ////  digitalWrite(PIN_CAPTEUR_GPS, (Etat_GPS && verticalSpeed < verticalSpeedSeuil && horizontalSpeed < horizontalSpeedSeuil) ? HIGH : LOW);
  digitalWrite(PIN_CAPTEUR_GPS, (Etat_GPS && verticalSpeed < verticalSpeedSeuil) ? HIGH : LOW);

  // Vérification des conditions pour PIN_ARMEMENT
  ////  if (etat_PIN_LED_OPERATEUR && etat_PIN_LED_SEPARATION && Etat_GPS && verticalSpeed < verticalSpeedSeuil && horizontalSpeed < horizontalSpeedSeuil) 
  if (etat_PIN_LED_OPERATEUR && etat_PIN_LED_SEPARATION && Etat_GPS && verticalSpeed < verticalSpeedSeuil) {
    if (!counting) {
      startTime = millis();
      counting = true;
    }
    if (millis() - startTime >= timeSeuil) {
      digitalWrite(PIN_ARMEMENT, HIGH);
      Serial.println("PIN_ARMEMENT activée (conditions remplies).");
    }
  } else {
    counting = false;
    startTime = 0;
    digitalWrite(PIN_ARMEMENT, LOW);
  }
}

// Fonction pour calculer la distance entre deux points GPS (en mètres)
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
  const double R = 6371000; // Rayon de la Terre en mètres
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);

  lat1 = radians(lat1);
  lat2 = radians(lat2);

  double a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1) * cos(lat2) * sin(dLon / 2) * sin(dLon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c; // Distance en mètres
}

// Attention : Ce code contient une erreur d'acquisition due au calcul de la position sur le plan horizontal (x et y) ainsi qu'aux formules utilisées pour les déterminer. L'utilisation de la fonction atan2 provoque des erreurs du a la division.
// Les positions x et y sont calculées en fonction de la latitude et de la longitude. Cependant, lorsque nous sommes immobiles, la formule utilisée pour calculer la vitesse sur le plan horizontal peut entraîner des erreurs. 
// Ces erreurs sont dues aux imprécisions des mesures relevées pour x et y ainsi qu'au calcul de la vitesse horizontale. Elles surviennent environ toutes les trois acquisitions. Nous avons tenté de les résoudre, mais nous avons manqué de temps.
// Comme cette condition n'est pas nécessaire (puisque nous nous intéressons uniquement à la vitesse verticale), il est recommandé de la retirer avant d'exécuter le code.
// Les lignes 128 et 132 sont commenter.