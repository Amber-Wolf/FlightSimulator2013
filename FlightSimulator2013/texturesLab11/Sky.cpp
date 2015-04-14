#include "Sky.h"


#include <vector>
#include <math.h>

#include "vertexStructs.h"

//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;
//

#define M_PI 3.1415926535897932384626433832795f

Sky::Sky( OpenGLApplicationBase * OpenGLApp, float radius, int slices, int stacks, vec4 color )
	:VisualObject(OpenGLApp), radius(radius), slices(slices), stacks(stacks), objectColor(color)
{ 
	bodyIndicesCount = bottomIndicesCount = topIndicesCount =0;
}


Sky::~Sky(void)
{
	glDeleteVertexArrays (1, &vertexArrayObjectForBottom);
	glDeleteVertexArrays (1, &vertexArrayObjectForTop);
}

void Sky::buildShaderProgram()
{
	if ( shaderProgram==0 )  {
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVS-phong-per-frag.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-phong-toon-per-frag.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

	//modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	//assert(modelLocation != 0xFFFFFFFF);

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	//solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	//assert(solidColorLocation != 0xFFFFFFFF);

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped"),
						 getUniformLocation( shaderProgram, "object.toon"),
						 getUniformLocation( shaderProgram, "object.sky") );// ****
	
	//material.setAmbientAndDiffuseMat( objectColor );
	material.setAmbientAndDiffuseMat( vec4( 1.0f, 1.0f, 1.0f, 1.0f ));
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );
	material.setTextureMapped( true ); ///****
	material.setToon( false );
	material.setSky( true );
}


// Construct visual object display list.
void Sky::initialize()
{
	textureObject = setupTexture( "Sky.bmp" );
	buildShaderProgram();

	stackStep = 2.0f * M_PI / (stacks*2.0f);
	sliceStep = 2.0f * M_PI / -slices;

	// Do the stacks
    z0 = 1.0;
    z1 = cos(stackStep);
    r0 = 0.0;
    r1 = sin(stackStep);

	initializeSkyTop();
	initializeSkyBody();
	initializeSkyBottom();
	
	VisualObject::initialize();

} // end initialize


// Construct visual object display list.
void Sky::initializeSkyTop()
{
	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3( 0.0f, 0.0f, 1.0f );
	vec3 vertex = vec3( 0.0f, 0.0f, radius );
	vec2 textureCoord =  getSphericalTextCoords(0.0f, vertex);
	v.push_back( pntVertexData( vertex, normal, textureCoord)); 

	indices.push_back(currentIndex++);

    for (int j = slices; j >=  0; j--) {

		vec3 n0 = vec3( cos(j * sliceStep) * r1, sin(j * sliceStep) * r1, z1 );
		vec3 v0 = vec3( cos(j * sliceStep) * r1 * radius, sin(j * sliceStep) * r1 *radius, z1* radius );

		float theta = j * sliceStep;
		vec2 textureCoord =  getSphericalTextCoords(theta, v0);

		v.push_back( pntVertexData( v0, n0, textureCoord )); 
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForTop);
	glBindVertexArray( vertexArrayObjectForTop );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,  sizeof(pntVertexData),  (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	topIndicesCount = indices.size();

	v.clear();
	indices.clear();

} // initializeSkyTop


// Construct visual object display list.
void Sky::initializeSkyBody()
{
	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;
	int i = 1;
    for ( int i=1; i < stacks-1; i++)
    {
		z0 = z1; 
		z1 = cos((i+1)*stackStep);
        r0 = r1; 
		r1 = sin((i+1)*stackStep);

        for (int j=0; j < slices; j++ )
        {
            vec3 n01 = normalize(vec3(cos(j * sliceStep) * r1, sin(j * sliceStep) * r1, z1 ));
            vec3 v0 = vec3(cos(j * sliceStep) * r1 * radius, sin(j * sliceStep) * r1 * radius, z1 * radius  );
			v.push_back( pntVertexData( v0, n01, getSphericalTextCoords(j * sliceStep, v0) )); // 0
			
			vec3 n11 = normalize(vec3(cos(j * sliceStep) * r0, sin(j * sliceStep) * r0, z0 ));
			vec3 v1 = vec3(cos(j * sliceStep) * r0 * radius, sin(j * sliceStep) * r0 * radius, z0 * radius  );
			v.push_back( pntVertexData( v1, n11, getSphericalTextCoords(j * sliceStep, v1) )); // 1

			vec3 n23 = normalize(vec3(cos((j+1) * sliceStep) * r1, sin((j+1) * sliceStep) * r1, z1 ));
			vec3 v2 = vec3(cos((j+1) * sliceStep) * r1* radius, sin((j+1) * sliceStep) * r1 * radius, z1 * radius  );
			v.push_back( pntVertexData( v2, n23, getSphericalTextCoords((j+1) * sliceStep, v2)) ); // 2

			vec3 n33 = normalize(vec3(cos((j+1)* sliceStep) * r0, sin((j+1)* sliceStep) * r0, z0 ));
            vec3 v3 = vec3(cos((j+1) * sliceStep) * r0 *radius, sin((j+1) * sliceStep) * r0 *radius, z0 * radius  );
			v.push_back( pntVertexData( v3, n33, getSphericalTextCoords((j+1) * sliceStep, v3)) ); // 3

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 3);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 3);

			currentIndex += 4;
        }
    }

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,  sizeof(pntVertexData),  (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bodyIndicesCount = indices.size();
 
	v.clear();
	indices.clear();

} // end initializeSkyBody


// Construct visual object display list.
void Sky::initializeSkyBottom()
{
	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

	z0 = z1;
    r0 = r1;

    vec3 normal = vec3( 0.0f, 0.0f, -1.0f );
	v.push_back( pntVertexData( vec3( 0.0f, 0.0f, -radius ), normal, getSphericalTextCoords(0.0f, vec3(0.0f,0.0f,-radius))) ); 

	indices.push_back(currentIndex++);

    for (int j = 0; j <= slices; j++) {

		vec3 n0 = vec3( cos(j * sliceStep) * r0, sin(j * sliceStep) * r0, z1 );
		vec3 v0 = vec3( cos(j * sliceStep) * r0 * radius, sin(j * sliceStep) * r0 *radius, z0* radius );

		v.push_back( pntVertexData( v0, n0, getSphericalTextCoords(j * sliceStep, v0)) ); 
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForBottom);
	glBindVertexArray( vertexArrayObjectForBottom );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,  sizeof(pntVertexData),  (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bottomIndicesCount = indices.size();

	v.clear();
	indices.clear();

} // end initializeSkyBottom


// Preform drawing operations
void Sky::draw()
{

	glFrontFace(GL_CW);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	//glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setShaderMaterialProperties();

	glBindTexture(GL_TEXTURE_2D, textureObject);

	//glFrontFace(GL_CW);

	// Draw top
	glBindVertexArray(vertexArrayObjectForTop);
    glDrawElements(GL_TRIANGLE_FAN, topIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw body
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, bodyIndicesCount, GL_UNSIGNED_INT, 0);
	
	// Draw bottom
	glBindVertexArray(vertexArrayObjectForBottom);
    glDrawElements(GL_TRIANGLE_FAN, bottomIndicesCount, GL_UNSIGNED_INT, 0);

	glFrontFace(GL_CCW);

	// Draw all children
	VisualObject::draw();

} // end draw


