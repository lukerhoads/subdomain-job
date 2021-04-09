#include <iostream>
#include <string>
#include <cpr/cpr.h>

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

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

    auto r = cpr::Post(Url{"https://httpbin.org/post"}, Multipart{{"key", "long-string"}});
    std::cout << r.text << std::endl;

    return 0;
}