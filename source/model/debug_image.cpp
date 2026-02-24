/**
 * @file    debug_image.cpp
 * @brief   Envoi ponctuel d'une image brute via UART pour débogage.
 *
 * Permet d'envoyer une seule fois l'image d'entrée du modèle (96×96×3 octets)
 * sur la liaison série, encadrée par des marqueurs IMG_START/IMG_END.
 * Utile pour vérifier visuellement ce que le modèle « voit » côté PC.
 * L'envoi est one-shot (flag alreadySent) pour ne pas saturer l'UART.
 *
 * @author  zehad
 * @date    25 juin 2025
 */



#include "debug_image.h"
#include "uart_utils.h"

namespace DebugImage {
    static bool alreadySent = false;

    void maybeSendImageOnce(const uint8_t* buffer, size_t size) {
        if (alreadySent) return;

        const char header[] = "IMG_START\n";
        const char footer[] = "\nIMG_END\n";

        sendImageUART(reinterpret_cast<const uint8_t*>(header), sizeof(header) - 1);
        sendImageUART(buffer, size);
        sendImageUART(reinterpret_cast<const uint8_t*>(footer), sizeof(footer) - 1);

        alreadySent = true;
    }

    void reset() {
        alreadySent = false;
    }
}

