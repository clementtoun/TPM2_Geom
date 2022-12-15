#include "myApp.h"

int main(int argc, char **argv){

    Application* app = new myApp("TPM2_Geom", 1080, 720);
    app->Run();
    delete app;

    return 0;
}

