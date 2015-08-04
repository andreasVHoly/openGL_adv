#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "glheaders.h" // Must be included before QT opengl headers
#include <QGLWidget>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "model.h"
#include <cmath>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
enum ROT_AXIS {X,Y,Z};
class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget( const QGLFormat& format, QWidget* parent = 0 );

    struct Light{
        glm::vec2 origin;
        float radius;
        float rotationSpeed;
        float xPos, yPos, zPos;
        float angle;

        float getX(float z){
            return sqrt(radius * radius - ((z - origin.y) * (z - origin.y))) + origin.x;
        }

        float getZ(float x){
            return sqrt(radius * radius - ((x - origin.x) * (x - origin.x))) + origin.y;
        }


//        float xPos;
//        float yPos;
//        float zPos;
//        float angle;
//        float radius;
//        float smoothing;


    };

    void setPosition();
    void rotate();

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void keyPressEvent( QKeyEvent* e );

private:
    bool prepareShaderProgram( const QString& vertexShaderPath,
                               const QString& fragmentShaderPath );
    QOpenGLShaderProgram m_shader;
    QOpenGLBuffer m_vertexBuffer;
    bhugo::model bunny;
    bhugo::model porsche;
    glm::mat4 CameraMatrix;
    glm::mat4 ProjectionMatrix;
    float rot;
    ROT_AXIS current_rot_axis;
    glm::vec3 current_translation;
    void recalibrate();

    Light orbitLight;




};

#endif // GLWIDGET_H
