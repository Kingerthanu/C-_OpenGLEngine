# CPP_OpenGLEngine

Current main project which uses OpenGL to create a 3D glm vector space. Has been a blast creating this as I've learned a lot about GPU-coding as well as how to properly maintain a optimized buffer of data and allocate and deallocate it according to my circumstances. Courteous Object-Orientated design has been fun and created majority of algorithms and calculations myself instead of using libraries for optimization as well as really understanding how this code works. I've learned a lot about blender and vertex data as well as how bounding boxes can be calculated and used in general cost-effective detection of maybe minor objects. 

<img src="https://media.giphy.com/media/i29DtU2PMkzsI/giphy.gif" alt="Blender + Fork = Yummers" width="120" height="90">



My current collision detection is created through a <a href="https://www.peroxide.dk/papers/collision/collision.pdf">document online</a> which outlines a ellipsoid based detection system of polygons. This means the user is treated as a bubble in its own ellipsoid normalized space and rolls around the space until collisons are detected in which he will roll around or up if theres a incline.
Bullets are using bounding box detection and have ray caste hit scan implemented as well but is inactive while still showing the tracer. The entities also use bounding box detection for bullet collision.

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/5e4ad275-8357-45ca-9c2e-4af4921ae1f1" alt="Cornstarch <3" width="95" height="99">

I personally developed a runtime algorithm in which creates randomly placed walls on my given floor plane by creating their own normals using winding order of our given vertices of a given wall to make a dual-sided, 4 vertex wall to help cutdown on some runtime. The walls work on random generation for their width and orientation/position but keep a shared height.
Each thing is treated in worldspace as well with their transformation matrixes shifting them about amongst the world.

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/83edb814-76a4-4818-804f-ba46b2501c2d" alt="Cornstarch <3" width="95" height="99">


Code is still rusty around the edges but gave a huge insight which I'm bringing to new projects of the likeness and gives me comfort in GUI applications in C++ now. Still in heavy development as it works as my bed for learning more about OpenGL like animations and particle effects. (this is more a file dump than a .exe build)

----------------------------------------------------------------------------

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/77fc6299-bc4f-479c-a927-0295775ace8c" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/77fc6299-bc4f-479c-a927-0295775ace8c" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/77fc6299-bc4f-479c-a927-0295775ace8c" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/77fc6299-bc4f-479c-a927-0295775ace8c" alt="Cornstarch <3" width="55" height="49">


**The Breakdown:**

This Program Works With A GLSL Window To Call OpenGL For Drawing A 3D Vector-Based Environment With Imported Models As Well As Runtime Generated Geometry. This World-Space Will Be Populated By Enemy Entities And The User Which Is Attracting The Enemy Cat Wizards And Which Is Also 
Supplied A 1-In-The-Chamber Projectile-Based Bullet To Hit The Enemies With. The Entities Will Have Their Own Motion Velocities In Which Will Increase As They "Charge" Toward You, Allowing You The Ability To Dodge Their Charge If Possible As They Can Overshoot, Giving You Opporunity To Hit Them While Also Giving Them The Opportunity To Hit You.

In The Programs Current State, We Have Only 3 Seperate (Albeit Connected) Hard-Baked World Geometry Made By Myself In Blender. This Is Our Spawn/Safe Space In Which Will Store The Shop And Will Heal The User.
We Also Have The SkyBox Which Is A 8-Vertex Based Mesh Holding Our PlayerSpace. Finally We Have The Frontline Which Is The Space Below The Spawn Which Is A Simple Square Plane. On Runtime, We Will Call A Function In Which Will Generate 4-Vertex Walls On The Frontline Space Randomly. 

We Also Have The Enemy Entities Which Currently Have Pathing To The Users Coordinates While Also Having Their Own Health And Model/Meshes.

