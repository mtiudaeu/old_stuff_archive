#ifndef CONFIG_UNIT_TEST
#define CONFIG_UNIT_TEST

namespace unit_test {

  const char* getServerPort() {
    static const char* port = "1234";
    return port;
  }

}

#endif //CONFIG_UNIT_TEST

