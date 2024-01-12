#ifndef __LAB_WORK_4_HPP__
#define __LAB_WORK_4_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/models/triangle_mesh.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork4 : public BaseLabWork
	{
	  public:
		LabWork4() : BaseLabWork() {}
		~LabWork4();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _initCamera();

		void _updateViewMatrix();
		void _updateProjectionMatrix();

	  private:
		// ================ Scene data.
		TriangleMeshModel bunny;

		const std::string vertexShaderStr;	
		const std::string fragmentShaderStr; 

		//camera
		Camera _camera;
		float  _cameraSpeed		  = 0.1f;
		float  _cameraSensitivity = 0.1f;

		// ================ GL data.
		GLuint vertShader;
		GLuint fragShader;
		GLuint program;

		// variables uniformes
		Mat4f view;
		Mat4f MVP;
		Mat4f normal;

		GLint viewLoc;
		GLint projectionLoc;
		GLint MVPLoc;
		GLint normalLoc;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_4_HPP__
