#include <TestCTX.hpp>

namespace microhsm_tests 
{

            void TestCTX::init()
            {
                effect1Count_ = 0;
                effect2Count_ = 0;
                effect3Count_ = 0;
            }

            unsigned int TestCTX::getEffectCount1()
            {
                return effect1Count_;
            }

            unsigned int TestCTX::getEffectCount2()
            {
                return effect2Count_;
            }

            unsigned int TestCTX::getEffectCount3()
            {
                return effect3Count_;
            }

            void TestCTX::effect1(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                context->effect1Count_++;
            }

            void TestCTX::effect2(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                context->effect2Count_++;
            }

            void TestCTX::effect3(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                context->effect3Count_++;
            }

}
