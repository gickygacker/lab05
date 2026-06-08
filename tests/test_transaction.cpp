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
    EXPECT_EQ(t.fee(), 1);
}

TEST(TransactionTest, SetFeeTest) {
    Transaction t;
    t.set_fee(10);
    EXPECT_EQ(t.fee(), 10);
}

TEST(TransactionTest, MakeTransactionSameIdThrows) {
    Account acc(1, 1000);
    Transaction t;
    EXPECT_THROW(t.Make(acc, acc, 200), std::logic_error);
}

TEST(TransactionTest, MakeTransactionNegativeSumThrows) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction t;
    EXPECT_THROW(t.Make(from, to, -100), std::invalid_argument);
}

TEST(TransactionTest, MakeTransactionSmallSumThrows) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction t;
    EXPECT_THROW(t.Make(from, to, 50), std::logic_error);
}

TEST(TransactionTest, MakeTransactionInsufficientFunds) {
    Account from(1, 100);
    Account to(2, 500);
    Transaction t;
    t.set_fee(1);
    
    bool result = t.Make(from, to, 200);
    
    EXPECT_FALSE(result);
    EXPECT_EQ(from.GetBalance(), 100);
    EXPECT_EQ(to.GetBalance(), 500);
}

TEST(TransactionTest, MakeTransactionSuccess) {
    Account from(1, 1000);
    Account to(2, 500);
    Transaction t;
    t.set_fee(5);
    
    bool result = t.Make(from, to, 200);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(to.GetBalance(), 495);
}

TEST(TransactionMockTest, MockSaveToDataBase) {
    MockAccount from(1, 1000);
    MockAccount to(2, 500);
    MockTransaction t;
    
    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(t, SaveToDataBase(testing::Ref(from), testing::Ref(to), 200)).Times(1);
    
    t.Make(from, to, 200);
}

TEST(TransactionMockTest, MockFeeCalculation) {
    MockTransaction t;
    t.set_fee(15);
    EXPECT_EQ(t.fee(), 15);
}
