#include "Core/Application.h"

namespace nebula {

class NebulaApp : public Application {
public:
    NebulaApp() = default;
    ~NebulaApp() override = default;
};

Application* CreateApplication() {
    return new NebulaApp();
}

} // namespace nebula

int main() {
    auto app = nebula::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
