This projept only suports x86, be sure to switch from Debug/Release x64 to x86 when compiling.


https://user-images.githubusercontent.com/67437415/143783809-2162886b-6f0c-48b0-b720-28ddf7f03f48.mp4


https://user-images.githubusercontent.com/67437415/143783834-b50a4d50-a7fd-4a7f-ba2e-4958fb8f99b9.mp4


SandBox is a C++ project where I use OpenGL to provide data to the GPU. At the moment, this is a very small
game without an end, where you can run, jump and crawl around with an animated character. The goal of the
project was to be able to load a 3D model to the GPU and animate it if the model have bones. Gravity and hit
detection have also been added.

Further work: Separate large parts of the project and create game engine API out of it, which can be added as
.dll. Handling of multiple file types. Create better dynamic shaders based on existing information in the file.
