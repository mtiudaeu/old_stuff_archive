#ifndef INTRUSIVE_UNIT_TEST
#define INTRUSIVE_UNIT_TEST

#ifdef UNIT_TEST_ACTIVATED
  #define UT_DEF_FRIEND(TestClass) class friend TestClass;
  #define UT_DEF_VIRT_FUNC(function) virtual function{};
  #define UT_CALL_FUNC(function) function;
#else
  #define UT_DEF_FRIEND(TestClass)
  #define UT_DEF_VIRT_FUNC(function)
  #define UT_CALL_FUNC(function)
#endif // UNIT_TEST_ACTIVATED

#endif // INTRUSIVE_UNIT_TEST
