#include "../Singularity.hpp"
#include <gtest/gtest.h>

using namespace Jah;

class SingularityTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize test resources
    atom = new Atom("feat", "add feature", "body", "footer");
    hook = new Hook(C);
    singularity = new Singularity(testId, atom, testLanguage, hook);
  }

  void TearDown() override {
    // Cleanup resources
    if (singularity)
      delete singularity;
    if (atom)
      delete atom;
    if (hook)
      delete hook;
  }

  Atom *atom = nullptr;
  Hook *hook = nullptr;
  Singularity *singularity = nullptr;
  const unsigned testId = 1;       // Changed to const
  const Language testLanguage = C; // Changed to const
};

TEST_F(SingularityTest, ConstructorInitializesCorrectly) {
  EXPECT_NE(nullptr, singularity);
  EXPECT_FALSE(singularity->isAccepted());
}

TEST_F(SingularityTest, CheckThrowsWhenNoIssueExists) {
  EXPECT_THROW(singularity->check(), std::runtime_error);
}

TEST_F(SingularityTest, HandlesDifferentLanguages) {
  // Create separate instances for this test
  const auto testAtom = new Atom("feat", "add feature", "body",
                                 "footer issue #42"); // Added issue reference
  const auto testHook = new Hook(C);

  Singularity pythonSingularity(42, testAtom, PYTHON,
                                testHook); // Changed testId to match issue
  Singularity javaSingularity(42, testAtom, JAVA,
                              testHook); // Changed testId to match issue

  EXPECT_THROW(pythonSingularity.check(), std::runtime_error);
  EXPECT_THROW(javaSingularity.check(), std::runtime_error);

  // Additional assertions to verify issue handling
  EXPECT_FALSE(pythonSingularity.isAccepted());
  EXPECT_FALSE(javaSingularity.isAccepted());

  delete testAtom;
  delete testHook;
}

TEST_F(SingularityTest, NullPointerHandling) {
  EXPECT_THROW(Singularity(testId, nullptr, testLanguage, hook),
               std::invalid_argument);
  EXPECT_THROW(Singularity(testId, atom, testLanguage, nullptr),
               std::invalid_argument);
}

TEST_F(SingularityTest, AcceptanceStateManagement) {
  EXPECT_FALSE(singularity->isAccepted());
}

