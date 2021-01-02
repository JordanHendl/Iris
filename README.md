# Karma
  Karma is a C++ framework for quick and easy program development. 
  
## How does it work?

  - Karma works by loading different mini-programs called 'modules' into a graph.
  - These modules are shared objects built from objects inheriting the "Karma Module" class, with each one acting as it's own program.
  - The Karma framework is set up by a JSON file that describes all the inputs needed. ( See @setup.json  )
  - One of the inputs, is another JSON file that describes the graphs and modules in each as well. ( See @karma_config.json )
  - Ideally, over time, there will be enough modules on my Github so that complex programs can be chained through JSON, and never require any code.
  - If waiting isn't your thing, then making your own Module is also easy, just link against karma_module of the Karma library, and create your own module inheriting the Karma Module class.
  - Communication between modules is done so using the Karma Data Bus class, which allows communication without dependencies. ( See https://github.com/JordanHendl/Data-Bus ).

## How to build
  To build, as with all Karma libraries, **CMake** is used.
  
  On linux:

  ```
  mkdir build
  cd build 
  cmake .. 
  make
  ```

  On windows I suggest using CMake GUI and MinGW-w64.

  On **Linux**, package installation installs to */usr/local/Karma*.
  On **Windows**, an installer is provided to install wherever you choose.

  In the install directory, lies a KarmaConfig.cmake, all you have to do is add a path to that to your CMAKE_PREFIX_PATH and then link against whatever library you need.

## Usage

  Usage is very simple. All you have to do to make an object of the Karma class, and call initialize with the correct JSON file path.
  
  ### Example
  ```
  #include <karma/Karma.h>

  int main()
  {
    Karma karma ;
    karma.initialize( "setup.json" ) ;

    // Let the karma modules do their work.
    while( karma.running() ) {} ;
    
    // Shutdown when done.
    karma.shutdown() ;

    return 0 ;
  }
  ```

## Planned

  - Design wise, interaction with each module through drivers is planned.
      E.g. A module calculated something using the GPU, and can be obtained through a callback in your driver program when it is completed.

  - A Base Driver program, in the case that you just want to run a bunch of modules and want to write 0 code at all!

  - Many, many modules.

  - A Python GUI helper program to alleviate having to write any JSON.

## Final Words

  If you have any questions or thoughts feel free to shoot then at me at my email jordiehendl@gmail.com or at my twitter ( @jajajordie )!
  I work on this stuff any free time I get so please be patient!




  

