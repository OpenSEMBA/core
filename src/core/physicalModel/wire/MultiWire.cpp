#include "MultiWire.h"
#include <stdexcept>
#include <format>
 

namespace semba::physicalModel::wire {
    std::vector<std::size_t> getMatrixRowDimensions(const std::vector< std::vector<math::Real> >& matrix);
    std::string getRowSizesFormat(const std::vector<std::size_t>& rowSizes);
    std::string getMatrixErrorMessage(const std::string& matrixName, std::size_t numberOfRows, const std::string& rowSizesString);

    MultiWire::MultiWire(
        const Id id,
        const std::string& name,
        const std::vector<math::Real>& resistanceVector,
        const std::vector< std::vector<math::Real> >& inductanceMatrix,
        const std::vector< std::vector<math::Real> >& capacitanceMatrix) : 
        Identifiable<Id>(id), 
        PhysicalModel(name)
    {
        assertDimensions(resistanceVector, inductanceMatrix, capacitanceMatrix);

        this->resistanceVector = resistanceVector;
        this->inductanceMatrix = inductanceMatrix;
        this->capacitanceMatrix = capacitanceMatrix;
    }

    MultiWire::MultiWire(const MultiWire& rhs) :
        Identifiable<Id>(rhs.getId()),
        PhysicalModel(rhs.getName())
    {
        resistanceVector = rhs.resistanceVector;
        inductanceMatrix = rhs.inductanceMatrix;
        capacitanceMatrix = rhs.capacitanceMatrix;
    }

    void MultiWire::assertDimensions(
        const std::vector<math::Real>& resistanceVector,
        const std::vector< std::vector<math::Real> >& inductanceMatrix,
        const std::vector< std::vector<math::Real> >& capacitanceMatrix) {
        std::vector<size_t> inductanceRowSizes = getMatrixRowDimensions(inductanceMatrix);
        std::vector<size_t> capacitanceRowSizes = getMatrixRowDimensions(capacitanceMatrix);

        if (resistanceVector.size() != inductanceMatrix.size() ||
            resistanceVector.size() != capacitanceMatrix.size() ||
            inductanceMatrix.size() != inductanceRowSizes[0] ||
            inductanceRowSizes.size() > 1 ||
            capacitanceMatrix.size() != capacitanceRowSizes[0] ||
            capacitanceRowSizes.size() > 1) {

            std::string stringOfInductanceSizes = getRowSizesFormat(inductanceRowSizes);
            std::string stringOfCapacitanceSizes = getRowSizesFormat(capacitanceRowSizes);

            std::string stringError = "All vectors and matrixes must share the same size!\n"
                "Resistance Vector size: " + std::to_string(resistanceVector.size()) + "\n";
            stringError += getMatrixErrorMessage("Inductance", inductanceMatrix.size(), stringOfInductanceSizes) + "\n";
            stringError += getMatrixErrorMessage("Capacitance", capacitanceMatrix.size(), stringOfCapacitanceSizes);

            throw std::length_error(stringError);
        }

    }

    std::string getRowSizesFormat(const std::vector<std::size_t> & rowSizes) {
        if (rowSizes.size() == 1)
            return std::to_string(rowSizes[0]);

        std::string stringOSizes;

        stringOSizes += "[";
        for (int i = 0; i < rowSizes.size(); ++i) {
            if (i != 0)
                stringOSizes += ", ";

            stringOSizes += std::to_string(rowSizes[i]);
        }
        stringOSizes += "]";
        
        return stringOSizes;
    }

    std::vector<std::size_t> getMatrixRowDimensions(const std::vector< std::vector<math::Real> >& matrix) {
        std::vector<size_t> rowSizes;
        auto numberOfRows = matrix.size();
        bool allHaveTheSameSize = true;

        for (auto& specificRow : matrix) {
            rowSizes.push_back(specificRow.size());
            if (numberOfRows != specificRow.size())
                allHaveTheSameSize = false;
        }

        if (allHaveTheSameSize) {
            rowSizes.resize(1);
        }

        return rowSizes;
    }

    std::string getMatrixErrorMessage(const std::string& matrixName, std::size_t numberOfRows, const std::string& rowSizesString) {
        return matrixName + " Matrix sizes:\n"
            "- rows: " + std::to_string(numberOfRows) + "\n"
            "- size of each row : " + rowSizesString;
    }

    const std::vector<math::Real>& MultiWire::getResistanceVector() const {
        return resistanceVector;
    }
    const std::vector< std::vector<math::Real> >& MultiWire::getInductanceMatrix() const {
        return inductanceMatrix;
    }
    const std::vector< std::vector<math::Real> >& MultiWire::getCapacitanceMatrix() const {
        return capacitanceMatrix;
    }
}