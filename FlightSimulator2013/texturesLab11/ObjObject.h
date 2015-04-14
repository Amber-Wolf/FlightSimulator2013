#ifndef ___OBJOBJECT_H___
#define ___OBJOBJECT_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

#include <iostream>
#include <fstream>


class ObjObject : public VisualObject
{
	public:

		/**
		 * Constructor for the Pyramid class. Created a visual object in the shape of a pyramid. Vertex
		 * positions are expressed relative to an origin that is located in the center of the pyramid.
		 * @param OpenGLApp - reference to the OpenGLApplicationBase object that holds the projection
		 *        and viewing transformations
		 * @param w - width of the base of the box. Default is 1 unit.
		 */
		ObjObject(OpenGLApplicationBase * OpenGLApp, string modelName, string texName, vec4 color = vec4(1.0f,1.0f,1.0f,1.0f));

		/**
		 * Builds shader program and initializes vertex array object that will be used to render the 
		 * the object.
		 */
		virtual void initialize();

		/*
		 * Renders the object using the visual object using the vertex array object that was created by
		 * the initialize method.
		 */
		virtual void draw();

	protected:

		/**
		 * Builds the shader program.
		 */
		void buildShaderProgram();
		
		void initObj();
		
		int numberOfIndices; // number of indices to be used during indexed rendering.


		//friend vec3 findVec3(char* str, int end);
		//friend vec2 findVec2(char* str, int end);
		
		
		// location in the shader of the projection matrix uniform variable
		GLuint projectionLocation; 

		// location in the shader of the model/view matrix uniform variable
		//GLuint modelViewLocation;
		Material material;

		GLuint modelLocation;
		GLuint solidColorLocation;
		GLuint textureObject;

		GLuint vertexArrayObject;

		vec4 objectColor;

		string modelName;
		string textureName;
};

#endif // ___OBJOBJECT_H___