#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() {
		glDeleteShader( vertShader );
		glDeleteShader( fragShader );
		glDeleteProgram( program );

		glDeleteBuffers( 1, &vbo );

		glDisableVertexArrayAttrib( vao, 0 );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork1::init()
	{

		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		std::cout << "Done!" << std::endl;

		// vertex shader
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
		const GLchar * vSrc = vertexShaderStr.c_str();
		glShaderSource( vertShader, 1, &vSrc, NULL );
		glCompileShader( vertShader );

		// fragment shader
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
		GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
		const GLchar * fSrc = fragmentShaderStr.c_str();
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
		GLuint program = glCreateProgram();
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



		//vecteur
		//p1 -0.5,0.5     p2 0.5,0.5     p3 0.5,-0.5

		Vec2f v1( -0.5f, 0.5f );
		Vec2f v2( 0.5f, 0.5f );
		Vec2f v3( 0.5f, - 0.5f);

		std::vector<Vec2f> vectors = { v1, v2, v3 };

		//vbo

		glCreateBuffers(1, &vbo);
		glNamedBufferData( 
			vbo, 
			vectors.size() * sizeof( Vec2f ), 
			vectors.data(), 
			GL_STATIC_DRAW 
		);

		//vao

		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );


		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );

		glVertexArrayAttribBinding( vao, 0, 0 );

		glUseProgram( program );

		return true;
	}


	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{ 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray( vao );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
