#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Configuration des broches RX et TX pour le GPS
SoftwareSerial gpsSerial(A2, A3); // RX, TX

// Initialisation de l'objet TinyGPS++
TinyGPSPlus gps;

// Variables pour le GPS et les vitesses
double lastAltitude = 0.0; // Dernière altitude mesurée
double verticalSpeed = 0.0; // Vitesse verticale calculée
double horizontalSpeed = 0.0; // Vitesse horizontale calculée
double lastLat = 0.0; // Dernière latitude mesurée
double lastLng = 0.0; // Dernière longitude mesurée
const double verticalSpeedSeuil = 0.05; // Seuil de vitesse verticale (m/s)
const double horizontalSpeedSeuil = 4; // Seuil de vitesse horizontale (m/s)
const int timeSeuil = 10000; // Temps seuil en millisecondes
volatile unsigned long lastTime = 0; // Dernier temps mesuré

// Chronomètre et états des LEDs
#define LED_AUTORISATION 7
#define LED_ETAGE1 8
#define LED_ARMER 6
#define LED_GPS 12
#define AUTORISATION 5
#define ETAGE1 4

volatile unsigned long startTime = 0; // Pour suivre le temps écoulé
bool counting = false; // Indique si le chronomètre est actif
bool etat_LED_AUTORISATION = false; // État de la LED_AUTORISATION
bool etat_LED_ETAGE1 = false; // État de la LED_ETAGE1
bool dernierEtat_AUTORISATION = false; // Dernier état connu du bouton AUTORISATION
bool dernierEtat_ETAGE1 = false; // Dernier état connu du bouton ETAGE1
bool Etat_GPS = false; // État du GPS (données valides ou non)

void setup() {
  pinMode(LED_AUTORISATION, OUTPUT);
  pinMode(LED_ETAGE1, OUTPUT);
  pinMode(LED_ARMER, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(AUTORISATION, INPUT);
  pinMode(ETAGE1, INPUT);

  // Initialisation des LEDs à LOW (éteintes)
  digitalWrite(LED_AUTORISATION, LOW);
  digitalWrite(LED_ETAGE1, LOW);
  digitalWrite(LED_ARMER, LOW);
  digitalWrite(LED_GPS, LOW);

  // Initialisation du port série
  Serial.begin(9600);
  gpsSerial.begin(9600);

  Serial.println("Programme de test prêt.");
}

void loop() {
  // Lecture des états des boutons
  bool bouton_AUTORISATION = digitalRead(AUTORISATION);
  bool bouton_ETAGE1 = digitalRead(ETAGE1);

  // Basculer l'état de LED_AUTORISATION lors d'un appui sur AUTORISATION
  if (bouton_AUTORISATION && !dernierEtat_AUTORISATION) {
    etat_LED_AUTORISATION = !etat_LED_AUTORISATION; // Inverser l'état
    digitalWrite(LED_AUTORISATION, etat_LED_AUTORISATION ? HIGH : LOW);
    Serial.println("LED_AUTORISATION changée.");
  }
  dernierEtat_AUTORISATION = bouton_AUTORISATION;

  // Basculer l'état de LED_ETAGE1 lors d'un appui sur ETAGE1
  if (bouton_ETAGE1 && !dernierEtat_ETAGE1) {
    etat_LED_ETAGE1 = !etat_LED_ETAGE1; // Inverser l'état
    digitalWrite(LED_ETAGE1, etat_LED_ETAGE1 ? HIGH : LOW);
    Serial.println("LED_ETAGE1 changée.");
  }
  dernierEtat_ETAGE1 = bouton_ETAGE1;

  // Lecture des données GPS
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.satellites.value() >= 3 && gps.location.isValid() && gps.altitude.isValid()) {
        // La première conditions permet de vérifier le nombre de satellites en communication, en réaliter le GPS en a besoin de 4 pour fonctionner correctement.
        // La deuxième condition vérifie si les données de localisation sont valides
        // La troisième condition vérifie si les données d'altitude sont valides
        
        double currentAltitude = gps.altitude.meters(); // Altitude actuelle
        double currentLat = gps.location.lat(); // Latitude actuelle
        double currentLng = gps.location.lng(); //  Longitude actuelle
        unsigned long currentTime = millis(); // Temps actuel

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
          String("Atitude : ") + gps.location.lat() + " °,   " +
          String("Longitude : ") + gps.location.lng() + " °,   " +
          "Vitesse verticale : " + verticalSpeed + " m/s,   " +
          "Vitesse horizontale : " + horizontalSpeed + " m/s,   "+
          "Satellites en communication : "+ gps.satellites.value()
        );
        /*Serial.println("GPS valide, données mises à jour.");*/
        digitalWrite(LED_GPS, HIGH);
      } else {
        // Cette conditions permet de vérifier et d'empecher l'activation de startTime et donc LED_AMER si les données ne sont pas valides
        Etat_GPS = false;
        Serial.println(String("Données GPS non valides, Satellites en communication : " ) + gps.satellites.value());
      }
    }
  }

  // Mise à jour des LEDs
  ////  digitalWrite(LED_GPS, (Etat_GPS && verticalSpeed < verticalSpeedSeuil && horizontalSpeed < horizontalSpeedSeuil) ? HIGH : LOW);
  digitalWrite(LED_GPS, (Etat_GPS && verticalSpeed < verticalSpeedSeuil ) ? HIGH : LOW);

  // Vérification des conditions pour LED_ARMER
  ////  if (etat_LED_AUTORISATION && etat_LED_ETAGE1 && Etat_GPS && verticalSpeed < verticalSpeedSeuil && horizontalSpeed < horizontalSpeedSeuil) 
  if (etat_LED_AUTORISATION && etat_LED_ETAGE1 && Etat_GPS && verticalSpeed < verticalSpeedSeuil) {
    if (!counting) {
      startTime = millis();
      counting = true;
    }
    if (millis() - startTime >= timeSeuil) {
      digitalWrite(LED_ARMER, HIGH);
      Serial.println("LED_ARMER activée (conditions remplies).");
    }
  } else {
    counting = false;
    startTime = 0;
    digitalWrite(LED_ARMER, LOW);
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

// Attention : Ce code contient une erreur d'acquisition due au calcul de la position sur le plan horizontal (x et y) ainsi qu'aux formules utilisées pour les déterminer. L'utilisation de la fonction atan2 peut également provoquer des erreurs.
// Les positions x et y sont calculées en fonction de la latitude et de la longitude. Cependant, lorsque nous sommes immobiles, la formule utilisée pour calculer la vitesse sur le plan horizontal peut entraîner des erreurs. 
// Ces erreurs sont dues aux imprécisions des mesures relevées pour x et y ainsi qu'au calcul de la vitesse horizontale. Elles surviennent environ toutes les trois acquisitions. Nous avons tenté de les résoudre, mais nous avons manqué de temps.
// Comme cette condition n'est pas nécessaire (puisque nous nous intéressons uniquement à la vitesse verticale), il est recommandé de la retirer avant d'exécuter le code.
// Les lignes124 et 128 sont commenter.
