QT:
The stencil code is written in QT and will therefore require QT Creator to be installed. QT Creator 4.5.0 Community edition has been tested and works.

How to build:
The included project files should be all you need to build, so just open the project in QT Creator and build the project.

How to run:
The compiled application takes two command line arguments.
The first argument is the path to the scene file that is being loaded to be rendered.
The second argument is the path of the file that the rendered image will be written out to.

About the code:
The main files you should need to edit/look at are pathtracer.cpp and the files in the scene/ directory, however feel free to modify any other code you like.

pathtracer.cpp is the class that gets passed in a scene and an array of pixels to fill in the color for, so this is where you should implement most of the actual path tracing.

In the scene directory, there is the Mesh class, which represents the objects that you are casting rays to collide with. Each mesh has a list of vertices, normals, colors and uvs, all of which are the same length, one for each vertex. They also contain a list of Vector3i's which correspond to the faces of the mesh. Each Vector3i describes a triangle in the mesh and contains 3 indices into the vertex, normal, etc arrays. There is also a list of integers, one for each face, that contains the indices of the material for that face. These indices index into the final array, the material array. This array is loaded from the .mat file that likely comes with the .obj file that you are loading, and will contain whatever material properties were specified for the mesh.
