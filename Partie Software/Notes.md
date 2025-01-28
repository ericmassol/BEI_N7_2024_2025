# Notes et remarques sur la partie software

## Aide pour Platform.io

Nécessité de commencer le code en définissant l'environnement :

```
[env:lolin32_lite]
platform = espressif32
board = lolin32_lite

; change microcontroller
board_build.mcu = esp32

; change MCU frequency
board_build.f_cpu = 240000000L
```

Pour plus de détails sur cette mise en oeuvre, voir [ici](https://docs.platformio.org/en/latest/boards/espressif32/lolin32.html).

## Reflexion sur microcontroleur - Effort

![Architecture détaillée](https://github.com/ericmassol/BEI_N7_2024_2025/blob/main/Partie%20Hardware/architecture_detaille.png)

Au niveau de la réception des données on considère qu'on reçoit un flottant en entrée et il faudra ajouter une boucle pour vérifier les valeurs en entrées (VS NaN notamment).

Exemple utilisation biblio AdafruitFRAM_I2C 
https://registry.platformio.org/libraries/adafruit/Adafruit%20FRAM%20I2C/examples/Generic_I2C_EEPROM/Generic_I2C_EEPROM.ino
