class Foo {
  public:
    Foo(): bar_(13){}
    ~Foo(){};

    const Foo& operator=(const Foo& rhs) {
      func1();
      bar_ = rhs.bar_;
      return *this;
    }

    void func1();
  private:
    int bar_;
};

void Foo::func1 () {
  int count = 1;

  for ( int i = 0; i < 10; ++i ) {
    count += count;
  }
}
int main()
{
  Foo foo;
  foo.func1();

  Foo* foo2 = new Foo();
  foo2->func1();
  foo = *foo2;
  delete foo2;
  return 0;
}
