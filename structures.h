#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

typedef struct mousePositionStruct {
    int mousePositionX; // Current mouse position on axis X
    int mousePositionY; // Current mouse position on axis Y
    int prevMousePositionX; // Previous mouse position on axis X
    int prevMousePositionY; // Previous mouse position on axis Y
} mousePositionStruct_t;

/**
 * La vera e propria rotazione viene aggiunta nella coda di rotazioni definita in
 * VaoDrawingInfoStructure.RotationStructure.rotHistory
 */
char inputKey;

typedef struct RotationStructure
{
    char rotHistory[MAX_ROT]; // array per le rotazioni
    int lastRotation;         // Posizione dell'ultimo elemento nella coda
} RotationStructure_t;

typedef struct VaoDrawingInfoStructure
{
    RotationStructure_t rotation;
} VaoDrawingInfoStructure_t;

typedef struct Vertex
{
    float coords[N_VERTICES]; // Coordinate omogenee
    float colors[N_VERTICES]; // RGBA
} Vertex_t;

#endif