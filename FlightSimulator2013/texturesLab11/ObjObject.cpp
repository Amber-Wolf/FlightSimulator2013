#include "ObjObject.h"

#include "vector"

#include "vertexStructs.h"

ObjObject::ObjObject(OpenGLApplicationBase * OpenGLApp, string model, string texName, vec4 color)
	:VisualObject(OpenGLApp), modelName(model), textureName(texName), objectColor(color)
{ }

void ObjObject::initialize()
{
	textureObject = setupTexture( textureName );

	buildShaderProgram();
	glUseProgram(shaderProgram);
	
	initObj();
}

void ObjObject::initObj(){

	GLuint VBO;
	//GLuint CBO;
	GLuint IBO;

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// TODO: read .obj
	ifstream readfile;
	readfile.open(modelName);
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec2> tex;
	vector<vec3> faces;

	vector<pntVertexData> v;
	vector<GLuint> indices;

	// the idea to use fscanf came instead of trying to use aoif to convert char* to ints and floats came from opengl-tutorial.org
	FILE* file = fopen(modelName.c_str(), "r");

	if(file == NULL){
		return;
	}

	while(1){
		char header[128];
		int num = fscanf(file,  "%s", header);
		if(num == EOF){
			break;
		}
		
		// if this is a vertex line...
		if ( strcmp( header, "v" ) == 0 ){
			vec3 vertex;
			// read values and store
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			vertices.push_back(vertex);
		}

		// if this is a texture coordinate (UV) line...
		if ( strcmp( header, "vt" ) == 0 ){
			vec2 texCoords;
			fscanf(file, "%f %f\n", &texCoords.x, &texCoords.y);
			tex.push_back(texCoords);
		}

		// if this is a normals line...
		if ( strcmp( header, "vn" ) == 0 ){
			vec3 normal;
			// read values and store
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			normals.push_back(normal);
		}

		// if this is a faces line
		if ( strcmp( header, "f" ) == 0 ){
			vec3 vertex;
			vec3 normal;
			vec3 texture;

			int vert1, vert2, vert3;
			int normal1, normal2, normal3;
			int tex1, tex2, tex3;

			// read values and store
			fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vert1, &tex1, &normal1, &vert2, &tex2, &normal2, &vert3, &tex3, &normal3);
			//vertices.push_back(vertex);

			v.push_back(pntVertexData(vertices[vert1-1],normals[normal1-1],tex[tex1-1]));
			indices.push_back((GLuint)v.size()-1);
			v.push_back(pntVertexData(vertices[vert2-1],normals[normal2-1],tex[tex2-1]));
			indices.push_back((GLuint)v.size()-1);
			v.push_back(pntVertexData(vertices[vert3-1],normals[normal3-1],tex[tex3-1]));
			indices.push_back((GLuint)v.size()-1);
		}

	}

	
	vector<vec4> c;

	for(int i = 0; i < v.size(); i++){
		c.push_back(vec4(1,1,1,1));
	}
	

	numberOfIndices = indices.size();

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
} // end initialize

void ObjObject::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		// Create array of ShaderInfo structs that specifies the vertex and
		// fragment shaders to be compiled and linked into a program.
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVS-phong-per-frag.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-phong-toon-per-frag.frag"},
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
							 getUniformLocation( shaderProgram, "object.textureMapped"),
							 getUniformLocation( shaderProgram, "object.toon") );
	
		//material.setAmbientAndDiffuseMat( objectColor );
		material.setAmbientAndDiffuseMat( vec4(0.8f,0.6f,0.2f,1.0f) * 0.5f );
		material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		material.setSpecularExponentMat( 64.0f );
		material.setTextureMapped(true);
		material.setToon(true);

	} // end buildShaderProgram


// Preform drawing operations
void ObjObject::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	//glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setShaderMaterialProperties();

	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw top
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw