#include "imgui.h"
#include "lab_work_2.hpp"
#include "utils/read_file.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteShader( vertShader );
		glDeleteShader( fragShader );
		glDeleteProgram( program );

		glDeleteBuffers( 1, &vbo );
		glDeleteBuffers( 1, &vboColor );
		glDeleteBuffers( 1, &ebo );

		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		std::cout << "Done!" << std::endl;

		// vertex shader
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw2.vert" );
		GLuint			  vertShader	  = glCreateShader( GL_VERTEX_SHADER );
		const GLchar *	  vSrc			  = vertexShaderStr.c_str();
		glShaderSource( vertShader, 1, &vSrc, NULL );
		glCompileShader( vertShader );

		// fragment shader
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );
		GLuint			  fragShader		= glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar *	  fSrc				= fragmentShaderStr.c_str();
		glShaderSource( fragShader, 1, &fSrc, NULL );
		glCompileShader( fragShader );

		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertShader, sizeof( log ), NULL, log );
			glDeleteShader( vertShader );
			glDeleteShader( fragShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;
		}

		glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragShader, sizeof( log ), NULL, log );
			glDeleteShader( vertShader );
			glDeleteShader( fragShader );
			std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			return false;
		}

		// program
		program = glCreateProgram();
		glAttachShader( program, vertShader );
		glAttachShader( program, fragShader );
		glLinkProgram( program );

		// Check if link is ok.
		GLint linked;
		glGetProgramiv( program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		// triangle 1
		// p1 -0.5,0.5     p2 0.5,0.5     p3 0.5,-0.5
		// triangle 2
		// p1 -0.5,0.5     p2 -0.5, -0.5     p3 0.5,-0.5

		Vec2f v1( -0.5f, 0.5f );
		Vec2f v2( 0.5f, 0.5f );
		Vec2f v3( 0.5f, -0.5f );
		Vec2f v4( -0.5f, -0.5f );

		std::vector<Vec2f> vectors = { v1, v2, v3, v4 };

		int array[ 6 ] = { 0, 1, 2, 0, 2, 3 };

		//couleurs

		Vec4f red( 1.f, 0.f, 0.f, 1.f );
		Vec4f green( 0.f, 1.f, 0.f, 1.f );
		Vec4f blue( 0.f, 0.f, 1.f, 1.f );
		Vec4f magenta( 1.f, 0.f, 1.f, 1.f );

		std::vector<Vec4f> colors = { red, green, blue, magenta };

		// vbo

		glCreateBuffers( 1, &vbo );
		glNamedBufferData( vbo, vectors.size() * sizeof( Vec2f ), vectors.data(), GL_STATIC_DRAW );

		// vbo couleurs

		glCreateBuffers( 1, &vboColor );
		glNamedBufferData( vboColor, colors.size() * sizeof( Vec4f ), colors.data(), GL_STATIC_DRAW );

		// ebo

		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, sizeof( array ), array, GL_STATIC_DRAW );

		// vao

		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );

		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );

		glEnableVertexArrayAttrib( vao, 1 ); //couleur
		glVertexArrayVertexBuffer( vao, 1, vboColor, 0, sizeof( Vec4f ) ); //couleur

		glVertexArrayAttribBinding( vao, 0, 0 );

		glVertexArrayElementBuffer( vao, ebo );

		glUseProgram( program );

		return true;
	}

	void LabWork2::animate( const float p_deltaTime )
	{ 
		loc = glGetUniformLocation( program, "uTranslationX" );
		glProgramUniform1f( program, loc, glm::sin( _time ) );
		_time += p_deltaTime;
	}

	void LabWork2::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( vao );
		glDrawElements( GL_TRIANGLES, sizeof( array ), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );
		if (ImGui::SliderFloat("Luminosity", &luminosity, 0.f, 1.f)) {
			loc = glGetUniformLocation( program, "luminosity" );
			glProgramUniform1f( program, loc, luminosity );
		}
		if (ImGui::ColorEdit3( "Background color", glm::value_ptr( _bgColor ))){
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}
		ImGui::End();
	}

} // namespace M3D_ISICG