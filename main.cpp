#include <iostream>
#include <string>
#include <stdio.h>
#include <cpr/cpr.h>

int main(int argc, char *argv[])
{
    std::cout << "Partitioning subdomains..." << std::endl;

    if (strlen(*argv) == 0) 
    { 
        exit()
    }

    int* maxRetries;
    char* hostedZone;
    char* accessKey;
    char* secretKey;

    for (int i = 1; i < argc; ++i)
    {
        if (i == 1) 
        {
            hostedZone = argv[i];
        };
        if (i == 2)
        {
            accessKey = argv[i];
        };
        if (i == 3)
        {
            secretKey = argv[i];
        };
    }
    
    bool err = false;
    for (int i = 0; i < maxRetries; ++i) 
    {
        bool err = makeRequest(hostedZone, accessKey, secretKey)
        if (!err) 
        {
            break;
        };
        std::cout << "Request failed, retrying..." << std::endl;
    }
    
    return 0;
}

bool makeRequest(char* hostedZone, char* accessKey, char* secretKey) 
{
    // CURL *curl;
    // CURLcode res;
    // curl = curl_easy_init();
    // if(curl) {
    //     curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://route53.amazonaws.com/doc/2013-04-01/kalinpatel.com");
    //     curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //     curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    //     struct curl_slist *headers = NULL;
    //     headers = curl_slist_append(headers, "X-Amz-Date: 20210409T172252Z");
    //     headers = curl_slist_append(headers, "Authorization: AWS4-HMAC-SHA256 Credential=AKIAQ2OXEXVMITIVIE6A/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=07746d322a02278f106be145fafa2d3014088d55e7e5825da7577a9eed89f59e");
    //     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //     res = curl_easy_perform(curl);
    // }
    // curl_easy_cleanup(curl);

    cpr::Response r = cpr::Post(cpr::Url{"https://route53.amazonaws.com/doc/2013-04-01/kalinpatel.com"},
                    cpr::Header{
                        {"X-Amz-Date", "20210409T172252Z"}, // date now unix iso format
                        {"Authorization", "AWS4-HMAC-SHA256 Credential=AKIAQ2OXEXVMITIVIE6A/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=07746d322a02278f106be145fafa2d3014088d55e7e5825da7577a9eed89f59e"},
                    });
    std::cout << r.text << std::endl;
}