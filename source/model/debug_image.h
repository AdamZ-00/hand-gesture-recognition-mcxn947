/**
 * @file    debug_image.h
 * @brief   Interface du module d'envoi d'image de débogage via UART.
 *
 * Expose deux fonctions dans le namespace DebugImage :
 * - maybeSendImageOnce() : envoie l'image une seule fois (protégé par un flag)
 * - reset() : réarme le flag pour autoriser un nouvel envoi
 *
 * @author  zehad
 * @date    25 juin 2025
 */

#ifndef MODEL_DEBUG_IMAGE_H_
#define MODEL_DEBUG_IMAGE_H_


#pragma once
#include <cstdint>
#include <cstddef>

namespace DebugImage {
    void maybeSendImageOnce(const uint8_t* buffer, size_t size);
    void reset();
}



#endif /* MODEL_DEBUG_IMAGE_H_ */
