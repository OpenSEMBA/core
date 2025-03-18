#include <gtest/gtest.h>

#include "core/math/function/Waveform.h"
#include "core/math/SpaceGenerator.h"

using namespace std;

using namespace semba;
using namespace math;
using namespace function;

float waveformTypeAGenerator(const float t) {
	float I0_ = 218810.0;
	float alpha_ = 11354.0;
	float beta_ = 647265.0;
	float gamma_ = 5423540.0;
	return I0_ *
		(std::exp(-alpha_ * t) - std::exp(-beta_ * t)) *
		std::pow((1 - std::exp(-gamma_ * t)), 2);
}

TEST(MathFunctionWaveFormTest, createCallable) {
	Waveform::Type type = Waveform::Type::A;
	Waveform waveform = Waveform(type);
	vector<Real> time = linspace(pair<Real, Real>(0, 10), 100);

	for (size_t i = 0; i < time.size(); ++i) {
		EXPECT_EQ(waveformTypeAGenerator(time[i]), 
				waveform(time[i]));
	}
}
		