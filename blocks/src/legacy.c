#include "headers.h"

/*void camera_render_world(struct camera* _camera, struct world* _world) {

    struct v3dabs cords = { 0, 0, 0 };

    double _sin_theta = sin(_camera->direction_sph3d.theta);
    double _cos_theta = cos(_camera->direction_sph3d.theta);
    double _sin_phi = sin(_camera->direction_sph3d.phi);
    double _cos_phi = cos(_camera->direction_sph3d.phi);

    struct v3d direction = {
                -_camera->direction_sph3d.radius * _sin_theta * _cos_phi,
                _camera->direction_sph3d.radius * _cos_theta * _cos_phi,
                _camera->direction_sph3d.radius * _sin_phi
    };

    struct v3d xtangentcomp = { _cos_theta * _camera->pixel_size, _sin_theta * _camera->pixel_size, 0 };

    struct v3d ytangentcomp = { _sin_theta * _sin_phi * _camera->pixel_size, -_cos_theta * _sin_phi * _camera->pixel_size, _cos_phi * _camera->pixel_size };

    for (unsigned int i = 0; i < _camera->height; i++) {
        for (unsigned int j = 0; j < _camera->width; j++) {

            
            struct v3d xtangent = { _cos_theta, _sin_theta, 0 };

            struct v3d ytangent = { _sin_theta * _sin_phi, -_cos_theta * _sin_phi, _cos_phi };

            struct v3d xpixelpointer = scale_v3d(&xtangent, _camera->pixel_size * (0.5 + j - 0.5 * _camera->width));
            struct v3d ypixelpointer = scale_v3d(&ytangent, _camera->pixel_size * (0.5 + i - 0.5 * _camera->height));

            struct v3d pixelpointer = {
                xpixelpointer.x + ypixelpointer.x + direction.x,
                xpixelpointer.y + ypixelpointer.y + direction.y,
                xpixelpointer.z + ypixelpointer.z + direction.z
            };
            

            //all in one

            struct v3d pixelpointer = {
                xtangentcomp.x * (0.5 + j - 0.5 * _camera->width) + ytangentcomp.x * (0.5 + i - 0.5 * _camera->height) + direction.x,
                xtangentcomp.y * (0.5 + j - 0.5 * _camera->width) + ytangentcomp.y * (0.5 + i - 0.5 * _camera->height) + direction.y,
                ytangentcomp.z * (0.5 + i - 0.5 * _camera->height) + direction.z
            };


            struct v3d time = { 0, 0, 0 };

            struct v3d inttime = { 0, 0, 0 };

            cords.x = -6;

            while (-6 <= cords.x && cords.x <= 4) {
                cords.x++;
                time.x = (cords.x - _camera->position.x) / pixelpointer.x;
                if (time.x > 0) {
                    cords.y = floor_double(pixelpointer.y * time.x + _camera->position.y);
                    cords.z = floor_double(pixelpointer.z * time.x + _camera->position.z);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.x == 0 || time.x < inttime.x)) { inttime.x = time.x; }
                        cords.x--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.x == 0 || time.x < inttime.x)) { inttime.x = time.x; }
                        cords.x++;
                    }

                }

            }

            cords.y = -6;

            while (-6 <= cords.y && cords.y <= 4) {
                cords.y++;
                time.y = (cords.y - _camera->position.y) / pixelpointer.y;
                if (time.y > 0) {
                    cords.x = floor_double(pixelpointer.x * time.y + _camera->position.x);
                    cords.z = floor_double(pixelpointer.z * time.y + _camera->position.z);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.y == 0 || time.y < inttime.y)) { inttime.y = time.y; }
                        cords.y--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.y == 0 || time.y < inttime.y)) { inttime.y = time.y; }
                        cords.y++;
                    }

                }

            }

            cords.z = -6;

            while (-6 <= cords.z && cords.z <= 4) {
                cords.z++;
                time.z = (cords.z - _camera->position.z) / pixelpointer.z;
                if (time.z > 0) {
                    cords.x = floor_double(pixelpointer.x * time.z + _camera->position.x);
                    cords.y = floor_double(pixelpointer.y * time.z + _camera->position.y);

                    if (-5 <= cords.x && cords.x <= 4 && -5 <= cords.y && cords.y <= 4 && -5 <= cords.z && cords.z <= 4) {
                        if (get_block_ptr(_world, &cords)->id && (inttime.z == 0 || time.z < inttime.z)) { inttime.z = time.z; }
                        cords.z--;
                        if (get_block_ptr(_world, &cords)->id && (inttime.z == 0 || time.z < inttime.z)) { inttime.z = time.z; }
                        cords.z++;
                    }

                }

            }

            _camera->pixels[i * _camera->width + j] = 0x88b2ff;

            double min = -1;

            if (inttime.x > 0) {
                _camera->pixels[i * _camera->width + j] = 0xff0000;
                min = inttime.x;
            }

            if (inttime.y > 0) {
                if (inttime.x == 0 || inttime.y < min) {
                    _camera->pixels[i * _camera->width + j] = 0xff00;
                    min = inttime.y;
                }

            }

            if (inttime.z > 0) {
                if ((inttime.x == 0 && inttime.y == 0) || inttime.z < min) {
                    _camera->pixels[i * _camera->width + j] = 0xff;
                }
            }

        }

    }

    printf("frame_done ");
}

void camera_render_triangle(struct camera* _camera, struct triangle* _tri) {

    struct v2d cords[3];
    int cords_length = 0;
    struct v2d out_cords[3];
    int out_cords_length = 0;

    for (int i = 0; i < 3; i++) {
        struct v3d piercer = { _tri->p[i].x - _camera->position.x, _tri->p[i].y - _camera->position.y, _tri->p[i].z - _camera->position.z };
        double factor = _camera->direction_sph3d.radius * _camera->direction_sph3d.radius / (_camera->direction_v3d.x * piercer.x + _camera->direction_v3d.y * piercer.y + _camera->direction_v3d.z * piercer.z);

        struct v3d comb = { piercer.x * factor - _camera->direction_v3d.x, piercer.y * factor - _camera->direction_v3d.y, piercer.z * factor - _camera->direction_v3d.z };
        struct v2d cordinates = {
            (comb.x * _camera->xpixelpointer.x + comb.y * _camera->xpixelpointer.y + comb.z * _camera->xpixelpointer.z) / _camera->pixel_size + _camera->width / 2,
            (comb.x * _camera->ypixelpointer.x + comb.y * _camera->ypixelpointer.y + comb.z * _camera->ypixelpointer.z) / _camera->pixel_size + _camera->height / 2 };

        if (factor < 0) {
            out_cords[out_cords_length] = cordinates;
            out_cords_length++;
        }

        else if (factor > 0) {
            cords[cords_length] = cordinates;
            cords_length++;
        }

    }

    if (out_cords_length == 0) {
        struct v2dabs min = { (int)floor(min_3double(cords[0].x, cords[1].x, cords[2].x)), (int)floor(min_3double(cords[0].y, cords[1].y, cords[2].y)) };
        struct v2dabs max = { (int)floor(max_3double(cords[0].x, cords[1].x, cords[2].x)), (int)floor(max_3double(cords[0].y, cords[1].y, cords[2].y)) };

        min.x = (min.x > (signed int)_camera->width - 1 ? (signed int)_camera->width - 1 : (min.x < 0 ? 0 : min.x));
        min.y = (min.y > (signed int)_camera->height - 1 ? (signed int)_camera->height - 1 : (min.y < 0 ? 0 : min.y));
        max.x = (max.x > (signed int)_camera->width - 1 ? (signed int)_camera->width - 1 : (max.x < 0 ? 0 : max.x));
        max.y = (max.y > (signed int)_camera->height - 1 ? (signed int)_camera->height - 1 : (max.y < 0 ? 0 : max.y));


        for (int i = min.x; i <= max.x; i++) {
            for (int j = min.y; j <= max.y; j++) {
                double v0x = cords[2].x - cords[0].x;
                double v0y = cords[2].y - cords[0].y;
                double v1x = cords[1].x - cords[0].x;
                double v1y = cords[1].y - cords[0].y;
                double v2x = i + 0.5 - cords[0].x;
                double v2y = j + 0.5 - cords[0].y;

                // Compute dot products
                double dot00 = v0x * v0x + v0y * v0y;
                double dot01 = v0x * v1x + v0y * v1y;
                double dot02 = v0x * v2x + v0y * v2y;
                double dot11 = v1x * v1x + v1y * v1y;
                double dot12 = v1x * v2x + v1y * v2y;

                // Compute barycentric coordinates
                double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
                double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
                double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

                // Check if point is in triangle
                if ((u >= 0) && (v >= 0) && (u + v <= 1)) _camera->pixels[i + _camera->width * j] = _tri->color;


            }
        }
    }

    else if (out_cords_length == 1) {


    }

    else if (out_cords_length == 2) {

    }

    return;
};

*/