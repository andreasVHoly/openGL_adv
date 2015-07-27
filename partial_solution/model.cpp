/**
  Simple STL Model loader and drawer
  (C) Benjamin Hugo, Department of Computer Science UCT
  Model answer for the basic OpenGL prac CSC3020H

  Copying this answer for the basic opengl practical constitutes
  plagiarism. Your mark will be voided if found guilty
*/
#include "model.h"
namespace bhugo{
    model::model():can_render(false){}
    model::model(const QString & name):can_render(true)
    {
        //setup a vao per model loaded
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        /*
        The following format has to be read according to wikipedia:

        UINT8[80] – Header
        UINT32 –  Number of triangles

        foreach triangle
            REAL32[3] – Normal vector
            REAL32[3] – Vertex 1
            REAL32[3] – Vertex 2
            REAL32[3] – Vertex 3
            UINT16 – Attribute byte count
        end
        */

        QFile f(name);

        if (!f.open(QIODevice::ReadOnly)){
            throw std::runtime_error("Failed to load model file");
        }
        uint8_t hdr[80];
        f.read((char*)hdr,80*sizeof(uint8_t));
        f.read((char*)&number_of_triangles,1*sizeof(uint32_t));
        std::vector<char> tmp_buffer(number_of_triangles*50); //each facet must be 50 bytes long
        f.read(&tmp_buffer[0],50 * number_of_triangles);
        f.close();
        //Now we need to discard the normals and attribute fields
        this->data.resize(number_of_triangles);
        this->normals.resize(number_of_triangles);
        for (size_t i = 0; i < number_of_triangles; ++i){
            memcpy(&(this->normals[i]), &(tmp_buffer[i*50]),sizeof(tripple<float>) * 1);
            memcpy(&(this->data[i]), &(tmp_buffer[i*50+sizeof(tripple<float>)]),sizeof(triangle<float>) * 1);
        }
        //one vbo per model
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER, number_of_triangles * (sizeof( triangle<float> ) + sizeof( tripple<float> )),nullptr,GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangle<float>) * number_of_triangles,(float*) &(this->data[0]));
        glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangle<float>) * number_of_triangles,sizeof(tripple<float>) * number_of_triangles, (float*) &(this->normals[0]));
    }

    void model::draw(GLuint shader_program){
        if (can_render){
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            #define BUFFER_OFFSET(i) ((void*)(i))
            GLuint vPosition = glGetAttribLocation(shader_program, "vertex");
            glEnableVertexAttribArray(vPosition);
            glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

            GLuint vNormal = glGetAttribLocation(shader_program, "normal");
            glEnableVertexAttribArray(vNormal);
            glVertexAttribPointer(vNormal,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(bhugo::triangle<float>) * number_of_triangles));
            glDrawArrays( GL_TRIANGLES, 0, number_of_triangles * 3 );
        }
    }
}
