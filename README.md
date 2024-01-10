# CPP_OpenGLEngine

Current main project which uses OpenGL to create a 3D glm vector space. Has been a blast creating this as I've learned a lot about GPU-coding as well as how to properly maintain a optimized buffer of data and allocate and deallocate it according to my circumstances. Courteous Object-Orientated design has been fun and created majority of algorithms and calculations myself instead of using libraries for optimization as well as really understanding how this code works. I've learned a lot about blender and vertex data as well as how bounding boxes can be calculated and used in general cost-effective detection of maybe minor objects. 
My current collision detection is created through a document online which outlines a ellipsoid based detection system of polygons. This means the user is treated as a bubble in its own ellipsoid normalized space and rolls around the space until collisons are detected in which he will roll around or up if theres a incline.
Bullets are using bounding box detection and have ray caste hit scan implemented as well. The entities also use bounding box detection for bullet collision.
Each thing is treated in worldspace as well with their transformation matrixes shifting them about amongst the world.
Code is still rusty around the edges but gave a huge insight which I'm bringing to new projects of the likeness and gives me comfort in GUI applications in C++ now. Still in heavy development as it works as my bed for learning more about OpenGL like animations and particle effects.

![2024-01-1002-28-18-ezgif com-optimize](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/cd0f9cdb-54af-4097-8cb2-a7661989ce38)
