#include <iostream>
#include <string>
#include <cpr/cpr.h>

int main(int argc, char *argv[])
{
    std::cout << "Partitioning subdomains..." << std::endl;

    char* cred1;
    char* cred2;

    for (int i = 1; i < argc; ++i)
    {
        if (i == 1) 
        {
            cred1 = argv[i];
        };
        if (i == 2)
        {
            cred2 = argv[i];
        };
    }

    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass"},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    std::cout << r.status_code << std::endl;

    return 0;
}