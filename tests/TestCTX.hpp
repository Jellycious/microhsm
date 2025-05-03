namespace microhsm_tests
{
    class TestCTX
    {
        public:

            void init();

            static void effect1(void* ctx);
            static void effect2(void* ctx);
            static void effect3(void* ctx);

            unsigned int getEffectCount1();
            unsigned int getEffectCount2();
            unsigned int getEffectCount3();

        private:

            unsigned int effect1Count_ = 0;
            unsigned int effect2Count_ = 0;
            unsigned int effect3Count_ = 0;
    };
}
