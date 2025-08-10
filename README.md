# GearLab

GearLab is a complete pipeline for bevel gear design, with tools for defining
gear parameters, generating 3D models, and exporting them for simulation or
manufacturing.

GearLab is a C++ project that focuses on the design and analysis of bevel
gears, specifically using spherical involute profiles. Use cases include 3D
printing, forging, injection moulding and milling of bevel gears where
spherical involute profiles can be manufactured efficiently.

GearLab is designed to be a comprehensive solution for engineers and designers
working with bevel gears, offering both macro and micro geometry generation,
contact analysis, and visualization capabilities.

GearLab is built using C++ and OpenGL, leveraging modern graphics techniques
for rendering and analysis. The project is structured to facilitate easy
extension and integration with other open source tools such as FreeCAD, CalculiX and gmsh.

## Features (Planned)

- **Spherical Involute Gear Design**: Define gear parameters using spherical involute profiles.
- **Macro and Micro Geometry**: Generate both macro and micro geometry for bevel gears.
- **Contact Analysis**: Perform contact analysis using the LTCA (Local Tooth Contact Analysis) method.
- **Export Capabilities**: Export gear models to formats suitable for FEM analysis or manufacturing.
- **Visualization**: Render 3D models using OpenGL for visual inspection.

## Directory structure

```cpp
GearLab
├── CMakeLists.txt
├── /src
│   ├── main.cpp
│   ├── geometry/
│   │   ├── GearParams.h              // Gear parameters definition
│   │   ├── BevelGear.cpp             // Bevel gear macro geometry
│   │   ├── SphericalInvolute.cpp/.h  // Spherical involute profile generation
│   ├── microgeometry/
│   │   ├── CrowningParams.h          // Micro gear parameters
│   │   ├── MicroGeometry.cpp/.h      // Micro geometry generation
│   │   ├── Mesh.cpp                  // Mesh generation for LTCA
│   ├── graphics/
│   │   ├── Renderer.cpp/.h
│   │   ├── Camera.cpp/.h
│   ├── ltca/
│   │   ├── ContactModel.cpp/.h       // LTCA contact model
│   │   ├── ContactAnalysis.cpp/.h    // LTCA analysis implementation
│   ├── export/
│   │   ├── Exporter.cpp/.h           // Base exporter
│   │   ├── FreeCADExporter.cpp/.h
│   │   ├── GmshExporter.cpp/.h
│   │   ├── CalculixWriter.cpp/.h
├── /shaders
├── /assets
└── /tests
    ├── CAD/                          // CAD reference files for tests
    └── test_gearparams.cpp           // Unit tests for gear parameter calculations

```

## Build Instructions

Coming soon...

## Contributing

Currently, contributions are not accepted. However, you can follow the project for updates.
Contributions will be accepted in the future at alpha stage.

## License

GearLab is licensed under the MIT License. See the [LICENSE](LICENSE) file for details
