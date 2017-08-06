// This file is designed to run unit tests.
// Arduino editor will ignore it, since it has a .c++ extension.
// But this is not a problem for g++

#include "kawako.h"
#include "gtest/gtest.h"
#include "cmath"
#include <string>
using namespace std;

namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FooTest() {
    // You can do set-up work for each test here.
  }

  virtual ~FooTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

TEST_F(FooTest, idFunctionMan) {
    string HTTP_req_GET = "GET /?COLR=%23000000&TEXT=coucou HTTP/1.1\r\n"
                      "Host: 10.12.1.51\r\n"
                      "Connection: keep-alive\r\n"
                      "Upgrade-Insecure-Requests: 1\r\n"
                      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\n"
                      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*\r\n"
                      "\r\n";

                      // "\n";
    // string HTTP_req_SIMPLE = "GET / HTTP/1.1\r\n"
    //                   "Host: 10.12.1.51\r\n"
    //                   "Connection: keep-alive\r\n"
    //                   "Upgrade-Insecure-Requests: 1\r\n"
    //                   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\n"
    //                   "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*\r\n"
    //                   "\n";
    string HTTP_req_SIMPLE = "GET / HTTP/1.1\r\nHost: 10.12.1.51\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.8,fr;q=0.6,de;q=0.4\r\n\r\n";
    string HTTP_req_favicon = "GET /favicon HTTP/1.1\r\n"
                      "Host: 10.12.1.51\r\n"
                      "Connection: keep-alive\r\n"
                      "Upgrade-Insecure-Requests: 1\r\n"
                      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\n"
                      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*\r\n"
                      "\r\n";
                      // "\n";
    string HTTP_req = HTTP_req_SIMPLE;
    for (int i = 0; i < HTTP_req.length(); ++i) {
      bool userProvidedData = false;
      char* text_c_str = nullptr;
      char c = HTTP_req[i];
      CRGB color(42, 42, 42);
      unsigned char requestType = 0;
      bool finished = parseRequest(c, text_c_str, color, userProvidedData, requestType);

      if (i == HTTP_req.length() - 1) { // finished
        EXPECT_EQ(finished, true);
        EXPECT_EQ(requestType, REQUESTED_SLASH);
      } else {
        EXPECT_EQ(finished, false);
        EXPECT_EQ(requestType, 0);
      }
      EXPECT_EQ(userProvidedData, false);
    }
    HTTP_req = HTTP_req_favicon;
    for (int i = 0; i < HTTP_req.length(); ++i) {
      bool userProvidedData = false;
      char* text_c_str = nullptr;
      char c = HTTP_req[i];
      CRGB color(42, 42, 42);
      unsigned char requestType = 0;
      bool finished = parseRequest(c, text_c_str, color, userProvidedData, requestType);

      if (i == HTTP_req.length() - 1) { // finished
        EXPECT_EQ(finished, true);
        EXPECT_EQ(requestType, REQUESTED_FAVICON);
      } else {
        EXPECT_EQ(finished, false);
        EXPECT_EQ(requestType, 0);
      }
      EXPECT_EQ(userProvidedData, false);
    }
    HTTP_req = HTTP_req_GET;
    for (int i = 0; i < HTTP_req.length(); ++i) {
      bool userProvidedData = false;
      char* text_c_str = nullptr;
      char c = HTTP_req[i];
      CRGB color(42, 42, 42);
      unsigned char requestType = 0;
      bool finished = parseRequest(c, text_c_str, color, userProvidedData, requestType);

      if (i == HTTP_req.length() - 1) { // finished
        EXPECT_EQ(finished, true);
        EXPECT_EQ(requestType, REQUESTED_GET);
        EXPECT_EQ(string(text_c_str), string("coucou"));
        EXPECT_EQ(color.r, 0);
        EXPECT_EQ(color.g, 0);
        EXPECT_EQ(color.b, 0);
      } else {
        EXPECT_EQ(finished, false);
        EXPECT_EQ(requestType, 0);
        EXPECT_EQ(color.r, 42);
        EXPECT_EQ(color.g, 42);
        EXPECT_EQ(color.b, 42);
      }
      EXPECT_EQ(userProvidedData, false);
    }
    EXPECT_EQ(1, 1);
}

}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
