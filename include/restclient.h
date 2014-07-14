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

class RestClientTransferInfo
{
public:
  virtual ~RestClientTransferInfo()
  {};
    
  virtual int UpdateTransferInfo(long dltotal, long dlnow, long ultotal, long ulnow) = 0;
};

class RestClient
{
  public:
    /**
     * public data definitions
     */
    typedef std::map<std::string, std::string> headermap;
    
    typedef struct request_s
    {
      headermap headers;
      std::string url;
    } request;
    
    /** response struct for queries */
    typedef struct response_s
    {
      int code;
      std::string body;
      headermap headers;
      std::ostream* file;
        
      response_s() : code(0), body(""), headers(), file(NULL)
      {}
    } response;
    /** struct used for uploading data */
    typedef struct
    {
      const char* data;
      size_t length;
    } upload_object;

    /** public methods */
    // Cookies
    static void clearCookies();
    static void setCookies(const std::string& cookies);

    // Auth
    static void clearAuth();
    static void setAuth(const std::string& user,const std::string& password);
    // HTTP GET
    static response get(const request& request);
    static response get(const request& request, const std::ostream* outputFile);
    static response get(const request& request, const std::ostream* outputFile, const RestClientTransferInfo& info);
    
    static response get(const std::string& url);
    
    // HTTP POST
    static response post(const std::string& url, const std::string& ctype,
                         const std::string& data);
    // HTTP PUT
    static response put(const std::string& url, const std::string& ctype,
                        const std::string& data);
    // HTTP DELETE
    static response del(const std::string& url);

  private:
    static int TransferInfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
    
    // writedata callback function
    static size_t write_callback(void *ptr, size_t size, size_t nmemb,
                                 void *userdata);

    // header callback function
    static size_t header_callback(void *ptr, size_t size, size_t nmemb,
				  void *userdata);
    // read callback function
    static size_t read_callback(void *ptr, size_t size, size_t nmemb,
                                void *userdata);
    static const char* user_agent;
    static std::string user_pass;

    static std::string cookies;
    
    // trim from start
    static inline std::string &ltrim(std::string &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
      return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
      return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
      return ltrim(rtrim(s));
    }

};

#endif  // INCLUDE_RESTCLIENT_H_
