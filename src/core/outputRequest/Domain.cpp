

#include "Domain.h"

#include <iostream>

namespace semba {
namespace OutputRequest {

Domain::Domain() {
    timeDomain_ = false;
    initialTime_ = 0.0;
    finalTime_ = 0.0;
    samplingPeriod_ = 0.0;
    frequencyDomain_ = false;
    initialFrequency_ = 0.0;
    finalFrequency_ = 0.0;
    frequencyStep_ = 0.0;
    logFrequencySweep_ = false;
    usingTransferFunction_ = false;
}

Domain::Domain(bool timeDomain,
               math::Real initialTime,
               math::Real finalTime,
               math::Real samplingPeriod,
               bool frequencyDomain,
               math::Real initialFrequency,
               math::Real finalFrequency,
               math::Real frequencyStep,
               bool logFrequencySweep,
               bool usingTransferFunction,
               std::string transferFunctionFile) {
    timeDomain_ = timeDomain;
    initialTime_ = initialTime;
    finalTime_ = finalTime;
    samplingPeriod_ = samplingPeriod;
    frequencyDomain_ = frequencyDomain;
    initialFrequency_ = initialFrequency;
    finalFrequency_ = finalFrequency;
    frequencyStep_ = frequencyStep;
    logFrequencySweep_ = logFrequencySweep;
    usingTransferFunction_ = usingTransferFunction;
    if (usingTransferFunction_) {
        transferFunctionFile_ = transferFunctionFile;
    }
}

Domain::Domain(const Domain& rhs) {
    timeDomain_ = rhs.timeDomain_;
    initialTime_ = rhs.initialTime_;
    finalTime_ = rhs.finalTime_;
    samplingPeriod_ = rhs.samplingPeriod_;
    frequencyDomain_ = rhs.frequencyDomain_;
    initialFrequency_ = rhs.initialFrequency_;
    finalFrequency_ = rhs.finalFrequency_;
    frequencyStep_ = rhs.frequencyStep_;
    logFrequencySweep_ = rhs.logFrequencySweep_;
    usingTransferFunction_ = rhs.usingTransferFunction_;
    transferFunctionFile_ = rhs.transferFunctionFile_;
}

Domain::~Domain() {

}

Domain& Domain::operator =(const Domain& rhs) {
    if (this == &rhs) {
        return *this;
    }
    timeDomain_ = rhs.timeDomain_;
    initialTime_ = rhs.initialTime_;
    finalTime_ = rhs.finalTime_;
    samplingPeriod_ = rhs.samplingPeriod_;
    frequencyDomain_ = rhs.frequencyDomain_;
    initialFrequency_ = rhs.initialFrequency_;
    finalFrequency_ = rhs.finalFrequency_;
    frequencyStep_ = rhs.frequencyStep_;
    logFrequencySweep_ = rhs.logFrequencySweep_;
    usingTransferFunction_ = rhs.usingTransferFunction_;
    transferFunctionFile_ = rhs.transferFunctionFile_;
    return *this;
}

bool
Domain::operator ==(const Domain& rhs) const {
    bool isSame = true;
    isSame &= timeDomain_ == rhs.timeDomain_;
    isSame &= initialTime_ == rhs.initialTime_;
    isSame &= finalTime_ == rhs.finalTime_;
    isSame &= samplingPeriod_ == rhs.samplingPeriod_;
    isSame &= frequencyDomain_ == rhs.frequencyDomain_;
    isSame &= initialFrequency_ == rhs.initialFrequency_;
    isSame &= finalFrequency_ == rhs.finalFrequency_;
    isSame &= frequencyStep_ == rhs.frequencyStep_;
    isSame &= logFrequencySweep_ == rhs.logFrequencySweep_;
    isSame &= usingTransferFunction_ == rhs.usingTransferFunction_;
    isSame &= transferFunctionFile_ == rhs.transferFunctionFile_;
    return isSame;
}

bool Domain::isTimeDomain() const {
    return timeDomain_;
}

math::Real Domain::getInitialTime() const {
    return initialTime_;
}

math::Real Domain::getFinalTime() const {
    return finalTime_;
}

bool Domain::isFrequencyDomain() const {
    return frequencyDomain_;
}

math::Real Domain::getInitialFrequency() const {
    return initialFrequency_;
}

math::Real Domain::getFinalFrequency() const {
    return finalFrequency_;
}

math::Real Domain::getSamplingPeriod() const {
    return samplingPeriod_;
}

math::Real Domain::getFrequencyStep() const {
    return frequencyStep_;
}

bool Domain::isLogFrequencySweep() const {
    return logFrequencySweep_;
}

bool Domain::isUsingTransferFunction() const {
    return usingTransferFunction_;
}

const std::string& Domain::getTransferFunctionFile() const {
    return transferFunctionFile_;
}

Domain::Type Domain::getDomainType() const {
    if (timeDomain_ && frequencyDomain_ && usingTransferFunction_) {
        return ALL;
    } else if (timeDomain_ && frequencyDomain_ && !usingTransferFunction_) {
        return TIFR;
    } else if (timeDomain_ && !frequencyDomain_ && usingTransferFunction_) {
        return TITR;
    } else if (timeDomain_ && !frequencyDomain_ && !usingTransferFunction_) {
        return TIME;
    } else if (!timeDomain_ && frequencyDomain_ && usingTransferFunction_) {
        return FRTR;
    } else if (!timeDomain_ && frequencyDomain_ && !usingTransferFunction_) {
        return FREQ;
    } else if (!timeDomain_ && !frequencyDomain_ && usingTransferFunction_) {
        return TRAN;
    } else {
        return NONE;
    }
}

void Domain::setFinalTime(const math::Real finalTime) {
    finalTime_ = finalTime;
}

void Domain::setSamplingPeriod(const math::Real samplingPeriod) {
    samplingPeriod_ = samplingPeriod;
}

} 
} 
