/**
 * @file    uart_utils.cpp
 * @brief   Utilitaire d'envoi de données brutes via UART.
 *
 * Fournit sendImageUART() qui transmet un buffer d'octets sur la console série
 * (via PUTCHAR du SDK NXP). Utilisé par debug_image.cpp pour envoyer l'image
 * d'entrée du modèle vers un PC pour vérification visuelle.
 *
 * @author  zehad
 * @date    25 juin 2025
 */



#include "uart_utils.h"
#include "fsl_debug_console.h"

extern "C" void sendImageUART(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        PUTCHAR(data[i]); // envoie 1 byte
    }
}

