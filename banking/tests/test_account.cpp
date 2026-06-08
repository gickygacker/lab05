#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

TEST(AccountTest, ConstructorTest) {
    Account acc(1, 1000);
    EXPECT_EQ(acc.id(), 1);
    EXPECT_EQ(acc.GetBalance(), 1000);
}

TEST(AccountTest, GetBalanceTest) {
    Account acc(1, 500);
    EXPECT_EQ(acc.GetBalance(), 500);
}

TEST(AccountTest, ChangeBalanceWithoutLockThrows) {
    Account acc(1, 1000);
    EXPECT_THROW(acc.ChangeBalance(500), std::runtime_error);
}

TEST(AccountTest, ChangeBalanceWithLock) {
    Account acc(1, 1000);
    acc.Lock();
    acc.ChangeBalance(500);
    EXPECT_EQ(acc.GetBalance(), 1500);
    
    acc.ChangeBalance(-200);
    EXPECT_EQ(acc.GetBalance(), 1300);
}

TEST(AccountTest, LockUnlockTest) {
    Account acc(1, 1000);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(500));
    EXPECT_EQ(acc.GetBalance(), 1500);
    
    acc.Unlock();
    EXPECT_THROW(acc.ChangeBalance(500), std::runtime_error);
}

TEST(AccountTest, DoubleLockThrows) {
    Account acc(1, 1000);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(AccountTest, IdTest) {
    Account acc(42, 1000);
    EXPECT_EQ(acc.id(), 42);
}

TEST(AccountMockTest, MockGetBalance) {
    MockAccount mock_acc(1, 1000);
    EXPECT_CALL(mock_acc, GetBalance())
        .WillOnce(testing::Return(2000));
    
    EXPECT_EQ(mock_acc.GetBalance(), 2000);
}

TEST(AccountMockTest, MockChangeBalance) {
    MockAccount mock_acc(1, 1000);
    EXPECT_CALL(mock_acc, ChangeBalance(500));
    mock_acc.ChangeBalance(500);
}

TEST(AccountMockTest, MockLockUnlock) {
    MockAccount mock_acc(1, 1000);
    EXPECT_CALL(mock_acc, Lock());
    EXPECT_CALL(mock_acc, Unlock());
    
    mock_acc.Lock();
    mock_acc.Unlock();
}
