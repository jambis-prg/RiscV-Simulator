#include <application.h>

int main(void)
{
    if (RSCV::Application::Init())
        RSCV::Application::Run();
    RSCV::Application::Shutdown();

    return 0;
}