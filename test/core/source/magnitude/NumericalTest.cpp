#include <gtest/gtest.h>
#include "core/source/magnitude/Numerical.h"

using namespace SEMBA;

TEST(NumericalTest, CanGetFileAttribute) {
	const std::string filePath = "testData/dmcwf/predefinedExcitation.1.exc";

	FileSystem::Project file(filePath);
	Source::Magnitude::Numerical num(file);

	EXPECT_EQ(file, num.getFile());
	EXPECT_EQ(file.getFullPath(), num.getFile().getFullPath());
}
