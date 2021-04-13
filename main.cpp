#include <iostream>
#include <string>
#include <stdio.h>
#include <cpr/cpr.h>
#include <rapidxml.h>
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
char* getHostedZoneId(char* hostedZoneName, char* accessKey) 
{
    // XML prep
    xml_document<> doc
    xml_node<> * root_node = NULL;

    // Time preparation
    time_t now;
    time(&now);
    char* buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    const char* yearMonthDay = (char*)(aTime->tm_year + 1900) + (char*)(aTime->tm_mon + 1) + (char*)aTime->tm_mday;

    // canonical request string
    const char* canonRequest = sprintf("
        GET \n
        /doc/2013-04-01/hostedzone\n
        \n
        content-type:application/xml
        host:route53.amazonaws.com
        x-amz-content:e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
        x-amz-date:%s\n
        content-type;host;x-amz-date\n
        e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855\n
    ", buf)

    // do i sha this canonical request string?

    // Authentication preparation
    const char *url = "https://route53.amazonaws.com/doc/2013-04-01/hostedzone";
    const char *authorization = sprintf("AWS4-HMAC-SHA256 Credential=%s/%s/us-east-1/route53/aws4_request, SignedHeaders=host;x-amz-date, Signature=%s", accessKey, yearMonthDay, signature);

    // Request constructing and execution (very simple with cpr)
    cpr::Response r = cpr::Get(
                    cpr::Url{url},
                    cpr::Header{
                        {"X-Amz-Date", buf}, // date now unix iso format
                        {"Authorization", authorization},
                    });

    if (r.status_code != 200) 
    {
        std::cout << "Access error!" << std::endl;
        exit(1);
    };

    // Parse XML response, skipping truncation for now
    rapidxml.parse<0>(&r.text);
    root_node = doc.first_node("HostedZones");

    for (xml_node<> * hosted_node = root_node->first_node("HostedZone"); hosted_node; hosted_node = hosted_node->next_sibling())
    {
        for(xml_node<> * hosted_name_node = hosted_node->first_node("Name"); student_name_node; student_name_node = student_name_node->next_sibling())
        {
            if (hosted_name_node->value() == hostedZoneName)
            {
                std::cout << hosted_node->first_node("Id")->value() << std::endl;
                return hosted_node->first_node("Id")->value();
            };
        }; 
    };
};

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
};
