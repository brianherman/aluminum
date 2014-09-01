// texture.cpp : Defines the entry point for the console application.
//
// Loads in a texture using libfreeimage and applies a bloom filter to it. 
// Moving the mouse along the x-axis changes the amount of glow.

#include "Aluminum.h"
#pragma comment ( lib, "Aluminum.lib" )


using namespace aluminum;
using glm::vec3;
using glm::mat4;

class TextureExample : public RendererWin32{

public:

	mat4 model, view, proj;

	Program program;
	ResourceHandler rh;

	GLint posLoc = 0;
	GLint texCoordLoc = 1;

	Texture texture;
	MeshBuffer mb1;

	float bloomAmt = 0.0;

	void loadProgram(Program &p, const std::string& name) {

		p.create();

		p.attach(p.loadText(name + ".vsh"), GL_VERTEX_SHADER);
		glBindAttribLocation(p.id(), posLoc, "vertexPosition");
		glBindAttribLocation(p.id(), texCoordLoc, "vertexTexCoord");

		p.attach(p.loadText(name + ".fsh"), GL_FRAGMENT_SHADER);

		p.link();
	}

	void onCreate() {
		//loadProgram(program, "resources/texture");
		//loadTexture(texture, "resources/hubble.jpg");
		rh.loadProgram(program, "resources/texture", posLoc, -1, texCoordLoc, -1);
		rh.loadTexture(texture, "resources/hubble.jpg");

		mb1.init(MeshUtils::makeRectangle(), posLoc, -1, texCoordLoc, -1);

		model = glm::mat4();
        view = glm::mat4();
        proj = glm::mat4();
  //       proj = glm::perspective(45.0, 1.0, 0.1, 100.0);
		// view = glm::lookAt(vec3(0.0, 0.0, -2), vec3(0, 0, 0), vec3(0, 1, 0));
		// model = glm::mat4();

		// glEnable(GL_DEPTH_TEST);
		// glViewport(0, 0, width, height);
		// glClearColor(0.3, 0.3, 0.3, 1.0);
	}

	void onFrame(){

		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.bind(); {

			glUniformMatrix4fv(program.uniform("model"), 1, 0, ptr(model));
			glUniformMatrix4fv(program.uniform("view"), 1, 0, ptr(view));
			glUniformMatrix4fv(program.uniform("proj"), 1, 0, ptr(proj));

			glUniform1f(program.uniform("bloom"), bloomAmt);
			glUniform1i(program.uniform("tex0"), 0);

			texture.bind(GL_TEXTURE0); {
				mb1.draw();
			} texture.unbind(GL_TEXTURE0);

		} program.unbind();
	}

	void onReshape() {
		glViewport(0, 0, width, height);
	}


	void mouseMoved(int px, int py) {
		bloomAmt = ((float)px / (float)width) * 0.02; //bloom between 0.00 -> 0.02
	}

};


int main() {
	TextureExample().start("Texture Example");
	return 0;
}
