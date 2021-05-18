/**
 * @file modelViewStack.c
 * @author Gaia Rossi (gaia.rossi@edu.unife.it)
 * @author Enrico Bregoli (enrico.bregoli@edu.unife.it)
 * @brief FUNCTIONS FOR MODELVIEW STACK HANDMADE
 * @version 1.0
 * @date 2021-04-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "modelViewStack.h"

void initStack(modelViewStack_t *stack)
{
    stack->tail = 0;
    return;
}

void pushMatrix(modelViewStack_t *stack, mat4 modelViewMatrix)
{
    if (stack->tail == MODELVIEW_STACK_SIZE - 1)
        return;
    glm_mat4_copy(modelViewMatrix, stack->stack[stack->tail]);
    stack->tail++;
    return;
}

void popMatrix(modelViewStack_t *stack, mat4 modelViewMatrix)
{
    if (stack->tail == 0)
        return;
    stack->tail--;
    glm_mat4_copy(stack->stack[stack->tail], modelViewMatrix);
    return;
}

void printMatrix(mat4 matrix)
{
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            printf("%f ", matrix[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}