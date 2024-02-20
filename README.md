# CPP_OpenGLEngine

Current main project which uses OpenGL to create a 3D glm vector space. Has been a blast creating this as I've learned a lot about GPU-coding as well as how to properly maintain a optimized buffer of data and allocate and deallocate it according to my circumstances. Courteous Object-Orientated design has been fun and created majority of algorithms and calculations myself instead of using libraries for optimization as well as really understanding how this code works. I've learned a lot about blender and vertex data as well as how bounding boxes can be calculated and used in general cost-effective detection of maybe minor objects. 
My current collision detection is created through a document online which outlines a ellipsoid based detection system of polygons. This means the user is treated as a bubble in its own ellipsoid normalized space and rolls around the space until collisons are detected in which he will roll around or up if theres a incline.
Bullets are using bounding box detection and have ray caste hit scan implemented as well. The entities also use bounding box detection for bullet collision.
Each thing is treated in worldspace as well with their transformation matrixes shifting them about amongst the world.
Code is still rusty around the edges but gave a huge insight which I'm bringing to new projects of the likeness and gives me comfort in GUI applications in C++ now. Still in heavy development as it works as my bed for learning more about OpenGL like animations and particle effects. (this is more a file dump than a build)

![2024-01-1002-28-18-ezgif com-optimize](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/cd0f9cdb-54af-4097-8cb2-a7661989ce38)

<img width="995" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/2bb74706-a816-4289-b9d0-3d13efe007b7">



![trimmedOpenGL-ezgif com-optimize (2)](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/ed7e62f3-21cd-4d54-9e02-20786b9ad700)


**With Flashlight .frag func**
<img width="997" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/4a772f84-6575-4875-8d24-635806f1c922">
![fallingOpenGLCat-ezgif com-optimize](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/f4c58a71-e407-4122-9213-aa7e7c5687a8)


**_Blender Snapshots:_**

**Model of Enemy Class Type**
<img width="1587" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/c449e420-4f34-463c-a21c-8a4e75f06e3a">

**Model of Bullet with Placeholder Texture**
<img width="1571" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/3b43afec-1b3a-48fb-9ecd-7c5f2c3e8418">

**Baked World Gen. (Rand. Walls In Demos Are Made At Runtime)**
<img width="1920" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/abab8c97-ad9e-457c-ae32-6bc9eb23c41d">
