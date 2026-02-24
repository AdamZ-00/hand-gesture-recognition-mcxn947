/**
 * @file    output_postproc.h
 * @brief   Interface du post-traitement de la sortie d'inférence.
 *
 * Déclare MODEL_ProcessOutput() qui interprète le tenseur de sortie du modèle,
 * identifie le geste reconnu et déclenche la commande des servomoteurs.
 */

/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _OUTPUT_POSTPROCESS_H_
#define _OUTPUT_POSTPROCESS_H_

#include "model.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type, int inferenceTime);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _OUTPUT_POSTPROCESS_H_ */
