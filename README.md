# Assignment 1: Path Tracing (Path)

**Released:** 2/3/23

**Milestone Due:** 2/10/23 @ 11:59pm EST

**Final Assignment Due:** 2/17/23 @ 11:59pm EST

In this assignment, you will implement a physically-based renderer based on the path tracing algorithm. Path tracing is a simple, elegant Monte Carlo sampling approach to solving the rendering equation. Like ray tracing, it produces images by firing rays from the eye/camera into the scene. Unlike basic ray tracing, it generates recursive bounce rays in a physically accurate manner, making it an unbiased estimator for the rendering equation. Path tracers support a wide variety of interesting visual effects (soft shadows, color bleeding, caustics, etc.), though they may take a long time to converge to a noise-free image.

If you have written a ray tracer in a previous graphics course (e.g. CS 1230 here at Brown), you are welcome to extend your ray tracing code for this assignment. If you do not already have a ray tracer, or if you do but you’d rather start from a clean slate, we provide a bare-bones ray tracer below (see "Resources").

## Relevant Reading

- The lecture slides!
- [The Global Illumination Compendium](https://people.cs.kuleuven.be/~philip.dutre/GI/TotalCompendium.pdf 'The Global Illumination Compendium')
- [The Graphics Codex](http://graphicscodex.com/), a $10 rendering reference (by Brown alum Morgan McGuire)

## Requirements

This assignment is out of **100 points**.

Your path tracer must implement the following features:

- Four basic types of BRDFs:
  - Diffuse **(7 points - milestone deadline)**
  - Glossy reflections **(7 points)**
  - Mirror reflection **(7 points)**
  - Refraction (with Fresnel reflection) **(10 points)**
- Soft shadows **(7 points)**
  - From sampling area light sources
- Indirect illumination **(7 points - milestone deadline)**
  - Your path tracer must produce color bleeding and caustics
- Russian roulette path termination **(5 points)**
- Event splitting **(15 points)**
  - At each recursive path tracing step, separately accumulate the contributions of direct lighting and indirect lighting.
  - Be careful not to double-count light sources in both contribution terms!
- Tone mapping **(5 points)**
  - Your path tracer should take a scene file as input and output an image to disk. To produce output images, you’ll need to convert the high-dynamic range radiance values produced by the path tracer into low-dynamic range values that can be written to standard 24-bit RGB image formats such as JPEG or PNG. You’ll need a tone-mapping operator for this; a simple global operator such as Equation 3 of [this paper](https://www.cs.utah.edu/docs/techreports/2002/pdf/UUCS-02-001.pdf) is fine.
- Example images **(5 points)**
  - You must submit images depicting at least two different scenes. These images should demonstrate every feature that your path tracer implements. You must also provide comparison images of a scene with and without direct lighting. A Cornell Box scene can be a good test-bed for different types of rendering effects; the starter code repository contains such an example scene.
  - Submitted images should be easily viewable low dynamic range images in a format like PNG. You should try to produce the highest-resolution, highest-sample-count images you can by the submission deadline.
- Readme **(5 points)**
  - Additionally, you must submit a [Markdown](https://www.markdownguide.org/basic-syntax/) README file. The README file should describe how to run your path tracer (e.g. how to specify different scene file inputs). This file should list all the features your path tracer implements. It should also describe what features are demonstrated in the images you’ve submitted. These images should be embedded in the README file.

Successfully implementing all of the requirements results in a total of **80/100 points** (a grade of B). To score **100/100** (or more!), you’ll need to implement some extra features (see "Extra Features" below)

## Milestone:

By the milestone deadline, you should have implemented diffuse reflection and indirect illumination.
Having these features implemented by the milestone deadline is worth **5 points**. That is to say: if you implement them correctly by the final submission deadline, but not by the milestone deadline, you'll incur a -5 point penalty to your project grade.
Why do we do this? We want to encourage you to start early and develop good habits that will carry over to the rest of the assignments in the course (the latter of which do not have formal milestones).

Please note that we will not have your milestone submissions graded before the final submission deadline--that's only a week's time, and we're only human.
If you want feedback on how well your code is working as of the milestone deadline, we expect you to take initiative and come talk to us at hours.

## Material Specifications

**Required Materials:**
Your path tracer must support these 4 types of materials:

- Ideal diffuse
- Glossy specular
- Ideal specular (mirror)
- Dielectric refraction (refraction + Fresnel reflection)

**You do not need to support any materials beyond this.** For example, it’s not necessary to handle materials that combine a diffuse and a glossy specular component (or other combinations of material types). You may do so if you wish, but it’s not required—in your submission, we only expect to see images depicting the 4 types of materials above.

**Getting and interpreting materials from input files**

Our scene files load ‘mesh’ primitives from .obj files, and .obj files sometimes also come with an associated .mtl file. The .mtl file provides materials for the geometry in the .obj file at the per-face level (i.e. per triangle). The latest revision of the starter code includes some commented-out example code showing how to read both types of materials (per-object and per-face) from a mesh that’s been hit by a ray. See the `PathTracer::traceRay` function in `pathtracer.cpp` for these examples.

The materials parsed from the .mtl file are a single material struct/class that contains coefficients for diffuse, specular, reflective, etc. See `utils/tiny_obj_loader.h` in the stencil code for detailed parsing logic. A detailed description of mtl specification can be found at http://paulbourke.net/dataformats/mtl/. The parameters that are most relevant to this assignment are Kd, Ks, Ke, Ns, Ni and Illum (you would mostly only need 2 and 5 for illum).

To convert such a material to one of the 4 required types of materials for this assignment, you can use whatever logic you like. For example, if you encounter a material with nonzero diffuse coefficient but zero for all other coefficients, a sensible thing to do might be to treat that material as an ideal diffuse reflector.

**You do not need to worry about making your code handle all possible types of input materials.** The only scene files your code needs to run on are the ones you design for it. So, it’s perfectly fine to ignore the case when, say, a material has both nonzero diffuse coefficient and nonzero specular coefficient, since you are not required to implement materials that combine those two effects.

**Scenes for submission**

The assignment spec states that you need to demonstrate your path tracer on at least two different input scenes. You could use the Cornell Box scenes included in the starting code for this purpose. Those are taken from https://casual-effects.com/data/, which contain many other obj files that you can test on. You can also build your own scenes, and we definitely encourage doing so.

**Light sources**

The assignment spec states that you need to support area light sources resulting in soft shadows. The recommended way to do this is to treat objects with emissive materials as area light sources. The best place in the code to look through the scene objects for emissive ones is probably in `Scene::parseTree`, which constructs a vector of scene objects before building a BVH acceleration structure out of them. You’ll also need to implement a method for sampling a point on an area light source (i.e. sampling a point on the surface of an emissive triangle mesh).

## Extra Features

Each of the following features that you implement will earn you extra points. The features are ordered roughly by difficulty of implementation.

- Attenuate refracted paths **(5 points)**
  - Attenuate the contribution of a refracted light path based on the distance it travels through the refracting object. For example, a path going through a gray glass crystal ball ends up less bright if it goes a long way through the glass rather than a short distance.
- Stratified sampling **(5 points)**
  - For your path tracer to converge to a relatively noise-free image, it’ll need to trace many rays per pixel. Rather than scattering them randomly over the sensor square, try stratified sampling, in which you divide the sensor area into a grid, and then pick one or two or twenty samples uniformly in each grid square.
  - Include at least one image comparing this approach to uniform random ray sampling, and describe this comparison in your README.
- Depth of field **(5 points)**
  - Instead of generating rays from a single eye location, scatter the starting location over a lens (e.g. a disk). This produces an effect that mimics camera defocus blur.
  - The scene will be in focus only at the same depth away from the eye/lens as the virtual film plane--rays diverge as they move away from this plane, leading to defocus blur. You can control the location of the focus plane by changing the location of the virtual film plane.
  - Obviously, this is an approximation, and not a physically-based model of how cameras work. If you’re interested in how you might do that, check out [this paper](https://graphics.stanford.edu/papers/camera/) on putting a simulated camera lens system into your path tracer.
- More advanced BRDFs **(5-10 points each)**
  - There are many other types of BRDFs you could implement to get more interesting material appearance.
  - The [Ward anisotropic BRDF](https://en.wikipedia.org/wiki/Specular_highlight#Ward_anisotropic_distribution) and the [Cook-Torrance microfacet model](https://en.wikipedia.org/wiki/Specular_highlight#Cook.E2.80.93Torrance_model) are just two possibilities.
- Low discrepancy sampling **(10 points)**
  - The rendering problem that a path tracer tries to solve is an integration problem, and there’s nothing that says we have use random samples to estimate integrals. Rather, any set of samples that are ‘well-spaced out’ but not perfectly uniform (to avoid aliasing) ought to work. This is the idea behind low-discrepancy sampling (also known as Quasi-Monte Carlo): use an algorithm that deterministically generates random sample points that have the right properties.
  - You can find a more detailed introduction to this concept [here](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/introduction-quasi-monte-carlo).
  - Include at least one image comparing this approach to uniform random ray sampling (or stratified sampling), and describe this comparison in your README.
- BRDF importance sampling **(10 points)**
  - When tracing a recursive ray from a surface intersection, you can choose it randomly from the hemisphere; alternatively, you can choose it in a way that's approximately proportional to the BRDF, and use importance sampling.
  - Include at least one image comparing this approach to uniform hemisphere sampling, and describe this comparison in your README.
- Multiple importance sampling **(10 points)**
  - Next event estimation separates the contributions of direct vs. indirect illumination. Instead, you can use multiple importance sampling (MIS), which provides a general procedure for combining samples drawn using different sampling strategies.
  - Check out [the chapter on MIS from Erich Veach’s thesis](https://graphics.stanford.edu/courses/cs348b-03/papers/veach-chapter9.pdf) to learn more about how it works.
  - Include at least one image comparing MIS to next event estimation, and describe this comparison in your README.
- Image-based lighting **(10 points)**
  - Instead of using area lights to illuminate your scene, use a high-dynamic range hemisphere or cube-map as a light source. This allows you mimic real-world lighting environments.
  - Paul Debevec provides several [light probe images](http://www.pauldebevec.com/Probes/) which you can use for image-based lighting.
- Fixed Function Denoising **(10 points)**
  - You can use post-processing filters to denoise images rendered with few samples per pixel. For example, take a look at the wavelet transform methods as described [here](https://web.ece.ucsb.edu/~psen/Papers/EG13_RemovingMCNoiseWithGeneralDenoising.pdf) and [here](https://research.nvidia.com/sites/default/files/pubs/2017-07_Spatiotemporal-Variance-Guided-Filtering%3A//svgf_preprint.pdf), which denoise the image by transforming the input image into the wavelet domain.
  - Include at least one set of comparison images for before and after denoising, and describe your methods in your README.
- Learned Denoising **(20 points)**
  - Rather than using fixed filter kernels, you can also use a neural network such as a CNN to learn kernel weights for denoising rendered images. Some example architectures are described in [this paper](https://people.engr.tamu.edu/nimak/Data/SIGGRAPH15_LBF.pdf) and [this paper](https://mint.univ-reims.fr/files/2020-1/BaVoWi_etal17.pdf).
  - You will be responsible for finding training data and training your model, so we consider this a fairly large undertaking.
  - Include at least one set of comparison images for before and after denoising, and describe your architecture and methods in your README.
- CUDA C++ Implementation **(20 points)**
  - Write your code in [CUDA C++](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html) so that it can take advantage of GPU parallelism
- Something else!
  - This list is not meant to be exhaustive--if you’ve got another advanced feature in mind, go for it! (though you may want to ask a TA or the instructor first if you’re concerned about whether the idea is feasible)
  - You might consider looking through [a recent paper](https://www.arnoldrenderer.com/research/Arnold_TOG2018.pdf) written by the authors of the Arnold renderer for some advanced feature ideas.

Any extra features you implement must be described in your README. **To get extra credit, you must also submit comparison images for each feature that you implement.**

You can also get extra credit for sharing a scene you’ve created with the class **(2 points)**. Post the scene file(s) and a rendering of the scene to Slack. If your path tracer is built on top of your own ray tracer and uses a different scene file format than the one used by the bare-bones ray tracer we provide, or if you’ve modified our scene file format in any way, you should also post instructions for how to read your scene file format.

## Resources

Feel free to use this stencil code to get started.

This includes all the code needed to load scene files, set up a scene, trace rays through pixels, compute intersections of rays with the scene, and output images. If you already have your own ray tracer, you may still want to use (a) the scene file parser and (b) the bounding volume hierarchy (for fast ray-scene intersections) provided by this code.

<details>
  <summary>Using the CS Grid for rendering (optional)</summary>

Rendering high quality images can take a long time. Rather than make your poor laptop sweat for hours, you may want to look into using the department's compute cluster, a.k.a. [The Grid](https://cs.brown.edu/about/system/services/hpc/). Anyone with a CS login has the ability to submit jobs to these machines. Some of them are pretty old, and none of them are terribly fast, but they are a nice resource for massively-parallel compute jobs (e.g. rendering!) Remember that because your path tracer is unbiased (or at least, it should be!), you can render the same scene multiple times on different Grid nodes and then average the resulting images together to get a lower-variance final image.

</details>

## Submission Instructions

For the milestone deadline, submit your GitHub repository to the "Path (Milestone)" assignment. **Your submission should include at least one image produced by your in-progress path tracer**, showing that you've implemented the required milestone features (diffuse reflection and indirect illumination).

For the final deadline, submit your branch of the Github classroom repository to the “Path (Final)” assignment.

## Implementation & Debugging Tips

To set these command line arguments in QT, go to the Projects tab on the left, under build and run, under your currently configured kit, click run, and you should have a screen where you can enter command line arguments to run the executable with.
- There are a lot of different probability calculations that go into computing the contribution of a ray to the final image. Make sure you really understand all of this math before you start trying to implement anything. You don’t want to get into the situation where your code is producing images that don’t quite look right, and all you can do is resort to aimlessly tweaking parts of the code (e.g. fiddling with constants, flipping minus signs) to try and make them look right.
- Don’t try to implement all the required features at once. Implement them one by one, thoroughly debugging as you go.
- It's useful to be able to look at your rendered output images _before_ you've implemented a tone mapping function. To do this, you can write your pathtracer's raw per-pixel radiance values to a high-dynamic range image format. The simplest such format is [PFM (short for Portable Float Map)](https://www.pauldebevec.com/Research/HDR/PFM/). You can also try using a library such as [tinyexr](https://github.com/syoyo/tinyexr) to read/write the industry-standard OpenEXR file format. Many different image viewing/editing apps support these formats.
- Path tracers can take a long time to produce an image. The first thing you should do is **make sure to compile your code in "Release" mode** (or with all compiler optimizations enabled, if you’re not using Qt Creator). To speed up your code-test-iterate cycle, you’ll want to render low-resolution images (or, small windows cut out of a larger image that focus on some region of interest). In addition, you can also parallelize your code. Using [OpenMP’s parallel for loop](http://supercomputingblog.com/openmp/tutorial-parallel-for-loops-with-openmp/) is a good option, and there is a commented out line in the starter code (pathtracer.cpp, line 21) that illustrates how to do so. **Strongly recommended!**
- When you allocate memory, make sure to zero it out. Bugs can creep into your code from uninitialized values.
- If you’re noticing ‘speckling’ artifacts, i.e. individual isolated pixels that look incorrect: try using an image editor to identify the coordinates of a problematic pixel (many image editors will display this information somewhere as you mouse over the image). Then, set a breakpoint in your code that only fires on that pixel, and use a debugger to step through and see what is going wrong.
- "Does this caustic look right?" The human visual system is easy to fool, and it can be hard to tell if your pathtracers are producing correct and/or unbiased results. You can compare your renders with "ground truth" images stored in example-scenes/ground_truth as you write and debug your pathtracer. These images were either rendered with the production renderer [Mitsuba](https://www.mitsuba-renderer.org/), or found [here](https://graphics.stanford.edu/~henrik/images/cbox.html).

## About the Code

### Eigen

After cloning the repository, run
```
git submodule update --init
```
to pull in the Eigen dependency

### Qt

The stencil code is written in Qt and will therefore require Qt Creator to be installed. Qt Creator 9 and 6.2 Community Edition have been tested. If you do not have QT setup, follow section 3 of [this guide from cs1230](https://cs1230.graphics/labs/lab1#qt-and-qt-creator).

### How to Build

The included project files should be all you need to build, so just open the project in Qt Creator ("Open Project" > select `CMakeLists.txt`) and build the project.

### How to Run

The compiled application takes two command line arguments. The first argument is the path to the scene file that is being loaded to be rendered. The second argument is the path of the file that the rendered image will be written out to.

To set these command line arguments in Qt, go to the Projects tab on the left, under Run, under your currently configured kit, click run, and you should have a screen where you can enter command line arguments to run the executable with.

Additional command line arguments can be added in `main.cpp`. The stencil code uses `QCommandLineParser` to parse the command line arguments, so take a look at the documentation if you want to add more arguments.

### Code Structure

The main files you should need to edit/look at are `pathtracer.cpp` and the files in the `scene/` directory. However, feel free to modify any other code you like.

`pathtracer.cpp` is the class that receives a scene and an array of pixels to fill in the color for, so this is where you should start implementing the actual path tracing. However, it's a good idea to think about your design beforehand so that you don't end up putting all of your code into one file!

In the scene directory, there is the mesh class, which represents the objects that you are casting rays to collide with. Each mesh has a list of vertices, normals, colors and UVs, all of which are the same length, one for each vertex. They also contain a list of `Vector3i`s which correspond to the faces of the mesh. Each `Vector3i` describes a triangle in the mesh and contains 3 indices into the vertex, normal, etc arrays. There is also a list of integers, one for each face, that contains the indices of the material for that face. These indices index into the final array, the material array. This array is loaded from the .mat file that likely comes with the .obj file that you are loading, and will contain whatever material properties were specified for the mesh.

### Example Scene Files

The stencil code uses a modified version of the CS123 scene files, which are xml files. The full spec for the scene files can be found in `scenefile.pdf`. Note that the stencil code only supports mesh primitives and not cubes, cylinders, etc. However, meshes can be used to represent any of these shapes, and if you want, you can add support for primitives.

Some example scenes can be found in the `example-scenes` directory.
