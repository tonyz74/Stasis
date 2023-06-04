#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>

int main(void) {
    using namespace std;

    glfwInit();

    cout << "Hello!" << endl;
    printf("Hello, world!\n");

    glfwTerminate();

    return 0;
}
