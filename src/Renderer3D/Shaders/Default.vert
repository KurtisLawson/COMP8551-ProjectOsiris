// This link is a handy plugin for syntax highlighting for GLSL code in Visual Studio.
// https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL
// ----------------------------------------------------------------------------------

#version 330 core
layout (location = 0) in vec3 aPos;			// The "position" attribute has a position index 0
layout (location = 1) in vec3 aCol;		// The "color" attribute has a position index of 1
layout (location = 2) in vec2 aTexCoord;	// The "texture" attribute has a position index of 2

out vec3 FragPos; //creates a difference in fragment position and light position
out vec3 vertexColor; // specify a color output to the fragment shader
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
	vertexColor = mat3(transpose(inverse(model))) * aCol;
	TexCoord = aTexCoord;
}