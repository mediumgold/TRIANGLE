/*
 *   
 * Authors:
 * nathan WONG (40168305)
 * alexis-john villamil DE LOS REYES (40007779)
 */


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "pyramid.h"

// transformation variables
glm::vec3 translation(0.0f, 0.0f, 0.0f);            // translation vector (X, Y, Z)
float rotationAngle = 0.0f;                         // rotation angle (in radians)
float s = 1.0f;                                     // scale factor for Z-axis

// window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);                // adjusts OpenGL viewport to new window size
}

// process keyboard input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);     // ESC to close

    // translation controls 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translation.y += 0.01f;                     // Up
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translation.y -= 0.01f;                     // Down
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translation.x -= 0.01f;                     // Left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translation.x += 0.01f;                     // Right

    // rotation controls (Q and E keys)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        rotationAngle += glm::radians(1.0f);        // CCW
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        rotationAngle -= glm::radians(1.0f);        // CW

    // scaling controls (R and F keys, only in Z direction)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        s *= 1.01f;                                 // Scale up
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        s /= 1.01f;                                 // Scale down

    // reset transformations when SPACE is pressed
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        translation = glm::vec3(0.0f, 0.0f, 0.0f); // Reset position
        rotationAngle = 0.0f;                      // Reset rotation
        s = 1.0f;                                  // Reset scale
    }

    // Prevent Z-scale from becoming too small
    if (s < 0.1f) s = 0.1f;
}

int main() {
    // start GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);            // makes sure depth buffer is created

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS compatibility fix
#endif

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Pyramid", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load shader program
    unsigned int shaderProgram = createShaderProgram();

    // set up the Pyramid model
    setupPyramid();

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window); // Handle user input

        // clear screen with black background and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // apply transformations
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, translation);
        transform = glm::rotate(transform, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, s));

        // camera setup (view and projection matrices)
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -2.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // send matrices to shader
        int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // draw the pyramid
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup resources
    cleanupPyramid();
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
