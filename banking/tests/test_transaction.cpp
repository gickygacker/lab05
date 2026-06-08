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

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(TransactionTest, ConstructorTest) {
    Transaction t;
    EXPECT_EQ(t.fee(), 0);
}

TEST(TransactionTest, SetFeeTest) {
    Transaction t;
    t.set_fee(10);
    EXPECT_EQ(t.fee(), 10);
}

TEST(TransactionTest, MakeTransactionTest) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction t;
    t.set_fee(5);
    
    bool result = t.Make(from, to, 200);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(from.GetBalance(), 795); 
    EXPECT_EQ(to.GetBalance(), 700);
}

TEST(TransactionTest, InsufficientFundsTest) {
    Account from(1, 100);
    Account to(2, 500);
    Transaction t;
    
    bool result = t.Make(from, to, 200);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), 100);
    EXPECT_EQ(to.GetBalance(), 500);
}

TEST(TransactionTest, ZeroSumTest) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction t;
    
    bool result = t.Make(from, to, 0);
    
    EXPECT_FALSE(result);
}

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
