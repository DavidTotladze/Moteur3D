#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "common/camera.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork3 : public BaseLabWork
	{
		struct Mesh
		{
			std::vector<Vec3f> sommetsPos;
			std::vector<Vec3f> sommetsCol;
			std::vector <unsigned int> sommetsInd;

			Mat4f transformation;

			GLuint vbo;
			GLuint vboColor;
			GLuint vao;
			GLuint ebo;
		};

	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		void _createCube();

		void _updateViewMatrix();
		void _updateProjectionMatrix();

		void _initCamera();

	  private:
		// ================ Scene data.
		std::vector<Vec2f> vectors;
		int	array[ 6 ];

		Mesh _cube;

		Camera _camera;
		float  _cameraSpeed		  = 0.1f;
		float  _cameraSensitivity = 0.1f;

		// ================

		// ================ GL data.
		GLuint vertShader;
		GLuint fragShader;
		GLuint program;

		//variables uniformes
		GLint modelLoc;
		GLint viewLoc;
		GLint projectionLoc;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;

	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_3_HPP__
