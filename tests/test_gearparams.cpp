// test_gearparams.cpp
// Unit test for checking gear parameter calculations

#include "../src/geometry/GearParams.h"
#include <cmath>
#include <iostream>

// Struct to store reference data obtained from CAD sketch
struct CADRefernceData {
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
  CADRefernceData() = default; 
  CADRefernceData(double PA, double addendum, double dedendum, double pinionPA,
                  double pinionFaceConeOffset, double pinionRootConeOffset,
                  double pinionAddendum, double pinionDedendum)
      : PA(PA), addendum(addendum), dedendum(dedendum), pinionPA(pinionPA),
        pinionFaceConeOffset(pinionFaceConeOffset),
        pinionRootConeOffset(pinionRootConeOffset),
        pinionAddendum(pinionAddendum), pinionDedendum(pinionDedendum) {}
};

bool checkValue(const std::string &name, double calculated, double expected,
                double tolerance) {
  if (fabsl(calculated - expected) >= tolerance) {
    std::cout << " The value of " << name << " is " << calculated
              << " instead of " << expected << std::endl;
    return false;
  }
  return true;
}
void printTestResult(const std::string &testName, bool passed) {
  std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;
}

bool testParams(BevelGear gear, BevelGear pinion, CADRefernceData CAD) {
  bool passed = true;
  double tol = 0.01;
  // Gear checks
  passed &= checkValue("Gear PA", gear.PA, CAD.PA, tol);
  passed &= checkValue("Gear addendum", gear.addendum, CAD.addendum, tol);
  passed &= checkValue("Gear dedendum", gear.dedendum, CAD.dedendum, tol);

  // Pinion checks
  passed &= checkValue("Pinion PA", pinion.PA, CAD.pinionPA, tol);
  passed &= checkValue("Pinion faceConeOffset", pinion.faceConeOffset,
                       CAD.pinionFaceConeOffset, tol);
  passed &= checkValue("Pinion rootConeOffset", pinion.rootConeOffset,
                       CAD.pinionRootConeOffset, tol);
  passed &= checkValue("Pinion addendum", pinion.addendum, CAD.pinionAddendum, tol);
  passed &= checkValue("Pinion dedendum", pinion.dedendum, CAD.pinionDedendum, tol);

  return passed;
}

int main() {
  BevelGearPair pair =
      BevelGearPair(11, 9, 5.593454, 0.1, 1.5, 90, 60, 40, 0, -0.74, 19.43, 60);
  BevelGear gear = pair.makeGear();
  BevelGear pinion = pair.makePinion();
  CADRefernceData CAD = CADRefernceData(); // defaults as the values for 
                                           // the 9-11 gear. Sketch is in
                                           // CAD/Gear_1.FCStd
  bool passed = testParams(gear, pinion, CAD);
  std::string testName =
      "Gear parameter calcualtion checks for 9-11 ratio test";
  printTestResult(testName, passed);
  // BevelGearPair pair2;
  // BevelGear gear2 = pair2.makeGear();
  // BevelGear pinion2 = pair2.makePinion();
  // passed = testParams(gear, pinion, CAD);
  // std::string test2Name =
  //     "Gear parameter calcualtion checks for 9-14 ratio test";
  // printTestResult(test2Name, passed);
}
