# Iris
  Iris is a C++ Node-based framework for quick and easy program development.
  
## How to build
  Iris is built using **CMake**. If on Linux, to build from source, simply do: 
  
  ```
  mkdir build
  cd build
  cmake ..
  make 
  ```
  
  For Windows, do 
  
  ```
  mkdir build
  cd build
  cmake ..
  ```
  
  And then simply open the generated .sln with whatever editor of your preference and build.

  Generated RPM's default install to /usr/local/Iris on UNIX, and C:\Program Files\Iris on Windows.

  Linking with CMake: 
  1) Add the path to the install to your *CMAKE_PREFIX_PATH*.
  2) ```FIND_PACKAGE( Iris ) ```
  3) Link against the **iris** library.

## Usage

  Usage is very simple. All you have to do to make an object of the Iris class, and call initialize with the correct JSON file path.
  
  ### Example
  ```
  #include <iris/Iris.h>

  int main()
  {
    Iris iris ;
    iris.initialize( "setup.json" ) ;

    // Let the iris modules do their work.
    while( iris.running() ) {} ;
    
    // Shutdown when done.
    iris.shutdown() ;

    return 0 ;
  }
  ```

## Planned

  - Interaction with each module through drivers is planned.
      E.g. A module calculated something using the GPU, and can be obtained through a callback in your driver program when it is completed.

  - A Base Driver program, in the case that you just want to run a bunch of modules and want to write 0 code at all!

  - Many, many modules.

  - A Python GUI helper program to alleviate having to write any JSON.

## Final Words

  If you have any questions or thoughts feel free to shoot then at me at my email jordiehendl@gmail.com or at my twitter ( @jajajordie )!
  I work on this stuff any free time I get so please be patient!




  

