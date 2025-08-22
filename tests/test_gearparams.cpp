// test_gearparams.cpp
// Unit test for checking gear parameter calculations

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include "../src/geometry/GearParams.h"

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"

// Struct to store reference data obtained from CAD sketch
struct CADReferenceData {
  // Gear
  double PA = 50.7106;
  double addendum = 6.5016;
  double dedendum = 8.00199;

  // Pinion
  double pinionPA = 90 - PA;
  double pinionFaceConeOffset = -1.33718;
  double pinionRootConeOffset = -3.0;
  double pinionAddendum = 6.4754;
  double pinionDedendum = 8.02201;
  CADReferenceData() = default;
  CADReferenceData(double PA, double addendum, double dedendum, double pinionPA,
                   double pinionFaceConeOffset, double pinionRootConeOffset,
                   double pinionAddendum, double pinionDedendum)
      : PA(PA),
        addendum(addendum),
        dedendum(dedendum),
        pinionPA(pinionPA),
        pinionFaceConeOffset(pinionFaceConeOffset),
        pinionRootConeOffset(pinionRootConeOffset),
        pinionAddendum(pinionAddendum),
        pinionDedendum(pinionDedendum) {}
};

struct GearTestCase {
  BevelGearPair pair;
  CADReferenceData CAD;
  std::string testName;
};

bool checkValue(const std::string& name, double calculated, double expected,
                double tolerance) {
  if (std::fabs(calculated - expected) >= tolerance) {
    std::cout << COLOR_RED << " ❌ The value of " << name << " is "
              << calculated << " instead of " << expected << COLOR_RESET
              << std::endl;
    return false;
  } else {
    std::cout << COLOR_GREEN << " ✅ " << name << " matches expected value."
              << COLOR_RESET << std::endl;
  }
  return true;
}

void printTestResult(const std::string& testName, bool passed) {
  if (passed) {
    std::cout << COLOR_GREEN << "[PASS] " << COLOR_RESET << testName
              << std::endl;
  } else {
    std::cout << COLOR_RED << "[FAIL] " << COLOR_RESET << testName << std::endl;
  }
}

bool testParams(const std::string& testName, const BevelGear& gear,
                const BevelGear& pinion, const CADReferenceData& CAD) {
  bool passed = true;
  constexpr double tol = 0.01;
  std::cout << COLOR_YELLOW << "Running test: " << COLOR_RESET << testName
            << std::endl;
  // Gear checks
  passed &= checkValue("Gear pitchConeAngle", gear.pitchConeAngle, CAD.PA, tol);
  passed &= checkValue("Gear addendum", gear.addendum, CAD.addendum, tol);
  passed &= checkValue("Gear dedendum", gear.dedendum, CAD.dedendum, tol);

  // Pinion checks
  passed &= checkValue("Pinion pitchConeAngle", pinion.pitchConeAngle,
                       CAD.pinionPA, tol);
  passed &= checkValue("Pinion faceConeOffset", pinion.faceConeOffset,
                       CAD.pinionFaceConeOffset, tol);
  passed &= checkValue("Pinion rootConeOffset", pinion.rootConeOffset,
                       CAD.pinionRootConeOffset, tol);
  passed &=
      checkValue("Pinion addendum", pinion.addendum, CAD.pinionAddendum, tol);
  passed &=
      checkValue("Pinion dedendum", pinion.dedendum, CAD.pinionDedendum, tol);

  return passed;
}

bool testValidations(const std::string& testName, const BevelGearPair& pair) {
  bool passed = true;
  std::cout << COLOR_YELLOW << "Validating test input data: " << COLOR_RESET
            << testName << std::endl;

  auto check = [&](const std::string& name, bool condition) {
    if (!condition) {
      std::cout << COLOR_RED << " ❌ Validation failed: " << name << COLOR_RESET
                << std::endl;
      passed = false;
    } else {
      std::cout << COLOR_GREEN << " ✅ Validation passed: " << name
                << COLOR_RESET << std::endl;
    }
  };

  check("Tooth counts", pair.validateToothCounts());
  check("Angles", pair.validateAngles());
  check("Distances", pair.validateDistances());
  check("Overall parameters", pair.validateParam());

  return passed;
}

int main() {
  std::vector<GearTestCase> testCases = {
      {BevelGearPair(11, 9, 5.593454, 0.1, 1.5, 90, 60, 40, 0, -0.74, 19.43, 60,
                     20),
       CADReferenceData(),  // defaults as the values for the 9-11 gear
                            // assets/CAD/Gear_1.FCStd
       "Gear parameter calculation checks for 9-11 ratio test"},
      {BevelGearPair(14, 9, 4.77651, 0.1, 1.5, 90, 65, 45, 1.2, 0, 24.0405, 60,
                     20),
       CADReferenceData(57.265, 6.49665, 8.640975, 32.73522627,
                        -2.12132034355964, -6.12272, 7.10593991512259, 8.01044),
       // assets/CAD/Gear_2.FCStd
       "Gear parameter calculation checks for 9-14 ratio test"}};

  bool allPassed = true;

  for (const auto& tc : testCases) {
    BevelGear gear = tc.pair.makeGear();
    BevelGear pinion = tc.pair.makePinion();

    bool validationPassed = testValidations(tc.testName, tc.pair);
    bool paramPassed = testParams(tc.testName, gear, pinion, tc.CAD);

    bool passed = paramPassed && validationPassed;
    printTestResult(tc.testName, passed);
    allPassed &= passed;
  }

  std::string allTestName = "All gear parameter calculation tests";
  printTestResult(allTestName, allPassed);

  if (!allPassed)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
