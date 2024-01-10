#version 330 core

// Includes from C++ file

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;

layout (location = 3) in vec2 aTex;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Will output to frag a inputted color
out vec3 color;
out vec3 Normal;
out vec3 crntPos;
out vec2 texCoord;



uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{				// model *  translation * rotation * scale * vec4(aPos, 1.0f)
	crntPos = vec3(model *  translation * rotation * scale * vec4(aPos, 1.0f));


   // Send access to frag
   Normal = aNormal;
   color = aColor;
   texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
   

   // Set pos according to matrices
   gl_Position = camMatrix * vec4(crntPos, 1.0);

}