#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

#include "GearParams.h"
using namespace std;
// Angles in degree and linear distance in mm unless specified
// TODO: Check all constructors
// TODO: Create Validation functions for all structs

enum class ManufacturingMethod {
  ThreeD,             // 3D Printing
  InjectionMoulding,  // Injection Moulding
  Forging,            // Forging
  Milling             // Milling
};

struct ManufacturingHelper {
  static string toString(ManufacturingMethod method) {
    switch (method) {
      case ManufacturingMethod::ThreeD:
        return "3D Printing";
      case ManufacturingMethod::InjectionMoulding:
        return "Injection Moulding";
      case ManufacturingMethod::Forging:
        return "Forging";
      case ManufacturingMethod::Milling:
        return "Milling";
      default:
        throw std::invalid_argument("Unknown manufacturing method");
    }
  }
};

// Gear specific parameters
struct GearMacro {
  // TODO: Constructor
  double apexToThrustFace;  // Apex to thrust face i.e. mounting distance
  double stemLength;        // Length of stem from thrust face

  GearMacro(double apexToThrustFace_, double stemLength_)
      : apexToThrustFace(apexToThrustFace_), stemLength(stemLength_) {}
};

struct PinionStemMacro {
  // TODO: Constructor
  double apexToThrustFace;  // Apex to thrust face i.e. mounting distance
  double stemLength;        // Length of stem from thrust face

  PinionStemMacro(double apexToThrustFace_, double stemLength_)
      : apexToThrustFace(apexToThrustFace_), stemLength(stemLength_) {}
};

// Pinion specific parameters for spherical mounting
struct PinionSphericalMacro {
  // TODO: Constructor
  double sphericalRadius;   // Spherical radius
  double mountingDistance;  // Apex to mounting mounting
  double mountingPointDia;  // Diameter of mounting point
  double sphericalCutOff;   // Diameter at which the spherical counting surface
                            // cuts off
  double cutOffStep;        // Step height at spherical cut off

  PinionSphericalMacro(double sphericalRadius_, double mountingDistance_,
                       double mountingPointDia_, double sphericalCutOff_,
                       double cutOffStep_)
      : sphericalRadius(sphericalRadius_),
        mountingDistance(mountingDistance_),
        mountingPointDia(mountingPointDia_),
        sphericalCutOff(sphericalCutOff_),
        cutOffStep(cutOffStep_) {}
};

// Macro for variants
using MacroData =
    std::variant<GearMacro, PinionSphericalMacro, PinionStemMacro>;

struct BevelMacroGeometry {
  // TODO: Constructor
  double outerDia;             // Outer Diameter
  double innerDia;             // Inner Diameter
  double apexToTop;            // Apex to top surface of gear
  double ribDia;               // Diameter of rib
  double apexToWeb;            // Apex to top of web
  double minimumRibThickness;  // Minimum thickness of rib and web for
                               // validation checks of through hardening
  double draftAngle;           // Angle for draft of vertical surfaces
  ManufacturingMethod
      process;    // Manufacturing process for process specific parameters
  bool isPinion;  // For pinion specific parameters
  optional<bool> isSpherical;  // To check if pinion is spherical if is Pinion
  MacroData data;

  // Gear constructor
  BevelMacroGeometry(GearMacro g, ManufacturingMethod proc, double outerDia_,
                     double innerDia_, double apexToTop_, double ribDia_,
                     double apexToWeb_, double minRibThick_, double draftAngle_)
      : outerDia(outerDia_),
        innerDia(innerDia_),
        apexToTop(apexToTop_),
        ribDia(ribDia_),
        apexToWeb(apexToWeb_),
        minimumRibThickness(minRibThick_),
        draftAngle(draftAngle_),
        process(proc),
        isPinion(false),
        isSpherical(nullopt),
        data(std::move(g)) {}

  // Pinion spherical constructor
  BevelMacroGeometry(PinionSphericalMacro p, ManufacturingMethod proc,
                     double outerDia_, double innerDia_, double apexToTop_,
                     double ribDia_, double apexToWeb_, double minRibThick_,
                     double draftAngle_)
      : outerDia(outerDia_),
        innerDia(innerDia_),
        apexToTop(apexToTop_),
        ribDia(ribDia_),
        apexToWeb(apexToWeb_),
        minimumRibThickness(minRibThick_),
        draftAngle(draftAngle_),
        process(proc),
        isPinion(true),
        isSpherical(true),
        data(std::move(p)) {}

  // Pinion stem constructor
  BevelMacroGeometry(PinionStemMacro p, ManufacturingMethod proc,
                     double outerDia_, double innerDia_, double apexToTop_,
                     double ribDia_, double apexToWeb_, double minRibThick_,
                     double draftAngle_)
      : outerDia(outerDia_),
        innerDia(innerDia_),
        apexToTop(apexToTop_),
        ribDia(ribDia_),
        apexToWeb(apexToWeb_),
        minimumRibThickness(minRibThick_),
        draftAngle(draftAngle_),
        process(proc),
        isPinion(true),
        isSpherical(false),
        data(std::move(p)) {}

  // Helper functions for accessing variants
  GearMacro& asGear() {
    if (isPinion) {
      throw std::invalid_argument(
          "This BevelMacroGeometry is a pinion, not a gear!");
    }
    return std::get<GearMacro>(data);
  }

  PinionSphericalMacro& asSpherical() {
    if (!isPinion) {
      throw std::invalid_argument(
          "This BevelMacroGeometry is a gear, not a pinion!");
    }
    if (!isSpherical.has_value() || !isSpherical.value()) {
      throw std::invalid_argument(
          "This BevelMacroGeometry is a stem pinion, not a spherical pinion!");
    }
    return std::get<PinionSphericalMacro>(data);
  }

  PinionStemMacro& asStem() {
    if (!isPinion) {
      throw std::invalid_argument(
          "This BevelMacroGeometry is a gear, not a pinion!");
    }
    if (!isSpherical.has_value() || isSpherical.value()) {
      throw std::invalid_argument(
          "This BevelMacroGeometry is a spherical pinion, not a stem pinion!");
    }
    return std::get<PinionStemMacro>(data);
  }
};

struct BevelPairMacro {
  // TODO: Constructor

  // TODO: Update macro geometry initialisation after creating constructors
  BevelMacroGeometry gearGeom;
  BevelMacroGeometry pinionGeom;
  BevelGear gear;
  BevelGear pinion;

  BevelPairMacro(BevelMacroGeometry gGeom, BevelMacroGeometry pGeom,
                 const BevelGear& g, const BevelGear& p)
      : gearGeom(std::move(gGeom)),
        pinionGeom(std::move(pGeom)),
        gear(g),
        pinion(p) {}

  // TODO: Validation functions
  bool validateParams() { return true; };
  bool clearanceCheck() { return true; };
  bool coneDistanceCheck() { return true; }
};
