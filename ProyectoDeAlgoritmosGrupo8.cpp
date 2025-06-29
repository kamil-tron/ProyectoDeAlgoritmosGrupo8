#include "JetSmartApp.h"
#include <cstdlib>

int main() {
    std::system("mode con: cols=200 lines=60");
    JetSmartApp app;
    app.run();
    return 0;
}