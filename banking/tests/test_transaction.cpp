#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Transaction.h"
#include "Account.h"


class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};


class MockTransacti…ектами
TEST(TransactionMockTest, MockSaveToDataBase) {
    MockAccount from(1, 1000);
    MockAccount to(2, 500);
    MockTransaction t;
    
    EXPECT_CALL(from, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(to, ChangeBalance(testing::_)).Times(1);
    EXPECT_CALL(t, SaveToDataBase(testing::Ref(from), testing::Ref(to), 200)).Times(1);
    
    t.Make(from, to, 200);
}

TEST(TransactionMockTest, MockFeeCalculation) {
    MockTransaction t;
    t.set_fee(15);
    EXPECT_EQ(t.fee(), 15);
}
