#pragma once

#include "math_definitions.h"
#include "material.h"
#include "scene.h"
#include "config.h"
#include <array>
#include <cmath>

namespace path
{
    // NOTE: in is w.r.t. rays from the camera
    struct BaseBRDF
    {
        virtual Ray sample(const Ray& in, const HitResult& hit)
        {
            // Samples a hemisphere uniformly by default
            double u1 = dist(gen);
            double u2 = dist(gen);

            double A = std::sqrt(1 - u1 * u1);
            double phi = 2 * PBR_PI * u2;

            Vec w = hit.normal;
            Vec u = normalize(cross(w, Vec{ 0, 1, 0 }));
            Vec v = normalize(cross(u, w));

            return {
                hit.point,
                normalize(u * A * std::cos(phi) + v * A * std::sin(phi) + w * u1)
            };
        }

        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out)
        {
            // Returns hit.material.color by default
            return hit.material->color;
        }

        BaseBRDF() : dist(0.0, 1.0)
        {
            gen = std::mt19937(rd());
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

    /** Lambertian Diffuse BRDF */
    struct DiffuseBRDF : public BaseBRDF
    {
        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out) override
        {
            // return normalize(out.direction);
            auto diff = cosv(out.direction, hit.normal);
            return hit.material->color * diff * 2; // 2pi for monte carlo, 1/pi for Lambertian BRDF
        }
    };

    struct SpecularBRDF : public BaseBRDF
    {
        virtual Ray sample(const Ray& in, const HitResult& hit) override
        {
            Ray refl;
            refl.direction = reflect(in.direction, hit.normal);
            refl.origin = hit.point;
            return refl;
        }

        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out) override
        {
            return PBR_COLOR_WHITE;
        }
    };

    struct OrenNayarBRDF : public BaseBRDF
    {
        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out) override
        {
            double sigma = dist(gen);
            double s = sigma * sigma;
            double A = 1.0 - (s / (2.0 * (s + 0.33)));
            double B = 0.45 * s / (s + 0.09);

            double costhetai = cosv((in.direction)*-1, hit.normal);
            double costhetao = cosv(out.direction, hit.normal);
            double sinalpha, tanbeta;
            if (costhetai > costhetao) {
                sinalpha = sqrt(1 - costhetao * costhetao);
                tanbeta = sqrt(1 - costhetai * costhetai) / costhetai;
            }
            else {
                sinalpha = sqrt(1 - costhetai * costhetai);
                tanbeta = sqrt(1 - costhetao * costhetao) / costhetao;
            }
            /*Ray normal_in, normal_in_h, normal_out, normal_out_h;

            normal_in.origin = hit.point;
            normal_in_h.origin = hit.point;

            normal_out_h.origin = hit.point;
            normal_out.origin = hit.point;
            */
            Vec normal_in = hit.normal * cosv((in.direction)*-1, hit.normal);
            Vec normal_in_h = (in.direction)*-1 - normalize(normal_in);

            Vec normal_out = hit.normal * cosv(hit.normal, out.direction);
            Vec normal_out_h = out.direction - normalize(normal_out);

            double maxcos;
            if (cosv(normalize(normal_out_h), normalize(normal_in_h)) > 0.0) 
            {
                maxcos = cosv(normalize(normal_out_h), normalize(normal_in_h));
            }
            else {
                maxcos = 0;
            }
            auto diff = A + B * maxcos * tanbeta * sinalpha;

            return hit.material->color * diff * 2;
        }
        OrenNayarBRDF() : dist(0.0, 1.0) 
        {
            gen = std::mt19937(rd());
        }
    
    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };
}


