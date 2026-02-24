/**
 * @file    uart_utils.h
 * @brief   Interface de l'utilitaire d'envoi UART.
 *
 * Déclare sendImageUART() en linkage C pour compatibilité avec les fichiers .c et .cpp.
 *
 * @author  zehad
 * @date    25 juin 2025
 */

#ifndef MODEL_UART_UTILS_H_
#define MODEL_UART_UTILS_H_

#pragma once

#include <cstdint>
#include <cstddef>

extern "C" void sendImageUART(const uint8_t* data, size_t length);



#endif /* MODEL_UART_UTILS_H_ */
