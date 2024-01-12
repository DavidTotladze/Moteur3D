#include "lab_work_4.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <common/camera.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace M3D_ISICG
{
	const std::string LabWork4::_shaderFolder = "src/lab_works/lab_work_4/shaders/";

	LabWork4::~LabWork4()
	{
		glDeleteShader( vertShader );
		glDeleteShader( fragShader );
		glDeleteProgram( program );

		bunny.cleanGL();
	}

	bool LabWork4::init()
	{
		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		std::cout << "Done!" << std::endl;

		// vertex shader
		const std::string vertexShaderStr = readFile( _shaderFolder + "mesh.vert" );
		GLuint			  vertShader	  = glCreateShader( GL_VERTEX_SHADER );
		const GLchar *	  vSrc			  = vertexShaderStr.c_str();
		glShaderSource( vertShader, 1, &vSrc, NULL );
		glCompileShader( vertShader );

		// fragment shader
		const std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );
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

		glEnable( GL_DEPTH_TEST );

		glUseProgram( program );

		bunny.load("bunny", "data/models/bunny.obj");

		return true;
	}

	void LabWork4::animate( const float p_deltaTime ) {
	}

	void LabWork4::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// MVP matrice
		MVP = _camera.getProjectionMatrix() * _camera.getViewMatrix() * bunny._transformation;
		MVPLoc = glGetUniformLocation( program, "uMVPMatrix" );
		glProgramUniformMatrix4fv( program, MVPLoc, 1, GL_FALSE, glm::value_ptr( MVP ) );

		// normal matrice
		normal = glm::transpose( glm::inverse( _camera.getViewMatrix() * bunny._transformation ) );
		normalLoc = glGetUniformLocation( program, "uNormalMatrix" );
		glProgramUniformMatrix4fv( program, normalLoc, 1, GL_FALSE, glm::value_ptr( normal ) );

		bunny.render( program );

	}
	void LabWork4::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

	void LabWork4::_initCamera()
	{
		_camera.setPosition( Vec3f( 0.f, 1.f, 3.f ) );
		_camera.setScreenSize( _windowWidth, _windowHeight );
	}

	void LabWork4::_updateViewMatrix()
	{
		viewLoc = glGetUniformLocation( program, "view" );
		glProgramUniformMatrix4fv( program, viewLoc, 1, false, glm::value_ptr( _camera._computeViewMatrix() ) );
	}

	void LabWork4::_updateProjectionMatrix()
	{
		projectionLoc = glGetUniformLocation( program, "projection" );
		glProgramUniformMatrix4fv(program, projectionLoc, 1, false, glm::value_ptr( _camera._computeProjectionMatrix() ) );
	}

	void LabWork4::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				view	= _camera.getViewMatrix();
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				view	= _camera.getViewMatrix();
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				view	= _camera.getViewMatrix();
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				view	= _camera.getViewMatrix();
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				view	= _camera.getViewMatrix();
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				view	= _camera.getViewMatrix();
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
} // namespace M3D_ISICG