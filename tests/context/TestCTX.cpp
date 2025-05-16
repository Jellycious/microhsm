#include <context/TestCTX.hpp>

namespace microhsm_tests
{

            void TestCTX::init()
            {
                flag_ = false;
            }

            unsigned int TestCTX::getFlag()
            {
                return flag_;
            }

            void TestCTX::setFlag(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                context->flag_ = true;
            }

            void TestCTX::clearFlag(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                context->flag_ = false;
            }

            bool TestCTX::guardFlag(void* ctx)
            {
                TestCTX* context = static_cast<TestCTX*>(ctx);
                return context->flag_;
            }

}
