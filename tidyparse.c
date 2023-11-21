#include <stdio.h>
#include "curl.h"
#include "tidy.h"
#include "tidybuffio.h"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    TidyBuffer *furl = (TidyBuffer *)userp;
    tidyBufAppend(furl, contents, realsize);
    return realsize;
}

int main(void) {
    CURL *c;
    CURLcode r;
    TidyBuffer html_content = {0};

    c = curl_easy_init();


    if (c) {
        const char *url = "http://127.0.0.1:5500/tidyparse2.html";
        printf("start");
        printf("%s",url);
        printf("end");
        curl_easy_setopt(c, CURLOPT_URL, url);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &html_content);
        printf("%d",111);
        
        printf("111 end   ");
    

        r = curl_easy_perform(c);

        if (r == CURLE_OK) {
            long http_response_code;
            curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &http_response_code);
            printf("HTTP Response Code: %ld\n", http_response_code);

            char *furl;
            curl_easy_getinfo(c, CURLINFO_EFFECTIVE_URL, &furl);
            printf(furl);

        
        
        }
        curl_easy_cleanup(c);
        }
        return 0;
        }

            
            // Initialize Tidy
            TidyDoc tdoc = tidyCreate();
            tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
            tidyOptSetBool(tdoc, TidyForceOutput, yes);

            // Parse the HTML content
            int rc = tidyParseBuffer(tdoc, &html_content);
            if (rc >= 0) {
                // Tidy parsing successful

                TidyBuffer errbuf = {0};
                tidyOptSetBool(tdoc, TidyShowWarnings, yes);
                tidyOptSetBool(tdoc, TidyShowErrors, yes);
                tidySetErrorBuffer(tdoc, &errbuf);
                rc = tidyCleanAndRepair(tdoc);
                if (rc >= 0) {
                    rc = tidyRunDiagnostics(tdoc);
                    if (rc > 0) {
                        printf("Tidy found %d errors:\n%s\n", rc, errbuf.bp);
                    }

                    TidyNode root = tidyGetRoot(tdoc);

                    // Manually traverse the HTML tree to find anchor tags and extract href attributes
                    TidyNode node = root;
                    while (node) {
                        printf("%d",0);
                        if (tidyNodeGetType(node) == TidyTag_A) {
                            printf("%d",1);
                            TidyAttr attr = tidyAttrFirst(node);
                            printf("%d",2);
                            while (attr) {
                                if (strcmp(tidyAttrName(attr), "href") == 0) {
                                    printf("Href: %s\n", tidyAttrValue(attr));
                                }
                                else {
                                    printf("fail big time");
                                }
                            attr = tidyAttrNext(attr);
                            }
                        }
                    node = tidyGetNext(node);
                    }

                    // Clean up Tidy resources
                    tidyBufFree(&errbuf);
                    tidyBufFree(&html_content); // Free the HTML content buffer
                    tidyRelease(tdoc);
                } else {
                    fprintf(stderr, "Tidy cleaning failed.\n");
                }
            } else {
                fprintf(stderr, "Tidy parsing failed with error code: %d\n", rc);
            }
        } else {
            fprintf(stderr, "Curl request failed: %s\n", curl_easy_strerror(r));
        }

       curl_easy_cleanup(c);
    } else {
        fprintf(stderr, "Curl initialization failed.\n");
    }

    return 0;
}