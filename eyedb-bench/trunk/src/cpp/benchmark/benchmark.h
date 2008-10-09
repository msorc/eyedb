namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:
      Benchmark();

      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void measure();

    private:
    };
  };
};
