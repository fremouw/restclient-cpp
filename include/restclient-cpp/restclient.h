/**
 * @file restclient.h
 * @brief libcurl wrapper for REST calls
 * @author Daniel Schauenberg <d@unwiredcouch.com>
 * @version
 * @date 2010-10-11
 */

#ifndef INCLUDE_RESTCLIENT_H_
#define INCLUDE_RESTCLIENT_H_

#include <curl/curl.h>
#include <string>
#include <map>
#include <cstdlib>
#include "meta.h"
#include <algorithm>
#include <fstream>

class RestClientTransferCallback
{
public:
    virtual ~RestClientTransferCallback()
    {};
    
    virtual int UpdateTransferInfo( long dltotal, long dlnow, long ultotal, long ulnow ) = 0;
};

class RestClient
{
  public:
    /**
     * public data definitions
     */
    typedef std::map<std::string, std::string> headermap;
    
    typedef struct Request_s
    {
        headermap   headers;
        std::string url;
    } Request;

    typedef struct _Internal Internal;
    
    /** response struct for queries */
    typedef struct Response_s
    {
        int           code;
        std::string   body;
        headermap     headers;
        std::ostream* file;
        CURL*         curl;
        struct curl_slist* headerChunk;

        Response_s() : code( 0 ), body( "" ), headers(), file( NULL ), curl( NULL ), headerChunk( NULL )
        {}
    } Response;
    
    /** */
    typedef enum
    {
        kString,
        kFile,
    } FormType;
    
    typedef struct
    {
        std::string value;
        FormType    type;
    } FormItem;
    
    /** struct used for uploading data */
    typedef struct
    {
        const char* data;
        size_t      length;
    } UploadObject;

    //
    static void Init();
    static void CleanUp();

    // Auth
    static void ClearAuth();
    static void SetAuth( const std::string& username, const std::string& password );
    
    // HTTP GET
    static Response Get( const Request& request );
    static Response Get( const Request& request, const std::ostream* outputFile, const RestClientTransferCallback* info );
    
    static Response Post( const Request& request, const std::map<std::string, FormItem>& form );

//    // HTTP PUT
//    static response put(const std::string& url, const std::string& ctype,
//                        const std::string& data);
//    // HTTP DELETE
//    static response del(const std::string& url);

  private:
    static bool CurlSharedEasyInit   ( const Request& request, Response& response );
    static bool CurlSharedEasyCleanUp( Response& response );
    
    static size_t CurlTransferCallback( void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow );
    static size_t CurlWriteCallback   ( void *ptr, size_t size, size_t nmemb, void *userdata );
    static size_t CurlHeaderCallback  ( void *ptr, size_t size, size_t nmemb, void *userdata );
    static size_t CurlReadCallback    ( void *ptr, size_t size, size_t nmemb, void *userdata );

    static const char* kDefaultUserAgent;
    static std::string UserPassword;
    
    // trim from start
    static inline std::string &ltrim( std::string &s )
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    static inline std::string &rtrim( std::string &s )
    {
        s.erase( std::find_if(s.rbegin(), s.rend(), std::not1( std::ptr_fun<int, int>( std::isspace ) ) ).base(), s.end() );
        return s;
    }

    // trim from both ends
    static inline std::string &trim( std::string &s )
    {
        return ltrim( rtrim( s ) );
    }
};

#endif  // INCLUDE_RESTCLIENT_H_
