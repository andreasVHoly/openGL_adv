#include "glwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <stdexcept>

#define VERT_SHADER ":/simple.vert"
#define FRAG_SHADER ":/simple.frag"
#define BUNNY ":/bunny.stl"
#define PORSCHE ":/porsche.stl"
GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent ),
      m_vertexBuffer( QOpenGLBuffer::VertexBuffer )
{
}

void GLWidget::initializeGL()
{
    // Resolve OpenGL functions
    glewExperimental = true;
    GLenum GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK) {
        const GLubyte* errorStr = glewGetErrorString(GlewInitResult);
        size_t size = strlen(reinterpret_cast<const char*>(errorStr));
        qDebug() << "Glew error "
                 << QString::fromUtf8(
                        reinterpret_cast<const char*>(errorStr), size);
    }

    // get context opengl-version
    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // Set the clear color to black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    qDebug() << "Attempting to load the Porsche Model " << PORSCHE;
    porsche = std::move(bhugo::model(PORSCHE));
    qDebug() << "Attempting to load the Stanford Bunny " << BUNNY;
    bunny = std::move(bhugo::model(BUNNY));
    //bunny.renormalize();
    qDebug() << "Attempting vertex shader load from " << VERT_SHADER;
    qDebug() << "Attempting fragment shader load from " << FRAG_SHADER;
    // Prepare a complete shader program...
    if ( !prepareShaderProgram( VERT_SHADER, FRAG_SHADER) )
        std::runtime_error("Failed to load shader");
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shader.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }
    //setup the shader to use GLSL variable "vertex" as input to the currently loaded vbo
    m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 3 );
    m_shader.enableAttributeArray( "vertex" );
    //set initial color:
    glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"),0.0f,1.0f,0.0f,1.0f);
    glm::vec3 cam_pos(0.0f,0.5f,1.5f);
    //setup a fixed camera (http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)
    CameraMatrix = glm::lookAt(cam_pos,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    ProjectionMatrix = glm::perspective(
        45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f       // Far clipping plane. Keep as little as possible.
    );
    {
        GLuint MatrixID = glGetUniformLocation(m_shader.programId(), "View");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &CameraMatrix[0][0]);
    }
    {
        GLuint MatrixID = glGetUniformLocation(m_shader.programId(), "Projection");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
    }
}

void GLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //draw a bunny
    {
        glm::vec3 rot_vec = (current_rot_axis == ROT_AXIS::X) ? glm::vec3(1.0f,0.0f,0.0f) : (current_rot_axis == ROT_AXIS::Y) ? glm::vec3(0.0f,1.0f,0.0f) : glm::vec3(0.0f,0.0f,1.0f);
        glm::mat4 ModelMatrix = glm::rotate(glm::translate(glm::mat4(1.0f),current_translation),rot,rot_vec);
        GLuint MatrixID = glGetUniformLocation(m_shader.programId(), "Model");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"),0.0f,1.0f,0.0f,1.0f);
        this->bunny.draw(m_shader.programId());
    }
    //draw a car (stationary)
    {
        glm::mat4 ModelMatrix = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.0f,0.0f)),-(float)M_PI/2.0f,glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(0.005f,0.005f,0.005f));
        GLuint MatrixID = glGetUniformLocation(m_shader.programId(), "Model");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"),1.0f,0.0f,0.0f,1.0f);
        this->porsche.draw(m_shader.programId());
    }

}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;
        case Qt::Key_Down:
            current_translation += glm::vec3(0,0,0.05f);
            this->repaint();
            break;
        case Qt::Key_Up:
            current_translation -= glm::vec3(0,0,0.05f);
            this->repaint();
            break;
        case Qt::Key_Left:
            current_translation -= glm::vec3(0.05f,0,0);
            this->repaint();
            break;
        case Qt::Key_Right:
            current_translation += glm::vec3(0.05f,0,0);
            this->repaint();
            break;
        case Qt::Key_R:
            rot = 0; //reset rotation
            this->current_rot_axis = (ROT_AXIS)((this->current_rot_axis + 1) % 3);
            qDebug() << "Switched rotation axis to axis " << this->current_rot_axis;
            this->repaint();
            break;
        case Qt::Key_Plus:
            this->rot += M_PI / 180.0 * 5.0; // rotate by 5 degrees
            this->repaint();
            break;
        case Qt::Key_Minus:
            this->rot -= M_PI / 180.0 * 5.0; // rotate by 5 degrees
            this->repaint();
            break;
        default:
            QGLWidget::keyPressEvent( e );
    }
}

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& fragmentShaderPath )
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QOpenGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QOpenGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();
    return result;
}
