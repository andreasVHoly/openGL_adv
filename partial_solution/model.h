/**
  Simple STL Model loader and drawer
  (C) Benjamin Hugo, Department of Computer Science UCT
  Model answer for the basic OpenGL prac CSC3020H

  Copying this answer for the basic opengl practical constitutes
  plagiarism. Your mark will be voided if found guilty
*/
#pragma once
#include <QFile>
#include <QString>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <cstring>
#include "glheaders.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace bhugo {
    template <typename T>
    struct tripple{
        float x,y,z;
    };
    template <typename T>
    struct triangle {
        tripple<T> v1;
        tripple<T> v2;
        tripple<T> v3;
    };

    class model
    {
    private:
        std::vector<triangle<float> > data; //stl requires IEEE 754 float32 for the triangle verticies
        std::vector<tripple<float> > normals; //stl requires IEEE 754 float32 for the normals
        uint32_t number_of_triangles;
        GLuint vbo,vao;
        bool can_render;
    public:
        model();
        model(const QString & name);
        model(const model & model) = delete;
        model& operator=(const model & model) = delete;
        model(model && rhs):data(std::move(rhs.data)),
                            normals(std::move(rhs.normals)),
                            number_of_triangles(rhs.number_of_triangles),
                            vbo(rhs.vbo),vao(rhs.vao),can_render(rhs.can_render){}
        model& operator=(const model && rhs){
            if (this != &rhs){
                data = std::move(rhs.data);
                normals = std::move(rhs.normals);
                number_of_triangles = rhs.number_of_triangles;
                vbo = rhs.vbo;
                vao = rhs.vao;
                can_render = rhs.can_render;
            }
            return *this;
        }

        void draw(GLuint shader_program);
    };
}
