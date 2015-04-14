#ifndef ___BOX_H___
#define ___BOX_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class Box : public VisualObject
{
	public:

		/**
		 * Constructor for the Pyramid class. Created a visual object in the shape of a pyramid. Vertex
		 * positions are expressed relative to an origin that is located in the center of the pyramid.
		 * @param OpenGLApp - reference to the OpenGLApplicationBase object that holds the projection
		 *        and viewing transformations
		 * @param w - width of the base of the box. Default is 1 unit.
		 */
		Box(OpenGLApplicationBase * OpenGLApp, GLfloat w = 1.0f, vec4 color = vec4(0,1,0,1));

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
		
		void initTopBottomBack();
		void initSides();
		
		int numberOfIndices, numberOfIndicesSides; // number of indices to be used during indexed rendering.

		//GLfloat height; // height for the pyramid
		GLfloat width; // width for the pyramid
	
		// location in the shader of the projection matrix uniform variable
		GLuint projectionLocation; 

		// location in the shader of the model/view matrix uniform variable
		//GLuint modelViewLocation;
		Material material;

		GLuint modelLocation;
		GLuint solidColorLocation;
		GLuint textureObject;

		GLuint vertexArrayObjectFBTB;
		GLuint vertexArrayObjectSides;

		vec4 objectColor;

};

#endif // ___BOX_H___