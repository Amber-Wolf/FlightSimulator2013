#ifndef ___SKY_H___
#define ___SKY_H___

#include "visualobject.h"
#include "LoadShaders.h"

#include "Lighting.h"


class Sky : public VisualObject
{
	public:
		Sky( OpenGLApplicationBase * OpenGLApp, float Radius = 1.0f, int slices = 16, int stacks = 16, vec4 color = vec4( 1.0f, 1.0f, 1.0f, 1.0f) );
		virtual ~Sky(void);

		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();

		void initializeSkyBody();
		void initializeSkyBottom();
		void initializeSkyTop();
		
		float radius;
		int stacks;
		int slices;

		GLuint vertexArrayObjectForBottom, vertexArrayObject, vertexArrayObjectForTop; 

		// this variable will later be used to hold the number of indices
		int bodyIndicesCount, bottomIndicesCount, topIndicesCount;

		GLuint modelLocation;
		GLuint solidColorLocation;

		// For texture mapping
		GLuint textureObject;
		
		vec4 objectColor;
		Material material;

	// Step in z and radius as stacks are drawn.
	double z0, z1;
	double r0, r1;

	float stackStep;
	float sliceStep;
};

#endif /* ___SKY_H___ */

