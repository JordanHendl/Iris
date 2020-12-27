#ifndef KARMA_TIMER_H
#define KARMA_TIMER_H

namespace karma
{
  namespace ms
  {
    /** Class for timing events in the program.
     */
    class Timer
    {
      public:
        
        /** Default constructor. Initializes implementation.
         */
        Timer() ;
        
        /** Default Deconstructor. Releases implementation.
         */
        ~Timer() ;
        
        /** Method to initialize this timer.
         * @param name The name to associate with this timer and it's output.
         */
        void initialize( const char* name = "" ) ;
        
        /** Method to check whether or not this object is initialized.
         * @return Whether or not thie object is initialized.
         */
        bool isInitialized() const ;
        
        /** Method to start this timer.
         */
        void start() ;
        
        /** Method to toggle pausing of this timer.
         */
        void pause() ;
        
        /** Method to stop this timer.
         */
        void stop() ;
        
        /** Method to recieve the calculated time after a call to start & stop.
         * @return The amount of time elapsed, in milliseconds.
         */
        double time() const ;
        
        /** Method to print this object's calculated time to stdout.
         */
        void print() const ;
        
        /** Method to retrieve the output string of this object. This is what gets outputted in @print.
         * @return The string representation of this object's output.
         */
        const char* output() const ;
      private:
        
        /** Forward declared pointer to this object's underlying data.
         */
        struct TimerData* timer_data ;
        
        /** Method to retrieve reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        TimerData& data() ;
        
        /** Method to retrieve reference to this object's underlying data.
         * @return Reference to this object's underlying data.
         */
        const TimerData& data() const ;
    };
  }
}
#endif

