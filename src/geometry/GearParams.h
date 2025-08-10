#pragma once

#include <cmath>

struct BevelGear {
  // Linear parameters are in mm by default
  // Angular parameters are in deg by default
  int numTeeth;
  double PA;
  double FA;
  double RA;
  double module;
  double faceConeOffset;
  double rootConeOffset;
  double innerConeDistance;
  double outerConeDistance;
  double pitchConeDistance;
  double addendum;
  double dedendum;
  double backlash;
  double shaftAngle;

  BevelGear(int numTeeth, double PA, double FA, double RA, double module,
            double faceConeOffset, double rootConeOffset,
            double innerConeDistance, double outerConeDistance,
            double pitchConeDistance, double addendum, double dedendum,
            double backlash, double shaftAngle)
      : numTeeth(numTeeth), PA(PA), FA(FA), RA(RA), module(module),
        faceConeOffset(faceConeOffset), rootConeOffset(rootConeOffset),
        innerConeDistance(innerConeDistance),
        outerConeDistance(outerConeDistance),
        pitchConeDistance(pitchConeDistance), addendum(addendum),
        dedendum(dedendum), backlash(backlash), shaftAngle(shaftAngle) {}
};

struct BevelGearPair {
  // default contructor
  BevelGearPair() = default;
  // general contructor
  BevelGearPair(int numGearTeeth, int numPinionTeeth, double module,
                double backlash, double coneClearance, double shaftAngle,
                double FA, double RA, double faceConeOffset,
                double rootConeOffset, double innerConeDistance,
                double outerConeDistance)
      : numGearTeeth(numGearTeeth), numPinionTeeth(numPinionTeeth),
        module(module), backlash(backlash), coneClearance(coneClearance),
        shaftAngle(shaftAngle), FA(FA), RA(RA), faceConeOffset(faceConeOffset),
        rootConeOffset(rootConeOffset), innerConeDistance(innerConeDistance),
        outerConeDistance(outerConeDistance) {
    computePA();
    computeDerivedValues();
    computePinionParameters();
  }
  // Basic parameters
  // Linear parameters are in mm by default
  // Angular parameters are in deg by default
  int numGearTeeth = 0;
  int numPinionTeeth = 0;
  double module = 0;
  double backlash = 0; // Angular backlash (deg)
  double coneClearance =
      1.5; // Clearance between gear face <-> pinion root and vice-versa
  double shaftAngle = 90.0; // Shaft angle (deg)

  // Teeth geometry parameters for gear
  double FA = 0; // Face cone angle (deg)
  double RA = 0; // Root cone angle (deg)
  double faceConeOffset = 0;
  double rootConeOffset = 0;
  double innerConeDistance = 0; // Inner pitch cone distance
  double outerConeDistance = 0; // Outer pitch cone distance
  double pitchConeDistance = 0; // Pitch cone distance for crowning center

  // Teeth geometry parameters for pinion (derived)
  double pinionFA = 0;
  double pinionRA = 0;
  double pinionFaceConeOffset = 0;
  double pinionRootConeOffset = 0;

  // Computed parameters, no preifx implies gear
  double PA = 0; // Pitch cone angle (deg)
  double pinionPA = 0;
  double gearPitch;
  double pinionPitch;
  double addendum;
  double dedendum;
  double pinionAddendum;
  double pinionDedendum;

  void computePA() {
    double ratio = static_cast<double>(numPinionTeeth) / numGearTeeth;
    PA = rad2deg(atan(sin(deg2rad(shaftAngle)) / ratio));
    pinionPA = shaftAngle - PA;
  }

  void computeDerivedValues() {
    gearPitch = module * numGearTeeth;
    pinionPitch = module * numPinionTeeth;
    pitchConeDistance = gearPitch / (2 * sin(deg2rad(PA)));
    addendum =  faceConeOffset * (sin(deg2rad(FA)) / cos(deg2rad(FA - PA)));
    addendum += pitchConeDistance * tan(deg2rad(FA - PA));
    dedendum = - rootConeOffset * (sin(deg2rad(RA)) / cos(deg2rad(PA - RA)));
    dedendum += pitchConeDistance * tan(deg2rad(PA - RA));
    pinionRA = shaftAngle - FA;
    pinionFA = shaftAngle - RA;
    pinionAddendum = dedendum - (coneClearance / cos(deg2rad(pinionFA-pinionPA)));
    pinionDedendum = addendum + (coneClearance / cos(deg2rad(pinionPA-pinionRA)));
  }

  void computePinionParameters() {
    // TODO:  compute pinion parameters using gear parameters
    pinionFaceConeOffset =
        (pinionAddendum - pitchConeDistance * tan(deg2rad(pinionFA - pinionPA))) *
        (cos(deg2rad(pinionFA - pinionPA)) / sin(deg2rad(pinionFA)));
    pinionRootConeOffset =
        (- pinionDedendum + pitchConeDistance * tan(deg2rad(pinionPA - pinionRA))) *
        (cos(deg2rad(pinionPA - pinionRA)) / sin(deg2rad(pinionRA)));
  }

  BevelGear makeGear() const {
    return BevelGear(numGearTeeth, PA, FA, RA, module, faceConeOffset,
                     rootConeOffset, innerConeDistance, outerConeDistance,
                     pitchConeDistance, addendum, dedendum, backlash,
                     shaftAngle);
  }

  BevelGear makePinion() const {
    return BevelGear(numPinionTeeth, pinionPA, pinionFA, pinionRA, module,
                     pinionFaceConeOffset, pinionRootConeOffset,
                     innerConeDistance, outerConeDistance, pitchConeDistance,
                     pinionAddendum, pinionDedendum, backlash, shaftAngle);
  }

  double deg2rad(double deg) { return deg * M_PI / 180; }

  double rad2deg(double rad) { return rad * 180 / M_PI; }

  bool validateParam() {
    // TODO: create validation checks for parameter
    if (numGearTeeth > numPinionTeeth && FA > PA && PA > RA && module >= 0.0 &&
        outerConeDistance > innerConeDistance && innerConeDistance > 0 &&
        RA > 0 && numPinionTeeth > 0 && coneClearance > 0 && shaftAngle > 0)
      return true;
    else
      return false;
  }
};
