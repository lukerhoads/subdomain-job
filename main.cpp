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
        exit(1);
    };

    int* maxRetries;
    char* hostedZoneName;
    char* accessKey;
    char* secretKey;
    char* signature;
    char* redirectLocation; // IPv4 required

    for (int i = 1; i < argc; ++i)
    {
        if (i == 1) 
        {
            hostedZoneName = argv[i];
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
        char* hostedZoneId = getHostedZoneId(hostedZoneName, accessKey, signature);
        bool err = createRecord(hostedZoneId, redirectLocation, accessKey, secretKey, signature);
        if (err) 
        {
            std::cout << "Request failed, retrying..." << std::endl;
        };
        break;
    }
    
    return 0;
}

// Gets the hostedzoneID with its doman/name
char* getHostedZoneId(char* hostedZoneName, char* accessKey, char* signature) 
{
    // Time preparation
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));

    // Authentication preparation
    const char *url = "https://route53.amazonaws.com/doc/2013-04-01/hostedzone";
    const char *authorization = sprintf("AWS4-HMAC-SHA256 Credential=%s/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=%s", accessKey, signature);

    // Request constructing and execution (very simple with cpr)
    cpr::Response r = cpr::Get(
                    cpr::Url{url},
                    cpr::Header{
                        {"X-Amz-Date", buf}, // date now unix iso format
                        {"Authorization", authorization},
                    });

    if (r.status_code != 200) 
    {
        std::cout << "No hosted zone with that name!" << std::endl;
        exit(1);
    };

    // Parse XML response, skipping truncation for now
}

// Creates the record with provided info
bool createRecord(char* hostedZoneId, char* redirectLocation, char* accessKey, char* secretKey, char* signature) 
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
    const char *url = sprintf("https://route53.amazonaws.com/doc/2013-04-01/hostedzone/%s/rrset/", hostedZoneId);
    const char *authorization = sprintf("AWS4-HMAC-SHA256 Credential=%s/20210409/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=%s", accessKey, signature);

    // Request constructing and execution (very simple with cpr)
    cpr::Response r = cpr::Post(
                    cpr::Url{url},
                    cpr::Header{
                        {"X-Amz-Date", buf}, // date now unix iso format
                        {"Authorization", authorization},
                        {"Content-type", "application/xml"} // or text/xml
                    },
                    cpr::Body{xmlStr});
    
    if (r.status_code != 200) 
    {
        std::cout << "Error adding A record!" << std::endl;
        return false;
    };

    std::cout << "Success!" << std::endl;
    return true;
}
