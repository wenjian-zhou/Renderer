#ifndef COLOR_H
#define COLOR_H

#include "Vector.hpp"

#include <iostream>

<<<<<<< HEAD
<<<<<<< HEAD
void writeColor(std::ostream &out , Color pixel_color, int spp)
=======
void write_color(std::ostream &out , Color pixel_color)
>>>>>>> parent of 51fe3f2 (add ray, sphere, hit class)
=======
void writeColor(std::ostream &out , Color pixel_color)
>>>>>>> parent of 70be2a2 (update camera and material)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif