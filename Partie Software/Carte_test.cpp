#define LED_AUTORISATION 4
#define LED_ETAGE1 6
#define LED_ARMER 12
#define LED_MISE_A_FEU 13
#define AUTORISATION A5
#define ETAGE1 A4
#define MISE_A_FEU A3
#define ARMER A2

unsigned long startTime = 0; // Pour suivre le temps écoulé
bool counting = false;       // Indique si le chronomètre est actif

bool etat_LED_AUTORISATION = false; // État de la LED_AUTORISATION
bool etat_LED_ETAGE1 = false;       // État de la LED_ETAGE1

bool dernierEtat_AUTORISATION = false; // Dernier état connu du bouton AUTORISATION
bool dernierEtat_ETAGE1 = false;       // Dernier état connu du bouton ETAGE1

void setup() {
  pinMode(LED_AUTORISATION, OUTPUT);
  pinMode(LED_ETAGE1, OUTPUT);
  pinMode(LED_ARMER, OUTPUT);
  pinMode(LED_MISE_A_FEU, OUTPUT);
  pinMode(AUTORISATION, INPUT);
  pinMode(ETAGE1, INPUT);
  pinMode(MISE_A_FEU, INPUT);
  pinMode(ARMER, INPUT);

  // Initialisation des LEDs à LOW (éteintes)
  digitalWrite(LED_AUTORISATION, LOW);
  digitalWrite(LED_ETAGE1, LOW);
  digitalWrite(LED_ARMER, LOW);
  digitalWrite(LED_MISE_A_FEU, LOW);

  // Initialisation du port série (si nécessaire pour débogage)
  Serial.begin(9600);
}

void loop() {
  // Lecture des états des boutons
  bool bouton_AUTORISATION = digitalRead(AUTORISATION);
  bool bouton_ETAGE1 = digitalRead(ETAGE1);
  bool bouton_ARMER = digitalRead(ARMER);       // Lecture du bouton ARMER
  bool bouton_MISE_A_FEU = digitalRead(MISE_A_FEU); // Lecture du bouton MISE_A_FEU

  // Basculer l'état de LED_AUTORISATION lors d'un appui sur AUTORISATION
  if (bouton_AUTORISATION && !dernierEtat_AUTORISATION) { // Détection du front montant
    etat_LED_AUTORISATION = !etat_LED_AUTORISATION;       // Inverser l'état
    digitalWrite(LED_AUTORISATION, etat_LED_AUTORISATION ? HIGH : LOW);
  }
  dernierEtat_AUTORISATION = bouton_AUTORISATION; // Mettre à jour l'état précédent

  // Basculer l'état de LED_ETAGE1 lors d'un appui sur ETAGE1
  if (bouton_ETAGE1 && !dernierEtat_ETAGE1) { // Détection du front montant
    etat_LED_ETAGE1 = !etat_LED_ETAGE1;       // Inverser l'état
    digitalWrite(LED_ETAGE1, etat_LED_ETAGE1 ? HIGH : LOW);
  }
  dernierEtat_ETAGE1 = bouton_ETAGE1; // Mettre à jour l'état précédent

  // Gestion de la LED_ARMER
  if (bouton_ARMER) { // Si le bouton ARMER est appuyé
    Serial.println("ARMER activée");
    digitalWrite(LED_ARMER, HIGH);
  } else {
    digitalWrite(LED_ARMER, LOW); // Éteindre la LED si le bouton est relâché
  }

  // Gestion de la LED_MISE_A_FEU
  if (bouton_MISE_A_FEU) { // Si le bouton MISE_A_FEU est appuyé
    Serial.println("MISE A FEU activée");
    digitalWrite(LED_MISE_A_FEU, HIGH);
  } else {
    digitalWrite(LED_MISE_A_FEU, LOW); // Éteindre la LED si le bouton est relâché
  }
}
