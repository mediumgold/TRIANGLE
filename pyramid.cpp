/*
 *
 * Authors:
 * nathan WONG (40168305)
 * alexis-john villamil DE LOS REYES (40007779)
 */

#include <GL/glew.h>
#include "pyramid.h"

unsigned int VBO[2], VAO[2], EBO[2];  // two EBOs, one for base, one for sides


// define vertices for the pyramid with solid colors per face
float baseVertices[] = {
    // Base (Yellow)
    -0.5f, 0.0f, -0.5f,    1.0f, 1.0f, 0.0f,  // Bottom-left
     0.5f, 0.0f, -0.5f,    1.0f, 1.0f, 0.0f,  // Bottom-right
     0.5f, 0.0f,  0.5f,    1.0f, 1.0f, 0.0f,  // Top-right
    -0.5f, 0.0f,  0.5f,    1.0f, 1.0f, 0.0f   // Top-left
};

float sideVertices[] = {
    // side 1 (Blue)
    -0.5f, 0.0f, -0.5f,    0.0f, 0.0f, 1.0f,  // Bottom-left
     0.5f, 0.0f, -0.5f,    0.0f, 0.0f, 1.0f,  // Bottom-right
     0.0f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  // Apex (Blue)

     // side 2 (Red)
      0.5f, 0.0f, -0.5f,    1.0f, 0.0f, 0.0f,  // Bottom-right
      0.5f, 0.0f,  0.5f,    1.0f, 0.0f, 0.0f,  // Top-right
      0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,  // Apex (Red)

      // side 3 (Orange)
       0.5f, 0.0f,  0.5f,    1.0f, 0.5f, 0.0f,  // Top-right
      -0.5f, 0.0f,  0.5f,    1.0f, 0.5f, 0.0f,  // Top-left
       0.0f,  0.5f,  0.0f,   1.0f, 0.5f, 0.0f,  // Apex (Orange)

       // side 4 (Green)
       -0.5f, 0.0f,  0.5f,    0.0f, 1.0f, 0.0f,  // Top-left
       -0.5f, 0.0f, -0.5f,    0.0f, 1.0f, 0.0f,  // Bottom-left
        0.0f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f   // Apex (Green)
};

unsigned int baseIndices[] = {
    0, 1, 2,
    2, 3, 0
};

unsigned int sideIndices[] = {
    0, 1, 2,  // Side 1 (Blue)
    3, 4, 5,  // Side 2 (Red)
    6, 7, 8,  // Side 3 (Orange)
    9, 10, 11 // Side 4 (Green)
};

void setupPyramid() {
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);

    // setup VAO for Base
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);  // base
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(baseIndices), baseIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // setup VAO for sides
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sideVertices), sideVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);  // Use EBO[1] for the sides
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sideIndices), sideIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void cleanupPyramid() {
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(2, EBO);
}