In Our Current State, The User Is Represented In The World As A "Circle Of Influence" In Which They Move In The World As. We Then Use This Ball To Check Triangle Collisions In Our Ellipsoid Representation of Our World-Space.
This Way Of Representing Our User Allows Runtime Benefits On Collision Detection As Well More Intuitive Design As It Allows Our User To "Roll" Along Surfaces Giving Us More Dynamic Movement Opportunities During A Single Collision Check Compared To A More Binary Algorithm For Collision Checks. This Design Principle Also Allows Dynamic Collisions On Oddly Shapen Geometry As We Check Direct Model Meshes Instead Of More Hard-Baked Bounding Areas. Our User Can Also Jump Which Is Cool As It Allows Cool Stunts of Jumping Over Walls Or Enemies For Cool TrickShots.

Because We Know Our Entities Intentions In Our Game We Only Need Bounding Boxes To Represent Their Own Collisions As They Are Only Moving On A 2D Plane As They Only Slide Along The Frontlines Z-Height. Their Bounding Boxes
Are Utitilized For User Projectile Collisions Of Either Rays Or Actual Other Moving Bounding Boxed Mesh Models As We Have Currently Enabled For Our Project.

Other Sugar Is Added In The Program, Like Other Types Of Lighting Like Flash-Light In The Dark Or Spot Light On The Frontline But Mainly This Has Been A Major Turning Point For My Understanding Of Computer Graphics And Data Science In Applicable Projects Like This One. Has Been Super Fun And Am Planning On Coming Back To Finish Many Missing Features But Really Enabled Me To Get A Firm Understanding Of Buffers, GLSL, And OpenGL As Well As Vector Based Mathematics.

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/7890f95a-6284-44a8-8bf5-26ddc9ee1cc3" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/7890f95a-6284-44a8-8bf5-26ddc9ee1cc3" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/7890f95a-6284-44a8-8bf5-26ddc9ee1cc3" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/7890f95a-6284-44a8-8bf5-26ddc9ee1cc3" alt="Cornstarch <3" width="55" height="49">

----------------------------------------------------------------------------

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/33cd4b5f-2836-4083-8137-681294613cf1" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/33cd4b5f-2836-4083-8137-681294613cf1" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/33cd4b5f-2836-4083-8137-681294613cf1" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/33cd4b5f-2836-4083-8137-681294613cf1" alt="Cornstarch <3" width="55" height="49">


**Features:**

<img src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/cd0f9cdb-54af-4097-8cb2-a7661989ce38" alt="Code Running Displaying Scene, Collision, And Projectiles As Well As Entities" width="750" height="750">

<img width="1195" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/2bb74706-a816-4289-b9d0-3d13efe007b7">

![trimmedOpenGL-ezgif com-optimize (2)](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/ed7e62f3-21cd-4d54-9e02-20786b9ad700)


**With Flashlight .frag func**

![fallingAgainOpenGL-ezgif com-optimize](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/4a9973a9-7576-42c6-942f-4e8d7840e54c)
<img width="997" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/4a772f84-6575-4875-8d24-635806f1c922">
![fallingOpenGLCat-ezgif com-optimize](https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/f4c58a71-e407-4122-9213-aa7e7c5687a8)


**_Blender Snapshots:_**

**Model of Enemy Class Type**
<img width="1587" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/c449e420-4f34-463c-a21c-8a4e75f06e3a">

**Model of Bullet with Placeholder Texture**
<img width="1571" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/3b43afec-1b3a-48fb-9ecd-7c5f2c3e8418">

**Baked World Gen. (Rand. Walls In Demos Are Made At Runtime)**
<img width="1920" alt="image" src="https://github.com/Kingerthanu/CPP_OpenGLEngine/assets/76754592/abab8c97-ad9e-457c-ae32-6bc9eb23c41d">

<img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/e580c5b3-6568-4a56-9132-38e08f708b6e" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/e580c5b3-6568-4a56-9132-38e08f708b6e" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/e580c5b3-6568-4a56-9132-38e08f708b6e" alt="Cornstarch <3" width="55" height="49"><img src="https://github.com/Kingerthanu/CPP_3D_OpenGLEngine/assets/76754592/e580c5b3-6568-4a56-9132-38e08f708b6e" alt="Cornstarch <3" width="55" height="49">

