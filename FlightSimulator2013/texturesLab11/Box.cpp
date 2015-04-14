#include "Box.h"

#include "vector"

#include "vertexStructs.h"

Box::Box(OpenGLApplicationBase * OpenGLApp, GLfloat w, vec4 color)
	:VisualObject(OpenGLApp), width(w), objectColor(color)
{ }

void Box::initialize()
{
	textureObject = setupTexture( "earth.bmp" );

	buildShaderProgram();
	glUseProgram(shaderProgram);
	
	initTopBottomBack();
	initSides();
}

void Box::initTopBottomBack(){

	GLuint VBO;
	GLuint CBO;
	GLuint IBO;

	glGenVertexArrays (1, &vertexArrayObjectFBTB);
	glBindVertexArray(vertexArrayObjectFBTB);

	vec3 v0 = vec3(-width/2, width/2, width/2);
	vec3 v1 = vec3(width/2, width/2, width/2);
	vec3 v2 = vec3(-width/2, -width/2, width/2);
	vec3 v3 = vec3(width/2, -width/2, width/2);
	
	vec3 v4 = vec3(-width/2, width/2, -width/2);
	vec3 v5 = vec3(width/2, width/2, -width/2);
	vec3 v6 = vec3(-width/2, -width/2, -width/2);
	vec3 v7 = vec3(width/2, -width/2, -width/2);

	vector<pntVertexData> v;

	vec2 t0 = vec2(0,1);
	vec2 t1 = vec2(1,1);
	vec2 t2 = vec2(0,0);
	vec2 t3 = vec2(1,0);

	vec2 t6 = vec2(0,1);
	vec2 t7 = vec2(1,1);
	vec2 t4 = vec2(0,0);
	vec2 t5 = vec2(1,0);

	v.push_back(pntVertexData(v0, normalize(vec3(-1,1,1)), t0));
	v.push_back(pntVertexData(v1, normalize(vec3(1,1,1)), t1));
	v.push_back(pntVertexData(v2, normalize(vec3(-1,-1,1)), t2));
	v.push_back(pntVertexData(v3, normalize(vec3(1,-1,1)), t3));
	
	v.push_back(pntVertexData(v4, normalize(vec3(-1,1,-1)), t4));
	v.push_back(pntVertexData(v5, normalize(vec3(1,1,-1)), t5));
	v.push_back(pntVertexData(v6, normalize(vec3(-1,-1,-1)), t6));
	v.push_back(pntVertexData(v7, normalize(vec3(1,-1,-1)), t7));

	/*
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	*/
	vector<vec4> c;

	vec4 c1 = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 c2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 c3 = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 c4 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 c5 = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	c.push_back(c1);
	c.push_back(c2);
	c.push_back(c3);
	c.push_back(c4);
	c.push_back(c5);
	c.push_back(c1);
	c.push_back(c2);
	c.push_back(c3);
	/*
	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, c.size() * sizeof(vec4), &c[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	*/
	vector<GLuint> indices;

	// front
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// back
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(5);

	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(6);

	// top
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(4);

	// bottom
	indices.push_back(7);
	indices.push_back(3);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(3);
	indices.push_back(2);

	numberOfIndices = indices.size();

	/*
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	*/

	glGenVertexArrays (1, &vertexArrayObjectFBTB);
	glBindVertexArray( vertexArrayObjectFBTB );

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
} // end initialize

void Box::initSides(){

	GLuint VBO;
	GLuint CBO;
	GLuint IBO;

	glGenVertexArrays (1, &vertexArrayObjectSides);
	glBindVertexArray(vertexArrayObjectSides);

	vec3 v0 = vec3(-width/2, width/2, width/2);
	vec3 v1 = vec3(width/2, width/2, width/2);
	vec3 v2 = vec3(-width/2, -width/2, width/2);
	vec3 v3 = vec3(width/2, -width/2, width/2);
	
	vec3 v4 = vec3(-width/2, width/2, -width/2);
	vec3 v5 = vec3(width/2, width/2, -width/2);
	vec3 v6 = vec3(-width/2, -width/2, -width/2);
	vec3 v7 = vec3(width/2, -width/2, -width/2);

	vector<pntVertexData> v;

	vec2 t0 = vec2(0,1);
	vec2 t1 = vec2(1,1);
	vec2 t2 = vec2(0,0);
	vec2 t3 = vec2(1,0);

	vec2 t5 = vec2(0,1);
	vec2 t4 = vec2(1,1);
	vec2 t7 = vec2(0,0);
	vec2 t6 = vec2(1,0);

	v.push_back(pntVertexData(v0, normalize(vec3(-1,1,1)), t0));
	v.push_back(pntVertexData(v1, normalize(vec3(1,1,1)), t1));
	v.push_back(pntVertexData(v2, normalize(vec3(-1,-1,1)), t2));
	v.push_back(pntVertexData(v3, normalize(vec3(1,-1,1)), t3));
	
	v.push_back(pntVertexData(v4, normalize(vec3(-1,1,-1)), t4));
	v.push_back(pntVertexData(v5, normalize(vec3(1,1,-1)), t5));
	v.push_back(pntVertexData(v6, normalize(vec3(-1,-1,-1)), t6));
	v.push_back(pntVertexData(v7, normalize(vec3(1,-1,-1)), t7));

	/*
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	*/
	vector<vec4> c;

	vec4 c1 = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 c2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 c3 = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 c4 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 c5 = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	c.push_back(c1);
	c.push_back(c2);
	c.push_back(c3);
	c.push_back(c4);
	c.push_back(c5);
	c.push_back(c1);
	c.push_back(c2);
	c.push_back(c3);
	/*
	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, c.size() * sizeof(vec4), &c[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	*/
	vector<GLuint> indices;

	

	// side
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(3);

	indices.push_back(3);
	indices.push_back(5);
	indices.push_back(1);

	// side
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(0);
	indices.push_back(4);

	

	numberOfIndicesSides = indices.size();

	/*
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	*/

	glGenVertexArrays (1, &vertexArrayObjectSides);
	glBindVertexArray( vertexArrayObjectSides );

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
} // end initialize

void Box::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		// Create array of ShaderInfo structs that specifies the vertex and
		// fragment shaders to be compiled and linked into a program.
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVS-phong-per-frag.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-phong-per-frag.frag"},
			//{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
			//{ GL_FRAGMENT_SHADER, "pFSc.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		shaderProgram = LoadShaders(shaders);

	}
	 glUseProgram(shaderProgram);
		modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
		assert(modelLocation != 0xFFFFFFFF);

		solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		//assert(solidColorLocation != 0xFFFFFFFF);

		material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
							 getUniformLocation( shaderProgram, "object.diffuseMat"),
							 getUniformLocation( shaderProgram, "object.specularMat"),
							 getUniformLocation( shaderProgram, "object.specularExp"),
							 getUniformLocation( shaderProgram, "object.emissiveMat" ),
							 getUniformLocation( shaderProgram, "object.textureMapped") );
	
		//material.setAmbientAndDiffuseMat( objectColor );
		material.setAmbientAndDiffuseMat( vec4(1.0f,1.0f,1.0f,1.0f) );
		material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		material.setSpecularExponentMat( 64.0f );
		material.setTextureMapped(true);

	} // end buildShaderProgram


// Preform drawing operations
void Box::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setShaderMaterialProperties();

	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw top
	glBindVertexArray(vertexArrayObjectFBTB);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(vertexArrayObjectSides);
	glDrawElements(GL_TRIANGLES, numberOfIndicesSides, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw