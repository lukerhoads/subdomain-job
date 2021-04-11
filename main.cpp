#include <iostream>
#include <string>
#include <stdio.h>
#include <cpr/cpr.h>
#include <ctime>

int main(int argc, char *argv[])
{
    std::cout << "Partitioning subdomains..." << std::endl;

    if (strlen(*argv) == 0) 
    { 
        exit(400);
    };

    int* maxRetries;
    char* hostedZone;
    char* accessKey;
    char* secretKey;
    char* signature;
    // MUST BE IPv4
    char* redirectLocation;

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
        if (i == 4)
        {
            signature = argv[i];
        };
        if (i == 5)
        {
            redirectLocation = argv[i];
        };
    }
    
    bool err = false;
    for (int i = 0; i < maxRetries; ++i) 
    {
        bool err = createRecord(hostedZone, redirectLocation, accessKey, secretKey, signature)
        if (!err) 
        {
            break;
        };
        std::cout << "Request failed, retrying..." << std::endl;
    }
    
    return 0;
}

// Gets the hostedzoneID with its doman/name
char getHostedZoneId() {

}

// Creates the record with provided info
bool createRecord(char* hostedZone, char* redirectLocation, char* accessKey, char* secretKey, char* signature) 
{
    // Time preparation
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    
    // XML preparation
    const char *xmlStr = "
        <?xml version=\"1.0\" encoding=\"UTF-8\"?>
        <ChangeResourceRecordSetsRequest xmlns=\"https://route53.amazonaws.com/doc/2013-04-01/\">
            <ChangeBatch>
                <Changes>
                    <Change>
                        <Action>CREATE</Action>
                        <ResourceRecordSet>
                            <Name>%s</Name>
                            <Type>A</Type>
                            <TTL>200</TTL>
                            <ResourceRecords>
                                <ResourceRecord>
                                    <Value>%s</Value>
                                </ResourceRecord>
                            </ResourceRecords>
                        </ResourceRecordSet>
                    </Change>
                </Changes>
            </ChangeBatch>
        </ChangeResourceRecordSetsRequest>
    ", hostedZone, redirectLocation);
    
    // Authentication preparation
    const char *url = sprintf("https://route53.amazonaws.com/doc/2013-04-01/hostedzone/%s/rrset/", hostedZone);
    const char *authorization = sprintf("AWS4-HMAC-SHA256 Credential=%s/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=%s", accessKey, signature);
    cpr::Response r = cpr::Post(
                    cpr::Url{url},
                    cpr::Header{
                        {"X-Amz-Date", buf}, // date now unix iso format
                        {"Authorization", authorization},
                        {"Content-type", "application/xml"} // or text/xml
                    },
                    cpr::Body{xmlStr});
    std::cout << r.text << std::endl;
}

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
//     headers = curl_slist_append(headers, "Authorization: AWS4-HMAC-SHA256 Credential=AKIAQ2OXEXVMITIVIE6A/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=e29ae6bcdfe4f99c72c6d1a98a6aa0cceefa517e52e6b16d7b4a932de3c336e6");
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//     res = curl_easy_perform(curl);
// }
// curl_easy_cleanup(curl);