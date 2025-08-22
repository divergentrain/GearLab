#pragma once

#include <cmath>

// Spiral tooth generation functions
// Explicit values assigned for stable serialization & external processing
enum spiralFunction {
  Logarithmic = 0,
  CircularCut = 1,
  Involute = 2,
};

// Utilities for spiralFunction handling
struct spiralFunctionUtils {
  static const char *toString(spiralFunction f) {
    switch (f) {
    case spiralFunction::Logarithmic:
      return "Logarithmic";
    case spiralFunction::CircularCut:
      return "CircularCut";
    case spiralFunction::Involute:
      return "Involute";
    default:
      return "unknown";
    }
  }
};

// Bevel gear struct applicable to be gears and pinions
struct BevelGear {
  // Linear parameters are in mm by default
  // Angular parameters are in deg by default
  int numTeeth;
  double pitchConeAngle;
  double faceConeAngle;
  double rootConeAngle;
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
  double pressureAngle;
  double spiralAngle;
  spiralFunction spiralType;

  BevelGear(int numTeeth, double PA, double FA, double RA, double module,
            double faceConeOffset, double rootConeOffset,
            double innerConeDistance, double outerConeDistance,
            double pitchConeDistance, double addendum, double dedendum,
            double backlash, double shaftAngle, double pressureAngle,
            double spiralAngle, spiralFunction spiralType)
      : numTeeth(numTeeth), pitchConeAngle(PA), faceConeAngle(FA),
        rootConeAngle(RA), module(module), faceConeOffset(faceConeOffset),
        rootConeOffset(rootConeOffset), innerConeDistance(innerConeDistance),
        outerConeDistance(outerConeDistance),
        pitchConeDistance(pitchConeDistance), addendum(addendum),
        dedendum(dedendum), backlash(backlash), shaftAngle(shaftAngle),
        pressureAngle(pressureAngle), spiralAngle(spiralAngle),
        spiralType(spiralType) {}
};

// Bevel gear pair for initial specifications and splitting into gear & pinion
struct BevelGearPair {
  // default contructor
  BevelGearPair() = default;
  // general contructor
  BevelGearPair(int numGearTeeth, int numPinionTeeth, double module,
                double backlash, double coneClearance, double shaftAngle,
                double FA, double RA, double faceConeOffset,
                double rootConeOffset, double innerConeDistance,
                double outerConeDistance, double pressureAngle,
                double spiralAngle = 0, spiralFunction spiralType = Logarithmic)
      : numGearTeeth(numGearTeeth), numPinionTeeth(numPinionTeeth),
        module(module), backlash(backlash), coneClearance(coneClearance),
        shaftAngle(shaftAngle), faceConeAngle(FA), rootConeAngle(RA),
        faceConeOffset(faceConeOffset), rootConeOffset(rootConeOffset),
        innerConeDistance(innerConeDistance),
        outerConeDistance(outerConeDistance), pressureAngle(pressureAngle),
        spiralAngle(spiralAngle), spiralType(spiralType) {
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
  double faceConeAngle = 0; // Face cone angle (deg)
  double rootConeAngle = 0; // Root cone angle (deg)
  double faceConeOffset = 0;
  double rootConeOffset = 0;
  double innerConeDistance = 0; // Inner pitch cone distance
  double outerConeDistance = 0; // Outer pitch cone distance
  double pitchConeDistance = 0; // Pitch cone distance for crowning center
  double pressureAngle = 0;
  double spiralAngle = 0;
  spiralFunction spiralType = Logarithmic;

  // Teeth geometry parameters for pinion (derived)
  double pinionFaceConeAngle = 0;
  double pinionRootConeAngle = 0;
  double pinionFaceConeOffset = 0;
  double pinionRootConeOffset = 0;

  // Computed parameters, no preifx implies gear
  double PA = 0; // Pitch cone angle (deg)
  double pinionPitchConeAngle = 0;
  double gearPitch;
  double pinionPitch;
  double addendum;
  double dedendum;
  double pinionAddendum;
  double pinionDedendum;

  void computePA() {
    double ratio = static_cast<double>(numPinionTeeth) / numGearTeeth;
    PA = rad2deg(atan(sin(deg2rad(shaftAngle)) / ratio));
    pinionPitchConeAngle = shaftAngle - PA;
  }

  void computeDerivedValues() {
    gearPitch = module * numGearTeeth;
    pinionPitch = module * numPinionTeeth;
    pitchConeDistance = gearPitch / (2 * sin(deg2rad(PA)));
    addendum = faceConeOffset *
               (sin(deg2rad(faceConeAngle)) / cos(deg2rad(faceConeAngle - PA)));
    addendum += pitchConeDistance * tan(deg2rad(faceConeAngle - PA));
    dedendum = -rootConeOffset *
               (sin(deg2rad(rootConeAngle)) / cos(deg2rad(PA - rootConeAngle)));
    dedendum += pitchConeDistance * tan(deg2rad(PA - rootConeAngle));
    pinionRootConeAngle = shaftAngle - faceConeAngle;
    pinionFaceConeAngle = shaftAngle - rootConeAngle;
    pinionAddendum =
        dedendum - (coneClearance /
                    cos(deg2rad(pinionFaceConeAngle - pinionPitchConeAngle)));
    pinionDedendum =
        addendum + (coneClearance /
                    cos(deg2rad(pinionPitchConeAngle - pinionRootConeAngle)));
  }

  void computePinionParameters() {
    // TODO:  compute pinion parameters using gear parameters
    pinionFaceConeOffset =
        (pinionAddendum -
         pitchConeDistance *
             tan(deg2rad(pinionFaceConeAngle - pinionPitchConeAngle))) *
        (cos(deg2rad(pinionFaceConeAngle - pinionPitchConeAngle)) /
         sin(deg2rad(pinionFaceConeAngle)));
    pinionRootConeOffset =
        (-pinionDedendum +
         pitchConeDistance *
             tan(deg2rad(pinionPitchConeAngle - pinionRootConeAngle))) *
        (cos(deg2rad(pinionPitchConeAngle - pinionRootConeAngle)) /
         sin(deg2rad(pinionRootConeAngle)));
  }

  BevelGear makeGear() const {
    return BevelGear(numGearTeeth, PA, faceConeAngle, rootConeAngle, module,
                     faceConeOffset, rootConeOffset, innerConeDistance,
                     outerConeDistance, pitchConeDistance, addendum, dedendum,
                     backlash, shaftAngle, pressureAngle, spiralAngle,
                     spiralType);
  }

  BevelGear makePinion() const {
    return BevelGear(numPinionTeeth, pinionPitchConeAngle, pinionFaceConeAngle,
                     pinionRootConeAngle, module, pinionFaceConeOffset,
                     pinionRootConeOffset, innerConeDistance, outerConeDistance,
                     pitchConeDistance, pinionAddendum, pinionDedendum,
                     backlash, shaftAngle, pressureAngle, spiralAngle,
                     spiralType);
  }

  double deg2rad(double deg) { return deg * M_PI / 180; }

  double rad2deg(double rad) { return rad * 180 / M_PI; }

  bool validateParam() {
    // TODO: create validation checks for parameter
    if (numGearTeeth > numPinionTeeth && faceConeAngle > PA &&
        PA > rootConeAngle && module >= 0.0 &&
        outerConeDistance > innerConeDistance && innerConeDistance > 0 &&
        rootConeAngle > 0 && numPinionTeeth > 0 && coneClearance > 0 &&
        shaftAngle > 0)
      return true;
    else
      return false;
  }
};
