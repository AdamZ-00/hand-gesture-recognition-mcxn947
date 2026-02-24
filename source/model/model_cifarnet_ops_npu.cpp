/**
 * @file    model_cifarnet_ops_npu.cpp
 * @brief   Résolveur d'opérateurs TFLite pour l'accélération NPU.
 *
 * Enregistre les opérateurs nécessaires au graphe d'inférence :
 * - Softmax (couche de sortie, exécutée sur le CPU)
 * - NEUTRON_GRAPH (le reste du réseau, délégué au NPU Neutron du MCXN947)
 */

/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/kernels/neutron/neutron.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver()
{
	static tflite::MicroMutableOpResolver<2> s_microOpResolver;
	s_microOpResolver.AddSoftmax();
	s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(), tflite::Register_NEUTRON_GRAPH());

    return s_microOpResolver;
}
