#include "model.h"

vector<unsigned char> Model::getData() {
	string bytes;
	string uri = JSON["buffers"][0]["uri"];

	string fileStr = string(file);
	string fileDir = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	bytes = getFileContents((fileDir + uri).c_str());

	vector<unsigned char> data(bytes.begin(), bytes.end());

	return data;
}

vector<float> Model::getFloats(json accessor) {
	vector<float> floatVec;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	string type = accessor["type"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpret the type and store it into numPerVert
	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

	// Go over all the bytes in the data at the correct place using the properties from above
	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
		float value;
		memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

vector<GLuint> Model::getIndices(json accessor) {
	vector<GLuint> indices;

	// Get properties from the accessor
	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int componentType = accessor["componentType"];

	// Get properties from the bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Get indices with regards to their type: unsigned int, unsigned short, or short
	unsigned int beginningOfData = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			unsigned int value;
			memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

vector<vec2> Model::groupFloatsV2(vector<float> floats) {
	vector<vec2> vectors;

	for (int i = 0; i < floats.size(); i)
		vectors.push_back(vec2(floats[i++], floats[i++]));

	return vectors;
}

vector<vec3> Model::groupFloatsV3(vector<float> floats) {
	vector<vec3> vectors;

	for (int i = 0; i < floats.size(); i)
		vectors.push_back(vec3(floats[i++], floats[i++], floats[i++]));
	
	return vectors;
}

vector<vec4> Model::groupFloatsV4(vector<float> floats) {
	vector<vec4> vectors;

	for (int i = 0; i < floats.size(); i)
		vectors.push_back(vec4(floats[i++], floats[i++], floats[i++], floats[i++]));
	
	return vectors;
}

vector<GLfloat> Model::assembleVertices(vector<vec3> coords, vector<vec2> texUV, vector <vec3> normals) {
	vector<GLfloat> result;

	for (int i = 0; i < coords.size(); i++) {
		// Retrieve coordinates
		result.push_back(coords[i].r);
		result.push_back(coords[i].g);
		result.push_back(coords[i].b);

		// Retrieve colors
		result.push_back(1.0f);
		result.push_back(1.0f);
		result.push_back(1.0f);

		// Retrieve texUVs
		result.push_back(texUV[i].r);
		result.push_back(texUV[i].g);

		// Retrieve light normals
		result.push_back(normals[i].r);
		result.push_back(normals[i].g);
		result.push_back(normals[i].b);
	}

	return result;
}

void Model::loadMesh(unsigned int id) {
	// Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][id]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][id]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][id]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][id]["primitives"][0]["indices"];

	// Use accessor indices to get all vertices components
	vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	vector<vec3> positions = groupFloatsV3(posVec);
	vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	vector<vec3> normals = groupFloatsV3(normalVec);
	vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	vector<vec2> texUVs = groupFloatsV2(texVec);

	// Combine all the vertex components and also get the indices and textures
	vector<GLfloat> vertices = assembleVertices(positions, texUVs, normals);

	vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	vector<Texture> textures = getTextures();

	// Combine the vertices, indices, and textures into a mesh
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, mat4 matrix) {
	// Current node
	json node = JSON["nodes"][nextNode];

	// Get translation if it exists
	vec3 translation = vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);
		translation = make_vec3(transValues);
	}
	// Get quaternion if it exists
	quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = make_quat(rotValues);
	}
	// Get scale if it exists
	vec3 scale = vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = make_vec3(scaleValues);
	}
	// Get matrix if it exists
	mat4 matNode = mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);
		matNode = make_mat4(matValues);
	}

	// Initialize matrices
	mat4 trans = mat4(1.0f);
	mat4 rot = mat4(1.0f);
	mat4 sca = mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	trans = translate(trans, translation);
	rot = mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Multiply all matrices together
	mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// Check if the node contains a mesh and if it does load it
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// Check if the node has children, and if it does, apply this function to them with the matNextNode
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

vector<Texture> Model::getTextures() {
	vector<Texture> textures;

	string fileStr = string(file);
	string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Go over all images
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri of current texture
		string texPath = JSON["images"][i]["uri"];

		// Check if the texture has already been loaded
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		// If the texture has been loaded, skip this
		if (!skip)
		{
			// Load diffuse texture
			if (texPath.find("baseColor") != string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			// Load specular texture
			else if (texPath.find("metallicRoughness") != string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	cout << textures.size() << endl;

	return textures;
}

Model::Model(const char* file) {
	string text = getFileContents(file);
	JSON = json::parse(text);

	this->file = file;
	data = getData();

	traverseNode(0);
}