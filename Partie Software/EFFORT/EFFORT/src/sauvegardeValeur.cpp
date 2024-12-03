#include "sauvegardeValeur.h"
#include <Arduino.h>
#include<Adafruit_FRAM_I2C.h>
#include <Wire.h>
#include <stdint.h>
 

// Créer une instance de la classe FRAM
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

const int dataSize=32; // Taille des données à sauvegarder
void sauvegarderValeur(float valeur,uint16_t address) {
  // Initialiser la communication I2C
  Wire.begin();
  fram.begin();
  
  // Sauvegarder la valeur dans la FRAM

  fram.write(address, (uint8_t*)&valeur, dataSize);

}