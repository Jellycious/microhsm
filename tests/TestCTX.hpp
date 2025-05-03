namespace microhsm_tests
{
    class TestCTX
    {
        public:

            void init();

            static void setFlag(void* ctx);
            static void clearFlag(void* ctx);
            static bool guardFlag(void* ctx);

            unsigned int getFlag();

        private:

            bool flag_ = false;
    };
}
