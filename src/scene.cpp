#include "scene.h"
#include "brdfs.h"

///////////////////////////////////////////////////////////////////////////////
// Scene description.
///////////////////////////////////////////////////////////////////////////////

// NOTE: We assume the following conventions
//   X-axis: Right
//   Y-axis: Up
//   Z-axis: Back
// Note that X cross Y = Z, making this a right-handed system

Scene PBR_SCENE_RTWEEKEND = {
    // Red ball
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 0.1, 0.1 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 1.5, 1.0, 0.0 },   // Position
            1.0                      // Radius
        }
    },

    // Light 1
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 6.0, 6.0, 6.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 6.0, 4.5, -4.0 },   // Position
            3.0                      // Radius
        }
    },

    // Light 2
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 6.0, 6.0, 6.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { -6.0, 4.5, -4.0 },   // Position
            3.0                      // Radius
        }
    },

    // Mirror
    Actor {
        std::make_shared<Material>(
            Colorf { 1.0, 1.0, 1.0 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::SpecularBRDF
        ),
        SphereGeometry {
            Vec { -2.0, 1.5, -1.0 },   // Position
            1.5                      // Radius
        }
    },

    // Floor
    Actor {
        std::make_shared<Material>(
            Colorf { 0.3, 0.3, 0.0 }, // Color
            Colorf { 0.0, 0.0, 0.0 },  // Emission
            new path::DiffuseBRDF
        ),
        SphereGeometry {
            Vec { 0.0, -1e5, 0.0 },  // Position
            1e5                      // Radius
        }
    },

    //OrenNayar Sphere 1
    Actor {
        std::make_shared<Material>(
            Colorf { 0.1, 0.1, 1.0 },
            Colorf { 0.0, 0.0, 0.0 },
            new path::OrenNayarBRDF
        ),
        SphereGeometry {
            Vec {-0.5, 0.5, 0.0},
            0.75
        }
    },
    //OrenNayar Sphere 2
    Actor {
        std::make_shared<Material>(
            Colorf { 0.1, 1.0, 0.1 },
            Colorf { 0.0, 0.0, 0.0 },
            new path::OrenNayarBRDF
        ),
        SphereGeometry {
            Vec {0.0, 0.5, 2.0},
            0.5
        }
    }
};
