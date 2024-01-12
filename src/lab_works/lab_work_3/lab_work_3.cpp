#include "lab_work_3.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <common/camera.hpp>
#include <iostream>
#include <utils/random.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";


	LabWork3::~LabWork3() {
		glDeleteShader( vertShader );
		glDeleteShader( fragShader );
		glDeleteProgram( program );

		glDeleteBuffers( 1, &_cube.vbo );
		glDeleteBuffers( 1, &_cube.vboColor );
		glDeleteBuffers( 1, &_cube.ebo );

		glDisableVertexArrayAttrib( _cube.vao, 0 );
		glDeleteVertexArrays( 1, &_cube.vao );
	}

	bool LabWork3::init()
	{

		std::cout << "Initializing lab work 3..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		std::cout << "Done!" << std::endl;

		// vertex shader
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw3.vert" );
		GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
		const GLchar * vSrc = vertexShaderStr.c_str();
		glShaderSource( vertShader, 1, &vSrc, NULL );
		glCompileShader( vertShader );

		// fragment shader
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw3.frag" );
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

		glEnable( GL_DEPTH_TEST );


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

		_createCube();

		glUseProgram( program );
		_initCamera();

		return true;
	}


	void LabWork3::animate( const float p_deltaTime ) 
	{
		
		_cube.transformation = glm::rotate( 
			_cube.transformation, 
			glm::radians( p_deltaTime), 
			glm::vec3( 0.f, 1.f, 1.f ) );

		glProgramUniformMatrix4fv( program, modelLoc, 1,
			false, glm::value_ptr( _cube.transformation ) );
			
	}

	void LabWork3::render()
	{ 
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( _cube.vao );
		glDrawElements( GL_TRIANGLES, _cube.sommetsInd.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void LabWork3::_updateViewMatrix()
	{ 
		viewLoc = glGetUniformLocation( program, "view" );
		glProgramUniformMatrix4fv( program, viewLoc, 1, 
			false, glm::value_ptr( _camera._computeViewMatrix() ) );
	}

	void LabWork3::_updateProjectionMatrix()
	{
		projectionLoc = glGetUniformLocation( program, "projection" );
		glProgramUniformMatrix4fv( program, projectionLoc, 1,
			false, glm::value_ptr( _camera._computeProjectionMatrix() ) );
	}

	void LabWork3::_initCamera()
	{ 
		_updateViewMatrix();
		_updateProjectionMatrix();
		_camera.setPosition( Vec3f( 0.f, 1.f, 3.f ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
	}


	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork3::_createCube() {

		//sommets cube

		Vec3f s1( -0.5f, -0.5f, 0.5f );
		Vec3f s2( 0.5f, -0.5f, 0.5f);
		Vec3f s3( 0.5f, 0.5f, 0.5f);
		Vec3f s4( -0.5f, 0.5f, 0.5f );

		Vec3f s5( -0.5f, -0.5f, -0.5f );
		Vec3f s6( 0.5f, -0.5f, -0.5f );
		Vec3f s7( 0.5f, 0.5f, -0.5f );
		Vec3f s8( -0.5f, 0.5f, -0.5f );

		std::vector<Vec3f> sommets = { s1, s2, s3, s4, s5, s6, s7, s8 };

		_cube.sommetsPos = sommets;

		//couleurs sommets cube

		Vec3f c1 = getRandomVec3f();
		Vec3f c2 = getRandomVec3f();
		Vec3f c3 = getRandomVec3f();
		Vec3f c4 = getRandomVec3f();
		Vec3f c5 = getRandomVec3f();
		Vec3f c6 = getRandomVec3f();
		Vec3f c7 = getRandomVec3f();
		Vec3f c8 = getRandomVec3f();

		std::vector<Vec3f> couleurs = { c1, c2, c3, c4, c5, c6, c7, c8 };

		_cube.sommetsCol = couleurs;

		//indices sommets cube

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			1, 5, 6,
			6, 2, 1,

			0, 3, 7,
			7, 4, 0,

			2, 6, 7,
			7, 3, 2,

			0, 1, 5,
			5, 4, 0
		};

		

		_cube.sommetsInd = indices;

		// matrice

		_cube.transformation = glm::mat4( 1.f );

		glm::mat4 matriceHomothetie = glm::scale( _cube.transformation, glm::vec3( 0.8f, 0.8f, 0.8f ) );

		_cube.transformation = _cube.transformation * matriceHomothetie;

		modelLoc = glGetUniformLocation( program, "model" );
		glProgramUniformMatrix4fv( program, modelLoc, 1, false, glm::value_ptr( _cube.transformation ) );


		// vbo

		glCreateBuffers( 1, &_cube.vbo );
		glNamedBufferData( 
			_cube.vbo, _cube.sommetsPos.size() * sizeof( Vec3f ),
			_cube.sommetsPos.data(), GL_STATIC_DRAW );

		// vbo couleurs

		glCreateBuffers( 1, &_cube.vboColor );
		glNamedBufferData( 
			_cube.vboColor, _cube.sommetsCol.size() * sizeof( Vec3f ),
			_cube.sommetsCol.data(), GL_STATIC_DRAW );

		// vao

		glCreateVertexArrays( 1, &_cube.vao );
		glBindVertexArray( _cube.vao );

		glVertexArrayVertexBuffer( _cube.vao, 0, _cube.vbo, 0, sizeof( Vec3f ) );
		glEnableVertexArrayAttrib( _cube.vao, 0 );
		glVertexArrayAttribFormat( _cube.vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribBinding( _cube.vao, 0, 0 );

		// couleurs						   
		glVertexArrayVertexBuffer( _cube.vao, 1, _cube.vboColor, 0, sizeof( Vec3f ) );
		glEnableVertexArrayAttrib( _cube.vao, 1 );
		glVertexArrayAttribFormat( _cube.vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribBinding( _cube.vao, 1, 1 );

		// ebo

		glCreateBuffers( 1, &_cube.ebo );
		glNamedBufferData( _cube.ebo, _cube.sommetsInd.size() * sizeof(Vec3f), _cube.sommetsInd.data(), GL_STATIC_DRAW );

		glVertexArrayElementBuffer( _cube.vao, _cube.ebo );

		glBindVertexArray(0);


	}

} // namespace M3D_ISICG
