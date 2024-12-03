#include <Arduino.h>
#include "sauvegardeValeur.h"
#include <stdint.h>

const int seuil_effort = 10;
bool maf = false;
bool ack_operateur = false;
bool ack_separation = false;

bool maf_deventement(float effort, bool ack_operateur, bool ack_separation) {
  if (effort < seuil_effort && ack_operateur == true && ack_separation == true) { // Si l'effort est inférieur au seuil et que les deux conditions sont remplies, alors le signal de mise à feu est envoyé.
    digitalWrite(33, HIGH);   // On envoie le signal de mise à feu.
    return true;
  }
  else {
    return false;
  }
}


void setup() {
  pinMode(32, INPUT); // Ce port est le port réel (numéro sur la carte). Il est utilisé pour relever les données du capteur d'effort.
  pinMode(14,INPUT); // Port d'autorisation opérateur.
  pinMode(27,INPUT); // Port d'autorisation séparation.

  pinMode(33, OUTPUT); // Ce port est le port réel (numéro sur la carte). Il est utilisé pour transmettre le signal de mise à feu.  
  
  digitalWrite(33, LOW); // Le signal de mise à feu est initialisé à 0.
  
}

void loop() {
  int effort = analogRead(32); // Cette variable est utilisée pour stocker la valeur de l'effort mesuré par le capteur.
  
  if (!ack_operateur) {
    ack_operateur = digitalRead(14); // Cette variable est utilisée pour stocker l'autorisation de l'opérateur.
  }

  if (!ack_separation) {
    ack_separation = digitalRead(27); // Cette variable est utilisée pour stocker l'autorisation de séparation.
  }

  if (!maf) {
    maf = maf_deventement(effort, ack_operateur, ack_separation); // Cette variable indique si le parachute a été déventé.
  }
  
  // ACQUISITION DES DONNÉES
  uint16_t currentAddress = 0; // Variable globale pour suivre l'adresse actuelle
  uint16_t dataSize=32; // Taille des données à sauvegarder

  sauvegarderValeur(effort, currentAddress); // Sauvegarder la valeur de l'effort

  // Mettre à jour l'adresse actuelle
  currentAddress += dataSize;
  

  delay(1000); // On attend 1 seconde avant de recommencer la boucle.
  
}


