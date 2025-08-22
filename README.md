![Work in Progress](https://img.shields.io/badge/status-work--in--progress-orange?style=for-the-badge&logo=github)
# GearLab

**GearLab** is a C++ pipeline for **bevel gear design**, from parameter
definition to 3D modeling and export for simulation or manufacturing.  

It focuses on **spherical involute profiles**, making it suitable for bevel
gears produced via **3D printing, forging, injection moulding, or milling**,
where spherical involute profiles can be manufactured efficiently.  

With GearLab, you can handle the **entire design workflow** — macro geometry,
micro geometry, contact analysis, and visualization — all in one place.  

## Features (Planned)

- **Spherical Involute Gear Design** – Define gear parameters using accurate spherical involute geometry.  
- **Macro & Micro Geometry** – Generate full macro geometry and detailed micro geometry for bevel gears.  
- **Contact Analysis** – Perform LTCA (Local Tooth Contact Analysis) for accurate load and contact predictions.  
- **Export** – Export gear models for FEM or manufacturing (FreeCAD, Gmsh, CalculiX).  
- **3D Visualization** – Inspect gear models in real-time with OpenGL rendering.  

## Directory structure

```typogram
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
│   ├── CAD/                          // CAD reference files for tests
│   ├── images/                       // CAD reference files for tests
└── /tests
    └── test_gearparams.cpp           // Unit tests for gear parameter calculations

```

## Build Instructions

Coming soon...

## Contributing

Currently, contributions are not accepted. However, you can follow the project for updates.
Contributions will be accepted in the future at alpha stage.

## License

GearLab is licensed under the MIT License. See the [LICENSE](LICENSE) file for details
