#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		GLint ambient = glGetUniformLocation( p_glProgram, "ambient" );
		glProgramUniform3fv( p_glProgram, ambient, 1, glm::value_ptr( _material._ambient ) );

		GLint diffuse = glGetUniformLocation( p_glProgram, "diffuse" );
		glProgramUniform3fv( p_glProgram, diffuse, 1, glm::value_ptr( _material._diffuse ) );

		GLint specular = glGetUniformLocation( p_glProgram, "specular" );
		glProgramUniform3fv( p_glProgram, specular, 1, glm::value_ptr( _material._specular ) );

		GLint shininess = glGetUniformLocation( p_glProgram, "shininess" );
		glProgramUniform1f( p_glProgram, shininess, _material._shininess );

		glUseProgram( p_glProgram );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	void TriangleMesh::_setupGL()
	{		
		// vbo

		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );

		// vao 

		glCreateVertexArrays( 1, &_vao );
		glBindVertexArray( _vao );

		//position

		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _position ) );
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vertex ) );
		glVertexArrayAttribBinding( _vao, 0, 0 );

		// normal

		glEnableVertexArrayAttrib( _vao, 1 );
		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _normal ) );
		glVertexArrayVertexBuffer( _vao, 1, _vbo, 0, sizeof( Vertex ) );
		glVertexArrayAttribBinding( _vao, 1, 0 );

		// texCoords

		glEnableVertexArrayAttrib( _vao, 2 );
		glVertexArrayAttribFormat( _vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _texCoords ) );
		glVertexArrayVertexBuffer( _vao, 2, _vbo, 0, sizeof( Vertex ) );
		glVertexArrayAttribBinding( _vao, 2, 0 );

		// tangent

		glEnableVertexArrayAttrib( _vao, 3 );
		glVertexArrayAttribFormat( _vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _tangent ) );
		glVertexArrayVertexBuffer( _vao, 3, _vbo, 0, sizeof( Vertex ) );
		glVertexArrayAttribBinding( _vao, 3, 0 );

		// bitangent

		glEnableVertexArrayAttrib( _vao, 4 );
		glVertexArrayAttribFormat( _vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _bitangent ) );
		glVertexArrayVertexBuffer( _vao, 4, _vbo, 0, sizeof( Vertex ) );
		glVertexArrayAttribBinding( _vao, 4, 0 );

		// ebo

		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW );

		glVertexArrayElementBuffer( _vao, _ebo );

	}
} // namespace M3D_ISICG
